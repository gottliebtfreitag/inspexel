#include "usb2dynamixel/USB2Dynamixel.h"
#include "usb2dynamixel/MotorMetaInfo.h"
#include "globalOptions.h"
#include "commonTasks.h"

#include "simplyfuse/FuseFS.h"
#include "simplyfile/Epoll.h"

#include <algorithm>
#include <numeric>
#include <atomic>
#include <future>

#include <unistd.h>
#include <functional>
#include <csignal>

namespace {

void runFuse();
auto interactCmd  = sargp::Command{"fuse", "create a fuse fs representing the connected motors", runFuse};
auto optTimeout   = interactCmd.Parameter<int>(10000, "timeout", "timeout in us");
auto ids          = interactCmd.Parameter<std::set<int>>({}, "ids", "the target Id");
auto mountPoint   = interactCmd.Parameter<std::string>("dynamixelFS", "mountpoint", "where to mount the fuse filesystem representing the motors");
using namespace dynamixel;

struct RegisterFile : simplyfuse::FuseFile {
	RegisterFile(MotorID _motorID, int _registerID, meta::LayoutField const& _layoutField, USB2Dynamixel &_usb2dyn)
		: motorID(_motorID)
		, registerID(_registerID)
		, layoutField(_layoutField)
		, usb2dyn(_usb2dyn)
	{}

	virtual ~RegisterFile() = default;

	int onRead(char* buf, std::size_t size, off_t)  override {
		if (not (int(layoutField.access) & int(meta::LayoutField::Access::R))) {
			return -EINVAL;
		}
		auto [timeout, motor, error, parameters] = usb2dyn.read(motorID, registerID, layoutField.length, std::chrono::microseconds{g_timeout});
		if (timeout or motor != motorID or parameters.size() != layoutField.length) {
			return -EINVAL;
		}
		int content {0};
		memcpy(&content, parameters.data(), std::min(sizeof(content), std::size_t(layoutField.length)));

		std::string renderedContent = std::to_string(content) + "\n";
		size = std::min(size, renderedContent.size()+1);
		std::memcpy(buf, renderedContent.data(), size);
		return size;
	}

	int onWrite(const char* buf, std::size_t size, off_t) override {
		if (not (int(layoutField.access) & int(meta::LayoutField::Access::W))) {
			return -ENOENT;
		}
		try {
			std::string stripped;
			std::stringstream{std::string{buf, size}} >> stripped;
			int toSet = sargp::parsing::detail::parseFromString<int>(stripped);
			Parameter param;
			for (std::size_t i{0}; i < layoutField.length; ++i) {
				param.emplace_back(std::byte{reinterpret_cast<uint8_t const*>(&toSet)[i]});
			}
			usb2dyn.write(motorID, registerID, param);
			return size;
		} catch (std::exception const&) {}
		return -ENOENT;
	}

	int onTruncate(off_t) override {
		return 0;
	}

	std::size_t getSize() override {
		return 4096;
	}

	int getFilePermissions() override {
		int permissions {0};
		if (int(layoutField.access) & int(meta::LayoutField::Access::R)) {
			permissions |= 0444;
		}
		if (int(layoutField.access) & int(meta::LayoutField::Access::W)) {
			permissions |= 0222;
		}
		return permissions;
	}

	MotorID motorID;
	int registerID;
	meta::LayoutField layoutField;
	USB2Dynamixel &usb2dyn;
};

struct PingFile : simplyfuse::SimpleWOFile {
	std::function<bool(MotorID)> callback;
	PingFile(std::function<bool(MotorID)> cb) : callback{cb} {}

	int onWrite(const char* buf, std::size_t size, off_t) override {
		int motorID;
		if (std::stringstream{std::string{buf, size}} >> motorID) {
			if (motorID >= 0xff) {
				return -EINVAL;
			}
			if (callback(motorID)) {
				return size;
			}
		}
		return -EINVAL;
	}
};

std::atomic<bool> terminateFlag {false};

template <LayoutType LT>
std::vector<std::unique_ptr<simplyfuse::FuseFile>> registerMotor(MotorID motorID, int modelNumber, USB2Dynamixel& usb2dyn, simplyfuse::FuseFS& fuseFS) {
	std::vector<std::unique_ptr<simplyfuse::FuseFile>> files;

	auto motorInfoPtr = meta::getMotorInfo(modelNumber);
	auto& motorModelFile = files.emplace_back(std::make_unique<simplyfuse::SimpleROFile>(motorInfoPtr->shortName + "\n"));
	fuseFS.rmdir("/" + std::to_string(motorID));
	fuseFS.registerFile("/" + std::to_string(motorID) + "/motor_model", *motorModelFile);

	using Info = meta::MotorLayoutInfo<LT>;
	auto const& infos = Info::getInfos();
	for (auto const& [reg, info] : infos) {
		auto& newFile = files.emplace_back(std::make_unique<RegisterFile>(motorID, int(reg), info, usb2dyn));
		fuseFS.registerFile("/" + std::to_string(motorID) + "/by-register-name/" + info.name, *newFile);
		fuseFS.registerFile("/" + std::to_string(motorID) + "/by-register-id/" + std::to_string(int(reg)), *newFile);
	}
	return files;
}

void runFuse() {
	auto timeout = std::chrono::microseconds{g_timeout};
	auto usb2dyn = USB2Dynamixel(g_baudrate, g_device.get(), Protocol{g_protocolVersion.get()});

	std::vector<int> range;
	if (g_id.isSpecified()) {
		range = {MotorID(g_id)};
	} else if (ids.isSpecified()) {
		for (auto x : ids.get()) {
			range.push_back(x);
		}
	} else {
		range.resize(0xfe);
		std::iota(begin(range), end(range), 0);
	}

	simplyfuse::FuseFS fuseFS{mountPoint.get()};
	std::map<MotorID, std::vector<std::unique_ptr<simplyfuse::FuseFile>>> files;

	auto detectAndHandleMotor = [&](MotorID motor) {
		auto [layout, modelNumber] = detectMotor(MotorID(motor), usb2dyn, timeout);
		if (modelNumber == 0) {
			return false;
		}
		std::vector<std::unique_ptr<simplyfuse::FuseFile>> newFiles;
		meta::forAllLayoutTypes([&](auto const& info) {
			using Info = std::decay_t<decltype(info)>;
			if (layout == Info::Type) {
				newFiles= registerMotor<Info::Type>(motor, modelNumber,usb2dyn, fuseFS);
			}
		});
		files[motor] = std::move(newFiles);
		return true;
	};

	auto pingFile = PingFile(detectAndHandleMotor);

	fuseFS.registerFile("/detect_motor", pingFile);
	auto future = std::async(std::launch::async, [&]{
		// ping all motors
		for (auto motor : range) {
			if (terminateFlag) {
				break;
			}
			detectAndHandleMotor(motor);
		}
	});

	auto sigHandler = [](int){ terminateFlag = true; };
	std::signal(SIGINT, sigHandler);

	simplyfile::Epoll epoll;
	epoll.addFD(fuseFS.getFD(), [&](int){
		fuseFS.loop();
		epoll.modFD(fuseFS.getFD(), EPOLLIN|EPOLLONESHOT);
	}, EPOLLIN|EPOLLONESHOT);

	while (not terminateFlag) {
		epoll.work(1);
	}
}

}

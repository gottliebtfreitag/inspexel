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
auto interactCmd  = parameter::Command{"fuse", "create a fuse fs representing the connected motors", runFuse};
auto optTimeout   = interactCmd.Parameter<int>(10000, "timeout", "timeout in us");
auto ids          = interactCmd.Parameter<std::set<int>>({}, "ids", "the target Id");
auto mountPoint   = interactCmd.Parameter<std::string>("dynamixelFS", "mountpoint", "where to mount the fuse filesystem representing the motors");
using namespace dynamixel;

struct RegisterFile : simplyfuse::FuseFile {
	RegisterFile(MotorID _motorID, int _registerID, std::size_t _registerSize, USB2Dynamixel &_usb2dyn)
		: motorID(_motorID)
		, registerID(_registerID)
		, registerSize(_registerSize)
		, usb2dyn(_usb2dyn)
	{}

	virtual ~RegisterFile() = default;
	int onRead(char* buf, std::size_t size, off_t)  override {
		auto [timeout, motor, error, parameters] = usb2dyn.read(motorID, registerID, registerSize, std::chrono::microseconds{g_timeout});
		if (parameters.size() != registerSize or timeout or motor != motorID) {
			return 0;
		}
		int content {0};
		memcpy(&content, parameters.data(), registerSize);

		std::string renderedContent = std::to_string(content);
		size = std::min(size, renderedContent.size()+1);
		std::memcpy(buf, renderedContent.data(), size);
		return size;
	}

	int onWrite(const char* buf, std::size_t size, off_t) override {
		try {
			std::string stripped;
			std::stringstream{std::string{buf, size}} >> stripped;
			int toSet = parameter::parsing::detail::parseFromString<int>(stripped);
			Parameter param;
			for (std::size_t i{0}; i < registerSize; ++i) {
				param.emplace_back(std::byte{reinterpret_cast<uint8_t const*>(&toSet)[i]});
			}
			usb2dyn.write(motorID, registerID, param);
		} catch (std::exception const&) {}
		return -ENOENT;
	}

	int onTruncate(off_t) override {
		return 0;
	}

	std::size_t getSize() override {
		return registerSize;
	}

	MotorID motorID;
	int registerID;
	std::size_t registerSize;
	USB2Dynamixel &usb2dyn;
};

std::atomic<bool> terminateFlag {false};

template <meta::LayoutType LT>
std::vector<std::unique_ptr<simplyfuse::FuseFile>> registerMotor(MotorID motorID, USB2Dynamixel& usb2dyn, simplyfuse::FuseFS& fuseFS) {
	std::vector<std::unique_ptr<simplyfuse::FuseFile>> files;
	auto infos = meta::getLayoutInfos<LT>();
	for (auto const& [reg, info] : infos) {
		auto& newFile = files.emplace_back(std::make_unique<RegisterFile>(motorID, int(reg), info.length, usb2dyn));
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
	// build the fuseFS with directories for all motors
	std::vector<std::unique_ptr<simplyfuse::FuseFile>> files;

	auto future = std::async(std::launch::async, [&]{
		// ping all motors
		for (auto motor : range) {
			if (terminateFlag) {
				break;
			}
			auto [layout, modelNumber] = detectMotor(MotorID(motor), usb2dyn, timeout);
			std::vector<std::unique_ptr<simplyfuse::FuseFile>> newFiles;
			if (layout == 1) {
				newFiles = registerMotor<meta::LayoutType::V1>(motor, usb2dyn, fuseFS);
			} else if (layout == 2) {
				newFiles = registerMotor<meta::LayoutType::V2>(motor, usb2dyn, fuseFS);
			} else if (layout == 3) {
				newFiles = registerMotor<meta::LayoutType::Pro>(motor, usb2dyn, fuseFS);
			}
			files.insert(files.end(), std::make_move_iterator(newFiles.begin()), std::make_move_iterator(newFiles.end()));
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

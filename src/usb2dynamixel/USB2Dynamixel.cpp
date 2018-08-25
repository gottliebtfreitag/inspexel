#include "USB2Dynamixel.h"

#include <cstdio>
#include <cstring>

#include <condition_variable>
#include <sstream>
#include <thread>

#include <simplyfile/SerialPort.h>

namespace dynamixel {

struct USB2Dynamixel::Pimpl {
	simplyfile::SerialPort mPort;
	std::mutex             mMutex;

	std::chrono::high_resolution_clock::time_point now() const {
		return std::chrono::high_resolution_clock::now();
	}

	Pimpl(std::vector<std::string> const& deviceNames, int baudrate) {
		for (auto const& dev : deviceNames) {
			try {
				simplyfile::SerialPort p{dev, baudrate};
				if (p.valid()) {
					mPort = std::move(p);
					break;
				}
			} catch (...) {
			}
		}
		if (not mPort.valid()) {
			std::stringstream ss;
			ss << "Cannot open any port for usb2dynamixel. Tried: ";
			for (auto const& dev : deviceNames) {
				ss << dev << ", ";
			}
			throw std::runtime_error(ss.str());
		}
	}

	void writePacket(Parameter const& packet) const {
		uint32_t bytesWritten = 0;
		const size_t count = packet.size();
//		std::cout << "write: " << std::hex << std::setfill('0');
//		for (auto p : packet) {
//			 std::cout << std::setw(2) << int(p) << " ";
//		}
//		std::cout << std::dec << std::endl;
		do {
			ssize_t w = ::write(mPort, packet.data() + bytesWritten, count - bytesWritten);

			if (w < 0) {
				throw std::runtime_error(std::string{"write to the dyanmixel bus failed: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
			}
			bytesWritten += w;

		} while (bytesWritten < count);
	}

	void flushRead() const {
		uint8_t dummy;
		ssize_t r = 1;
		while (r > 0) {
			r = ::read(mPort, &dummy, sizeof(dummy)); // read flush
		}
	}

	/** tries to read up to incomingLength bytes from filedescriptor
	 *
	 * return value
	 *  [timeoutFlag, valid, parameters] = readPacket(...);
	 *
	 *  timeoutFlag indicates if a timeout has occurred
	 *  valid       inidcates if parameters form a valid packet
	 *  paremeters  is a vector with read bytes
	 */
	[[nodiscard]] auto readPacket(uint8_t incomingLength, Timeout timeout) const -> std::tuple<bool, bool, Parameter> {
		auto startTime = now();

		Parameter rxBuf(incomingLength);
		uint8_t *data = rxBuf.data();
		size_t bytesRead = 0;

		bool timeoutFlag = false;

		do {
			ssize_t r = ::read(mPort, data + bytesRead, incomingLength - bytesRead);
			if (r > 0) {
				bytesRead += r;
				// check if first two bytes are 0xff
				if (bytesRead >= 2) {
					if (rxBuf[1] != 0xff) {
						bytesRead = 0;
					} else if (rxBuf[0] == 0xff) {
						bytesRead = 0;
					} else {
						bytesRead -= 1;
						memmove(rxBuf.data(), rxBuf.data()+1, bytesRead);
					}
				}
			} else if (errno != EAGAIN) {
				throw std::runtime_error(std::string{"unexpected read error: "} + strerror(errno) + " (" + std::to_string(errno) + ")");
			}
			timeoutFlag = (timeout.count() != 0) and (now() - startTime >= timeout);
		} while (bytesRead < incomingLength && not timeoutFlag);

		bool valid = bytesRead == incomingLength and validatePacket(rxBuf);

		return std::make_tuple(timeoutFlag, valid, std::move(rxBuf));
	}

	bool validatePacket(Parameter const& rxBuf) const {
		bool success = 0xff == rxBuf[0];
		success &= 0xff == rxBuf[1];
		success &= 0xff != rxBuf[2];
		success &= rxBuf.size() - 4 == rxBuf[3];
		uint8_t checkSum = 0;
		for (size_t i(2); i < rxBuf.size(); ++i) {
			checkSum += rxBuf[i];
		}
		success &= 0xff == checkSum;
		return success;
	}

/*	void dumpBuffer(Parameter const& rxBuf) const {
		std::cout << TERM_RED "received malformed packet" << std::endl;
		for (auto byte : i_packet) {
			printf("0x%X ", byte);
		}
		std::cout << TERM_RESET "\n";
	}*/
};

USB2Dynamixel::USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames)
	: m_pimpl(std::make_unique<Pimpl>(deviceNames, baudrate))
{}

USB2Dynamixel::~USB2Dynamixel() {
}

namespace {
	[[nodiscard]]
	uint8_t calculateChecksum(Parameter const& packet) {
		uint32_t checkSum = 0;
		for (size_t i(2); i < packet.size(); ++i) {
			checkSum += packet[i];
		}
		return ~checkSum;
	}

	[[nodiscard]]
	auto constructBuffer(MotorID motorID, Instruction instr, std::optional<uint8_t> baseRegister = {}, std::vector<uint8_t> data = {}) {
		uint8_t length = 2 + data.size() + (baseRegister.has_value()?1:0);

		std::vector<uint8_t> txBuf {
			0xff, 0xff, motorID, length, uint8_t(instr)
		};
		if (baseRegister) {
			txBuf.push_back(baseRegister.value());
		}
		txBuf.insert(txBuf.end(), data.begin(), data.end());

		txBuf.push_back(calculateChecksum(txBuf));
		return txBuf;
	}
}

bool USB2Dynamixel::ping(MotorID motor, Timeout timeout)
{
	auto g = std::lock_guard(m_pimpl->mMutex);
	auto txBuf = constructBuffer(motor, Instruction::PING);

	m_pimpl->writePacket(txBuf);
	auto [timeoutFlag, valid, rxBuf] = m_pimpl->readPacket(6, timeout);
	return not timeoutFlag and valid;
}

auto USB2Dynamixel::read(MotorID motor, int baseRegister, uint8_t length, Timeout timeout) -> std::tuple<bool, bool, std::vector<uint8_t>> {
	auto g = std::lock_guard(m_pimpl->mMutex);
	auto txBuf = constructBuffer(motor, Instruction::READ, uint8_t(baseRegister), {length});

	m_pimpl->writePacket(txBuf);
	auto [timeoutFlag, valid, rxBuf] = m_pimpl->readPacket(6+length, timeout);

	auto rxBuf2 = std::vector<uint8_t>{};
	if (valid) {
		rxBuf2 = {std::next(begin(rxBuf), 5), std::next(begin(rxBuf), rxBuf.size()-1)};
	}
	return std::make_tuple(timeoutFlag, valid, rxBuf2);
}

void USB2Dynamixel::write(MotorID motor, int baseRegister, Parameter const& writeBuffer) {
	auto g = std::lock_guard(m_pimpl->mMutex);
	auto txBuf = constructBuffer(motor, Instruction::WRITE, uint8_t(baseRegister), writeBuffer);
	m_pimpl->writePacket(txBuf);
}

void USB2Dynamixel::reset(MotorID motor) {
	auto g = std::lock_guard(m_pimpl->mMutex);
	auto txBuf = constructBuffer(motor, Instruction::RESET);
	m_pimpl->writePacket(txBuf);
}

void USB2Dynamixel::sync_write(std::map<MotorID, Parameter> const& motorParams, int baseRegister) {
	auto g = std::lock_guard(m_pimpl->mMutex);

	if (motorParams.empty() > 0) {
		throw std::runtime_error("sync_write: motorParams can't be empty");
	}

	const uint8_t len = motorParams.begin()->second.size();
	bool const okay = std::all_of(begin(motorParams), end(motorParams), [&](auto param) {
		return param.second.size() == len;
	});

	if (len <= 0 or not okay) {
		throw std::runtime_error("sync_write: data is not consistent");
	}

	std::vector<uint8_t> data;
	data.push_back(len);
	for (auto const& [id, params] : motorParams) {
		data.push_back(id);
		data.insert(data.end(), params.begin(), params.end());
	}

	auto txBuf = constructBuffer(BroadcastID, Instruction::SYNC_WRITE, uint8_t(baseRegister), data);
	m_pimpl->writePacket(txBuf);
}

}

#include "USB2Dynamixel.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>

#include <thread>
#include <condition_variable>
#include <iomanip>

#include <simplyfile/SerialPort.h>

#define TERM_RED                        "\033[31m"
#define TERM_RESET                      "\033[0m"

namespace dynamixel
{

struct USB2Dynamixel::Pimpl
{
	std::chrono::high_resolution_clock::time_point now() const {
		return std::chrono::high_resolution_clock::now();
	}
	simplyfile::SerialPort port;

	Pimpl(std::vector<std::string> const& deviceNames, int baudrate) {
		for (auto const& dev : deviceNames) {
			try {
				simplyfile::SerialPort p{dev, baudrate};
				if (p.valid()) {
					port = std::move(p);
					break;
				}
			} catch (std::runtime_error const& err) {
				std::cerr << err.what() << std::endl;
			}
		}
		if (not port.valid()) {
			std::stringstream ss;
			ss << "Cannot open any port for usb2dynamixel. Tried: ";
			for (auto const& dev : deviceNames) {
				ss << dev << ", ";
			}
			throw std::runtime_error(ss.str());
		}
	}

	void writePacket(dynamixel::parameter const& packet)
	{
		const uint8_t *data = packet.data();
		uint bytesWritten = 0;
		const size_t count = packet.size();
//		std::cout << "write: " << std::hex << std::setfill('0');
//		for (auto p : packet) {
//			 std::cout << std::setw(2) << int(p) << " ";
//		}
//		std::cout << std::dec << std::endl;
		do {
			ssize_t w = ::write(port, data + bytesWritten, count - bytesWritten);

			if (w < 0) {
				throw std::runtime_error("write to the dyanmixel bus failed");
			} else {
				bytesWritten += w;
			}
		} while (bytesWritten < count);
	}

	bool readPacket(parameter &i_packet, uint8_t incommingLength, timeout_t timeout)
	{
		auto startTime = now();

		i_packet.resize(incommingLength);
		uint8_t *data = i_packet.data();
		size_t bytesRead = 0;

		int headerReceived = 0;
		do {
			ssize_t r = ::read(port, &i_packet[headerReceived], 1);
			if (r < 0) {
				continue;
			}
//			std::cout << int(i_packet[headerReceived]) << " ";
			if (i_packet[headerReceived] == 0xff) {
				if (headerReceived < 2) {
					++headerReceived;
				}
			} else if (2 == headerReceived) {
				bytesRead = 3;
				break;
			} else {
				headerReceived = 0;
			}
		} while (timeout.count()==0 || now() - startTime < timeout);
		do {
			ssize_t r = ::read(port, data + bytesRead, incommingLength - bytesRead);
			if (r > 0) {
				bytesRead += r;
			} else if (errno != EAGAIN) {
				std::cout << "r: " << r << std::endl;
				break;
			}
		} while (bytesRead < incommingLength && (timeout.count()==0 || now() - startTime < timeout));


		if (bytesRead != incommingLength) {
			uint8_t dummy;
			ssize_t r = 1;
			while (r > 0) {
				r = ::read(port, &dummy, sizeof(dummy)); // read flush
			}
		}
		i_packet.resize(bytesRead);
		return bytesRead == incommingLength;
	}

	bool validatePacket(parameter const& i_packet, uint8_t motorID) {
		bool success = 0xff == i_packet[0];
		success &= 0xff == i_packet[1];
		success &= motorID == i_packet[2];
		success &= i_packet.size() - 4 == i_packet[3];
		uint8_t checkSum = 0;
		for (uint i(2); i < i_packet.size(); ++i) {
			checkSum += i_packet[i];
		}
		success &= 0xff == checkSum;
		return success;
	}

	parameter getRxValuesFromPacket(parameter const& i_packet, uint8_t motorID) {
		parameter returnParams;
		bool success = validatePacket(i_packet, motorID);

		if (success) {
			returnParams.insert(returnParams.end(), i_packet.begin()+5, i_packet.end()-1);
		} else {
			std::cout << TERM_RED "received malformed packet" << std::endl;
			for (auto byte : i_packet) {
				printf("0x%X ", byte);
			}
			std::cout << TERM_RESET "\n";
		}

		return returnParams;
	}
};

USB2Dynamixel::USB2Dynamixel(int baudrate, std::vector<std::string> const& deviceNames)
	: m_pimpl(new Pimpl(deviceNames, baudrate))
{}

USB2Dynamixel::~USB2Dynamixel() {
}

void USB2Dynamixel::setBaudrate(uint newBaudrate)
{
	std::lock_guard<std::mutex> lock(mMutex);
	m_pimpl->port.setBaudrate(newBaudrate);
}

bool USB2Dynamixel::ping(dynamixel::motorID motor, timeout_t timeout)
{
	std::lock_guard<std::mutex> lock(mMutex);
	std::vector<uint8_t> txBuf{
		0xff, 0xff, motor, 2, uint8_t(dynamixel::Instruction::PING),
	};
	txBuf.push_back(calculateChecksum(txBuf));

	std::vector<uint8_t> rxBuf(6, 0);
	m_pimpl->writePacket(txBuf);
	m_pimpl->readPacket(rxBuf, rxBuf.size(), timeout);
	return m_pimpl->validatePacket(rxBuf, motor);
}

bool USB2Dynamixel::read(dynamixel::motorID motor, int baseRegister, uint8_t length, timeout_t timeout, callback cb)
{
	std::lock_guard<std::mutex> lock(mMutex);
	std::vector<uint8_t> txBuf{
		0xff, 0xff, motor, 4, uint8_t(dynamixel::Instruction::READ), uint8_t(baseRegister), length
	};
	txBuf.push_back(calculateChecksum(txBuf));
	std::vector<uint8_t> rxBuf(6+length, 0);

	m_pimpl->writePacket(txBuf);
	m_pimpl->readPacket(rxBuf, rxBuf.size(), timeout);
	parameter recParams = m_pimpl->getRxValuesFromPacket(rxBuf, motor);
	if (cb) {
		cb(motor, recParams.data(), recParams.size());
	}
	return recParams.size();
}

bool USB2Dynamixel::write(dynamixel::motorID motor, int baseRegister, dynamixel::parameter const& writeBuffer)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if (not writeBuffer.empty()) {
		std::vector<uint8_t> txBuf{
			0xff, 0xff, motor,
			uint8_t(3 + writeBuffer.size()), uint8_t(dynamixel::Instruction::WRITE), uint8_t(baseRegister),
		};
		txBuf.insert(txBuf.end(), writeBuffer.begin(), writeBuffer.end());
		txBuf.push_back(calculateChecksum(txBuf));
		m_pimpl->writePacket(txBuf);
		return true;
	}
	return false;
}

bool USB2Dynamixel::reset(dynamixel::motorID motor)
{
	std::lock_guard<std::mutex> lock(mMutex);
	std::vector<uint8_t> txBuf{
		0xff, 0xff, motor,
		2, uint8_t(dynamixel::Instruction::RESET),
	};
	txBuf.push_back(calculateChecksum(txBuf));
	m_pimpl->writePacket(txBuf);
	return true;
}

bool USB2Dynamixel::sync_write(std::map<dynamixel::motorID, dynamixel::parameter> const& motorParams, int baseRegister)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if (motorParams.size() > 0)
	{
		const uint8_t len = motorParams.begin()->second.size();
		bool okay = true;
		for (std::pair<dynamixel::motorID, dynamixel::parameter> const& param : motorParams)
		{
			okay &= param.second.size() == len;
		}

		if (len > 0 && okay)
		{
			const uint8_t packetLength = 6 + (len + 1) * motorParams.size();
			std::vector<uint8_t> txBuf;
			txBuf.reserve(packetLength);
			txBuf.push_back(0xff);
			txBuf.push_back(0xff);
			txBuf.push_back(dynamixel::broadcastID);
			txBuf.push_back(4 + (len + 1) * motorParams.size());
			txBuf.push_back(uint8_t(dynamixel::Instruction::SYNC_WRITE));
			txBuf.push_back(uint8_t(baseRegister));
			txBuf.push_back(len);

			for (auto const& param : motorParams) {
				txBuf.push_back(param.first);
				txBuf.insert(txBuf.end(), param.second.begin(), param.second.end());
			}

			txBuf.push_back(calculateChecksum(txBuf));
			m_pimpl->writePacket(txBuf);
			return true;
		}
	}
	return false;
}


uint8_t USB2Dynamixel::calculateChecksum(dynamixel::parameter const& packet) const
{
	int checkSum = 0;
	for (uint i(2); i < packet.size(); ++i) {
		checkSum += packet[i];
	}
	return ~checkSum;
}
}

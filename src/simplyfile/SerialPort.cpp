#include "SerialPort.h"

#include <fcntl.h>
#include <string.h>        // String function definitions
#include <unistd.h>        // UNIX standard function definitions
#include <errno.h>         // Error number definitions
#include <termios.h>
#include <sys/ioctl.h>
#include <inttypes.h>
#include <linux/types.h>
#include <linux/serial.h>
#include <iostream>
#include <stdexcept>

namespace simplyfile
{

namespace {

struct termios2 {
	tcflag_t c_iflag;		/* input mode flags */
	tcflag_t c_oflag;		/* output mode flags */
	tcflag_t c_cflag;		/* control mode flags */
	tcflag_t c_lflag;		/* local mode flags */
	cc_t c_line;			/* line discipline */
	cc_t c_cc[19];		/* control characters */
	speed_t c_ispeed;		/* input speed */
	speed_t c_ospeed;		/* output speed */
};
#ifndef BOTHER
#define BOTHER 0010000
#endif

}

SerialPort::SerialPort(std::string const& name, std::optional<int> baudrate)
	: mHasOptionBaudrate {baudrate}
{
	FileDescriptor iFace{::open(name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY)};

	if (not iFace.valid()) {
		throw std::runtime_error("cannot open serial port: " + name);
	}

	// when reading, return immediately
	if (0 > fcntl(iFace, F_SETFL, FNDELAY)) {
		throw std::runtime_error("F_SETFL " +std::string(strerror(errno)));
	}

	if (baudrate) {
		struct serial_struct serial;
		bzero(&serial, sizeof(serial));
		if (0 > ioctl(iFace, TIOCGSERIAL, &serial)) {
			throw std::runtime_error("TIOCGSERIAL " +std::string(strerror(errno)));
		}

		serial.flags |= ASYNC_LOW_LATENCY;  /* enable low latency  */
		if (0 > ioctl(iFace, TIOCSSERIAL, &serial)) {
			mHasOptionBaudrate = false;
		}
	}

	struct termios2 options;
	bzero(&options, sizeof(options));
	if (0 > ioctl(iFace, TCGETS2, &options)) {
		throw std::runtime_error("TCGETS2 " +std::string(strerror(errno)));
	}

	// local line that supports reading
	options.c_cflag |= (CLOCAL | CREAD);

	// set 8N1
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	// set baudrate
	if (mHasOptionBaudrate) {
		options.c_ospeed = *baudrate;
		options.c_ispeed = *baudrate;
		options.c_cflag  &= ~CBAUD;
		options.c_cflag  |= BOTHER;
	}

	// disable hardware flow control
	options.c_cflag &= ~CRTSCTS;

	// use raw mode (see "man cfmakeraw")
	options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	options.c_cflag &= ~(CSIZE | PARENB);
	options.c_cflag |= CS8;

	if (0 > ioctl(iFace, TCSETS2, &options)) {
		throw std::runtime_error("TCSETS2 " +std::string(strerror(errno)));
	}

	FileDescriptor::operator=(std::move(iFace));
}


SerialPort::SerialPort(SerialPort&& other)
	: FileDescriptor(std::move(other))
{}

SerialPort& SerialPort::operator=(SerialPort&& other) {
	FileDescriptor::operator=(std::move(other));
	mHasOptionBaudrate = other.mHasOptionBaudrate;
	return *this;
}

bool SerialPort::hasOptionBaudrate() const {
	return mHasOptionBaudrate;
}


}

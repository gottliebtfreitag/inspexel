#include "ClientSocket.h"

#include <cstring>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <unistd.h>

namespace simplyfile {

ClientSocket::ClientSocket(int _fd, Host const& _host)
	: FileDescriptor(_fd)
	, host(_host)
{}

ClientSocket::ClientSocket(Host const& _host)
	: FileDescriptor(::socket(_host.family, _host.socktype, _host.protocol))
	, host(_host)
{}

void ClientSocket::connect() {
	if (::connect(*this, reinterpret_cast<struct sockaddr const*>(&host.sockaddr), host.sockaddrLen) != 0) {
		throw std::runtime_error("cannot connect to host: " + host.getName() + " errno: " + std::string(strerror(errno)));
	}
}

void ClientSocket::send(std::string_view view) const {
	if (::send(*this, &view[0], view.size(), MSG_NOSIGNAL) == -1) {
		throw std::runtime_error("error sending data: " + std::string(strerror(errno)));
	}
}


}

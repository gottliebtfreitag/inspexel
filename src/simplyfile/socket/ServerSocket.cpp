#include "ServerSocket.h"

#include <cerrno>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <unistd.h>

namespace simplyfile {

ServerSocket::ServerSocket(Host const& _host, bool reusePort)
	: host(_host)
{
	FileDescriptor sock = ::socket(host.family, host.socktype, host.protocol);
	int optVal = reusePort;
    ::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optVal, sizeof(optVal));
	if (not sock.valid()) {
		throw std::runtime_error("cannot create socket " + std::string(strerror(errno)));
	}
	if (host.family == AF_LOCAL) {
		// for unix sockets we might need to unlink the socket-file that is being created
		struct sockaddr_un& address = reinterpret_cast<struct sockaddr_un&>(host.sockaddr);
		::unlink(address.sun_path);
	}
	if (::bind(sock, reinterpret_cast<struct sockaddr const*>(&host.sockaddr), host.sockaddrLen)) {
		throw std::runtime_error("cannot bind socket to: " + host.getName() + " reason: " + std::string(strerror(errno)));
	}
	FileDescriptor::operator =(std::move(sock));
}

ServerSocket::~ServerSocket() {
	if (valid()) {
		if (host.family == AF_LOCAL) {
			// for unix sockets we might need to unlink the socket-file that is being created
			struct sockaddr_un& address = reinterpret_cast<struct sockaddr_un&>(host.sockaddr);
			::unlink(address.sun_path);
		}
	}
}

ClientSocket ServerSocket::accept() const {
	Host h = host;
	int _fd = ::accept(*this, reinterpret_cast<struct sockaddr*>(&h.sockaddr), &h.sockaddrLen);
	ClientSocket socket{_fd, h};
	if (not socket.valid()) {
		throw std::runtime_error("cannot accept incoming socket " + std::string(strerror(errno)));
	}
	return socket;
}

void ServerSocket::listen() const {
	if (::listen(*this, 0)) {
		throw std::runtime_error("cannot listen on socket " + std::string(strerror(errno)));
	}
}

}

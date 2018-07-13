#pragma once

#include "../FileDescriptor.h"
#include "Host.h"

namespace simplyfile
{

struct ClientSocket : FileDescriptor {
	using FileDescriptor::FileDescriptor;

	ClientSocket(Host const& host);
	// this constructor is used by ServerSocket::accept() to pass peer information to the socket
	ClientSocket(int fd, Host const& host);

	void connect();
	Host const& getHost() const {
		return host;
	}
	int getBytesAvailable() const;
protected:
	Host host;
};

struct ServerSocket : FileDescriptor {
	ServerSocket(Host const& host, bool reusePort=true);
	ServerSocket(ServerSocket&&)= default;
	ServerSocket& operator=(ServerSocket&&)= default;

	virtual ~ServerSocket();

	ClientSocket accept();
	void listen();
protected:
	Host host;
};

}



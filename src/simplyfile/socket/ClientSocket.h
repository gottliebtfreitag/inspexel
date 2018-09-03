#pragma once

#include "../FileDescriptor.h"
#include "Host.h"

#include <string_view>

namespace simplyfile {

struct ClientSocket : FileDescriptor {
	using FileDescriptor::FileDescriptor;

	ClientSocket(Host const& host);
	// this constructor is used by ServerSocket::accept() to pass peer information to the socket
	ClientSocket(int fd, Host const& host);

	void connect();
	Host const& getHost() const {
		return host;
	}

	void send(std::string_view) const;

protected:
	Host host;
};
}

#pragma once

#include "ClientSocket.h"

namespace simplyfile {

struct ServerSocket : FileDescriptor {
	using FileDescriptor::FileDescriptor;
	ServerSocket(Host const& host, bool reusePort=true);
	ServerSocket(ServerSocket&&)= default;
	ServerSocket& operator=(ServerSocket&&)= default;

	~ServerSocket() override;

	ClientSocket accept() const;
	void listen() const;
protected:
	Host host;
};

}

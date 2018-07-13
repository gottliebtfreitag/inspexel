#pragma once

#include <sys/socket.h>

#include <string>
#include <vector>

namespace simplyfile
{

struct Host {
	int family {};
	int socktype {};
	int protocol {};
	socklen_t sockaddrLen {};
	struct sockaddr_storage sockaddr {};

	std::string getName() const;
};

std::vector<Host> getHosts(std::string const& node, std::string const& service, int socktype=SOCK_STREAM, int family = AF_UNSPEC);

Host makeUnixDomainHost(std::string const& path, int socktype=SOCK_STREAM);
}




#include "Host.h"
#include <cstring>
#include <errno.h>
#include <stdexcept>
#include <netdb.h>
#include <sys/un.h>

namespace simplyfile
{


std::string Host::getName() const {
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    if (getnameinfo(reinterpret_cast<struct sockaddr const*>(&sockaddr), sockaddrLen, hbuf, sizeof(hbuf), sbuf,
                sizeof(sbuf), 0) != 0) {
    	throw std::runtime_error("getnameinfo returned an error");
    }
    return std::string(hbuf) + ":" + std::string(sbuf);
}


std::vector<Host> getHosts(std::string const& node, std::string const& service, int socktype, int family) {
	std::vector<Host> hosts;

	struct addrinfo *result;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = family;
	hints.ai_socktype = socktype;
	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
	hints.ai_protocol = 0;          /* Any protocol */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	if (getaddrinfo(node.c_str(), service.c_str(), &hints, &result)) {
		throw std::runtime_error("cannot call getaddrinfo");
	}
	for (struct addrinfo *rp = result; rp != NULL; rp = rp->ai_next) {
		Host h;
		h.family      = rp->ai_family;
		h.socktype    = rp->ai_socktype;
		h.protocol    = rp->ai_protocol;
		h.sockaddrLen = rp->ai_addrlen;
		memcpy(&h.sockaddr, rp->ai_addr, h.sockaddrLen);
		hosts.emplace_back(h);
	}
	freeaddrinfo(result);           /* No longer needed */

	return hosts;
}


Host makeUnixDomainHost(std::string const& path, int socktype) {
	Host host;
	host.family   = AF_LOCAL;
	host.protocol = 0;
	host.socktype = socktype;
	struct sockaddr_un& address = reinterpret_cast<struct sockaddr_un&>(host.sockaddr);
	address.sun_family = AF_LOCAL;
	strncpy(address.sun_path, path.c_str(), std::min(sizeof(address.sun_path), path.size()));
	host.sockaddrLen = sizeof(address);
	return host;
}


}

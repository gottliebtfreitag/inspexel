#include "simplyfile/socket/ServerSocket.h"
#include "simplyfile/Timer.h"
#include "simplyfile/Epoll.h"

#include <iostream>
#include <algorithm>

void ioStuff(simplyfile::Epoll& epoll) {
	auto hosts = simplyfile::getHosts("localhost", "8080");
	hosts.emplace_back(simplyfile::makeUnixDomainHost("testSock"));

	std::vector<simplyfile::ServerSocket> servers;
	std::transform(hosts.begin(), hosts.end(), std::back_inserter(servers), [](simplyfile::Host const& host) {
		auto server = simplyfile::ServerSocket{host};
		std::cout << "opened server on: " << host.getName() << std::endl;
		server.listen();
		return server;
	});

	std::vector<simplyfile::ClientSocket> clients;

	auto removeClient = [&](simplyfile::ClientSocket& c) {
		clients.erase(
				std::remove_if(clients.begin(), clients.end(), [&c](simplyfile::ClientSocket const& s) {return &s == &c;})
		);
	};

	for (auto& server : servers) {
		server.setFlags(O_NONBLOCK);
		epoll.addFD(server, [&] (int) {
			auto& c = clients.emplace_back(server.accept());
			if (not c.valid()) {
				return;
			}
			std::cout << "incomming connection from: " << c.getHost().getName() << std::endl;
			epoll.addFD(c, [&] (int clientEvents) {
				try {
					if (clientEvents & (EPOLLHUP|EPOLLERR|EPOLLRDHUP)) {
						removeClient(c);
						return;
					}
					if (clientEvents & EPOLLIN) {
						std::vector<uint8_t> incomming;
						incomming.resize(getAvailableBytes(c));
						int received = ::recv(c, incomming.data(), incomming.size(), MSG_NOSIGNAL);
						int sent = ::send(c, incomming.data(), incomming.size(), MSG_NOSIGNAL);
						std::cout << "received: " << received << " sent: " << sent << std::endl;
						if (received != sent) {
							removeClient(c);
						}
					}
				} catch (std::exception const&) {
					removeClient(c);
				}
			}, EPOLLIN|EPOLLHUP|EPOLLRDHUP|EPOLLET);
			epoll.modFD(server, EPOLLIN|EPOLLONESHOT);
		}, EPOLLIN|EPOLLONESHOT);
	}
	simplyfile::ClientSocket uds1{simplyfile::makeUnixDomainHost("testSock")};
	uds1.connect();

	using namespace std::chrono_literals;
	simplyfile::Timer timer{1s};

	epoll.addFD(timer, [&](int){
		std::cout << "timer tick" << std::endl;
		timer.getElapsed();
	});
}

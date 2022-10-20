#include "Server.hpp"

Server::Server() {

	listenFd = 0;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(PORT);
}

Server::~Server() {
	close(listenFd);
}

void	Server::initListenSocket() {
	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenFd < 0) {
		std::cerr << "Socket: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Listening socket created..." << std::endl;
}

void	Server::makeBind() {
	if (bind(listenFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cerr << "Bind: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Binding... success" << std::endl;
}

void	Server::makeListenQueue() {
	if (listen(listenFd, QUEUE) < 0) {
		std::cerr << "Listen: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "Making queue of " << QUEUE << "... success" << std::endl;
}

void	Server::printStatus() {
	std::cout << "Server started on address: " << inet_ntoa(addr.sin_addr);
	std::cout << ", port: " << ntohs(addr.sin_port) << std::endl;
}

void	Server::launch() {
	socklen_t	addrlen = sizeof(addr);
	while (true) {
		int connectionFd = accept(listenFd, (struct sockaddr *)&addr, &addrlen);
		if (connectionFd < 0) {
			std::cerr << "Accept: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		char buf[2048] = {0};
		recv(connectionFd, buf, sizeof(buf), 0);
		std::cout << buf << std::endl;
		close(connectionFd);
	}
}

int	Server::getFd() const {
	return listenFd;
}
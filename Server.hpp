#ifndef Server_hpp
#define Server_hpp

#include <sys/socket.h> //socket
#include <netinet/in.h> //sockaddr_in
#include <cerrno> //errno
#include <cstring> //memset, strerror
#include <unistd.h> //close
#include <arpa/inet.h> //inet_ntoa
#include <iostream>

//#define	HOST "127.0.0.1"
#define PORT 8080
#define QUEUE 10

class Server {
private:

	int					listenFd;
	struct sockaddr_in	addr;

public:

	Server();
	~Server();

	int	getFd() const;

	void	initListenSocket();
	void	makeBind();
	void	makeListenQueue();
	void	printStatus();
	void	launch();
};

#endif //Server.hpp
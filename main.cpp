#include "Server.hpp"

int	main(int argc, char **argv) {

	if (argc > 2) {
		std::cout << "Usage: ./webserv [config.file]" << std::endl;
		return 1;
	}

	if (argv[1])
		std::cout << "Config specified ..." << std::endl;
	else
		std::cout << "Default config loaded ..." << std::endl;

	Server serv;

	serv.initListenSocket();
	serv.makeBind();
	serv.makeListenQueue();
	serv.printStatus();
	serv.launch();
	
	return 0;
}
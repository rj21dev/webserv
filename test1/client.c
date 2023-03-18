#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 5555

int main(void) {
	int			client_fd;
	int			new_socket;
	socklen_t		addrlen;
	struct sockaddr_in	serv_addr;

	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	addrlen = sizeof(serv_addr);

	if (connect(client_fd, (struct sockaddr *)&serv_addr, addrlen) < 0) {
		perror("Error: Connect");
		exit(EXIT_FAILURE);
	}

	char *request = "Hello from client";
	char buff[3000] = {0};
	send(client_fd, request, strlen(request), 0);
	printf("Request sent\n");
	recv(client_fd, buff, 3000, 0);
	printf("%s\n", buff);
	printf("Responce recieved\n");
	close(client_fd);
	return 0;
}

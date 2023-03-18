#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define PORT 5555

int main(void) {
	int			server_fd;
	int			new_socket;
	socklen_t		addrlen;
	struct sockaddr_in	address;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	addrlen = sizeof(address);

	if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0) {
		perror("Bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 5) < 0) {
		perror("Listen");
		exit(EXIT_FAILURE);
	}
	
	//char *body = "Hello from server!\n";
	char response[500] = {0};
	char body[5000] = {0};
	int fd = open("index.html", O_RDONLY);
	read(fd, body, 5000);
	close(fd);
	sprintf(response, "HTTP/1.1 200 OK\nContent-Tyoe: text/plain\nContent-Length: %lu\n\n%s", strlen(body), body);

	while (1) {
		printf("Waiting for connection\n");
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
			perror("Accept");
			exit(EXIT_FAILURE);
		}
		char buff[3000] = {0};
		read(new_socket, buff, 3000);
		printf("Request recieved\n");	
		printf("%s", buff);
		write(new_socket, response, strlen(response));
		printf("Response sent\n");	
		close(new_socket);
	}
	close(server_fd);
	return 0;
}

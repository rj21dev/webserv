#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT	5555
#define BUFLEN		512

int write_to_client(int fd, char *buf);
int read_from_client(int fd, char *buf);

int main(void)
{
	int err, i, opt = 1;
	int sock, new_sock;
	struct sockaddr_in	addr;
	struct sockaddr_in	client;
	fd_set			active_set, read_set;
	socklen_t size;
	char buf[BUFLEN];

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Server: socket was not created");
		exit(EXIT_FAILURE);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

	err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (err < 0) {
		perror("Client: connect failure");
		exit(EXIT_FAILURE);
	}

	err = listen(sock, 3);
	if (err < 0) {
		perror("Server: listen queue failure");
		exit(EXIT_FAILURE);
	}
	
	FD_ZERO(&active_set);
	FD_SET(sock, &active_set);

	while (1)
	{
		read_set = active_set;
		if (select(FD_SETSIZE, &read_set, 0, 0, 0) < 0) {
			perror("Server: select failure");
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &read_set)) {
				if (i == sock) {
					size = sizeof(client);
					new_sock = accept(sock, (struct sockaddr *)&client, &size);
					if (new_sock < 0) {
						perror("accept");
						exit(EXIT_FAILURE);
					}
					printf("Server: connect from host %s, port %hu.\n", 
						inet_ntoa(client.sin_addr), ntohs(client.sin_port));
					FD_SET(new_sock, &active_set);
				} else {
					err = read_from_client(i, buf);
					if (err < 0) {
						close(i);
						FD_CLR(i, &active_set);
					} else {
						if (strstr(buf, "stop")) {
							close(i);
							FD_CLR(i, &active_set);
						} else {
							write_to_client(i, buf);
						}
					}
				}
			}
		}
	}

	close(sock);
	return 0;
}

int write_to_client(int fd, char *buf)
{
	int	nbytes;
	char	*s;

	for (s = buf; *s; s++) *s = toupper(*s);
	nbytes = write(fd, buf, strlen(buf) + 1);
	printf("Write back: %s\nnbytes=%d\n", buf, nbytes);
	if (nbytes < 0) return perror("Server: write failure"), -1;
	return 0;
}

int read_from_client(int fd, char *buf)
{
	int	nbytes;

	nbytes = read(fd, buf, BUFLEN);
	if (nbytes < 0)
		return perror("Server: read failure"), -1;
	else if (nbytes == 0)
		return -1;
	else
		fprintf(stdout, "Server got message: %s\n", buf);
	return 0;
}

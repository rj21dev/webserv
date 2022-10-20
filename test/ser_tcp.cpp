#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT		5555
#define BUFLEN		512
char	buf[BUFLEN];

void print_addr(const struct sockaddr_in &addr, const char *text)
{
	if (text) printf("%s", text);
	int f = addr.sin_family;
	int p = addr.sin_port;
	printf("type %d\n", f);
	printf("port %d\n", ntohs(p));
	printf("ip addr %s\n", inet_ntoa(addr.sin_addr));
}			

int main(void)
{
	int sock;
	int nbytes, i, err;
	unsigned int size;
	struct sockaddr_in addr_to;
	struct sockaddr_in addr_from;
	struct sockaddr_in addr_this;

	addr_this.sin_family = AF_INET;
	addr_this.sin_port = htons(PORT);
	addr_this.sin_addr.s_addr = INADDR_ANY;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Server: socket was not created");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

	err = bind(sock, (struct sockaddr *)&addr_this, sizeof(addr_this));
	if (err < 0) {
		perror("Server: cannot bind socket");
		exit(EXIT_FAILURE);
	}
	
	listen(sock, 3);
	
	size = sizeof(addr_from);
	int newsock = accept(sock, (struct sockaddr *)&addr_from, &size);
	//nbytes = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&addr_from, &size);
	
	nbytes = recv(newsock, buf, BUFLEN, 0);
	print_addr(addr_from, "get message from\n");
	printf("%d bytes :\n%s\n", nbytes, buf);
	
	strcat(buf, " Echo");

	nbytes = send(newsock, buf, strlen(buf) + 1, 0);
	if (nbytes < 0) {
		fprintf(stderr, "cannot send data\n");
		perror("Server");
		close(sock);
		exit(EXIT_FAILURE);
	} else {
		printf("sending message of %d bytes\n", nbytes);
	}
	
	close(sock);
	close(newsock);

	return 0;
}

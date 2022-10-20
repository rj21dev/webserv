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

#define PORT_THIS	5556
#define PORT_TO		5555
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
	struct hostent	*hostinfo;

	hostinfo = gethostbyname("127.0.0.1");
	if (!hostinfo) {
		fprintf(stderr, "Unknown host\n");
		exit(EXIT_FAILURE);
	}

	addr_to.sin_family = hostinfo->h_addrtype;
	addr_to.sin_port = htons(PORT_TO);
	addr_to.sin_addr = *(struct in_addr *) hostinfo->h_addr;

	addr_this.sin_family = AF_INET;
	addr_this.sin_port = htons(PORT_THIS);
	addr_this.sin_addr.s_addr = INADDR_ANY;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

	//err = bind(sock, (struct sockaddr *)&addr_this, sizeof(addr_this));
	err = connect(sock, (struct sockaddr *)&addr_to, sizeof(addr_to));
	if (err < 0) {
		perror("Client: cannot connect socket");
		exit(EXIT_FAILURE);
	}

	printf("Your message: ");
	fgets(buf, BUFLEN, stdin);
	size = strlen(buf) + 1;

	//print_addr(addr_to, "sending to\n");
	//nbytes = sendto(sock, buf, size, 0, (struct sockaddr *)&addr_to, sizeof(addr_to));
	nbytes = send(sock, buf, size, 0);
	if (nbytes < 0) {
		fprintf(stderr, "cannot send data\n");
		perror("Client");
		close(sock);
		exit(EXIT_FAILURE);
	} else {
		printf("sending message of %d bytes\n", nbytes);
	}
	
	//size = sizeof(addr_from);
	//nbytes = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *)&addr_from, &size);
	nbytes = recv(sock, buf, BUFLEN, 0);
	//:print_addr(addr_from, "get message from\n");
	printf("%d bytes :\n%s\n", nbytes, buf);
	close(sock);

	return 0;
}

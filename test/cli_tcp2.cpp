#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT	5555
#define SERVER_NAME	"127.0.0.1"
#define BUFLEN		512

int write_to_server(int fd);
int read_from_server(int fd);

int main(void)
{
	int err;
	int sock;
	struct sockaddr_in	server_addr;
	struct hostent		*hostinfo;

	hostinfo = gethostbyname(SERVER_NAME);
	if (hostinfo == NULL) {
		fprintf(stderr, "Unlnown host %s.\n", SERVER_NAME);
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}

	err = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err < 0) {
		perror("Client: connect failure");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Connection is ready\n");

	while (1)
	{
		if (write_to_server(sock) < 0) break;
		if (read_from_server(sock) < 0) break;
	}
	printf("The end.\n");

	close(sock);
	return 0;
}

int write_to_server(int fd)
{
	int	nbytes;
	char	buf[BUFLEN];

	fprintf(stdout, "Send to server > ");
	if (fgets(buf, BUFLEN, stdin) == nullptr) printf("error\n");
	buf[BUFLEN] = 0;
	nbytes = write(fd, buf, strlen(buf) + 1);
	if (nbytes < 0) return perror("write"), -1;
	if (strstr(buf, "stop")) return -1;
	return 0;
}

int read_from_server(int fd)
{
	int	nbytes;
	char	buf[BUFLEN];

	nbytes = read(fd, buf, BUFLEN);
	if (nbytes < 0)
		return perror("read"), -1;
	else if (nbytes == 0)
		fprintf(stderr, "Client: no message\n");
	else
		fprintf(stdout, "Server's replay: %s\n", buf);
	return 0;
}

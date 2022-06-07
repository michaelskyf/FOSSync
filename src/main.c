#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "args.h"
#include "msg.h"
#include "worker.h"

#define PORT 6969

int bind_socket(int *sockfd, struct sockaddr_in *srv_addr)
{
	*sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(*sockfd == -1)
	{
		fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	memset(srv_addr, 0, sizeof(*srv_addr));

	/* Assign ip and port */
	srv_addr->sin_family = AF_INET;
	srv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr->sin_port = htons(PORT);

	if(bind(*sockfd, (struct sockaddr *)srv_addr, sizeof(*srv_addr)) == -1)
	{
		fprintf(stderr, "Failed to bind socket: %s\n", strerror(errno));
		return -1;
	}

	if(listen(*sockfd, 5) == -1)
	{
		fprintf(stderr, "Failed to listen? (changeme): %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

int serve(void)
{
	int sockfd, connfd;
	struct sockaddr_in srv_addr, cli_addr;
	socklen_t cli_len;

	if(bind_socket(&sockfd, &srv_addr))
	{
		return -1;
	}

	while(1)
	{
		connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
		if(connfd == -1)
		{
			fprintf(stderr, "Failed to accept incoming connection: %s\n", strerror(errno));
			return -1;
		}

		worker_create(connfd);
	}

	close(sockfd);
	return 0;
}

int main(int argc, char *argv[])
{
	args_parse(argc, argv);
	return serve();
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <selog/selog.h>
#include <sys/types.h>

#include "args.h"

#ifndef FOSSYNC_VERSION
#define FOSSYNC_VERSION "UNKNOWN"
#endif

struct sockaddr_in srv_address;
int sockfd;

int bind_udp(void)
{
	struct sockaddr_in srv_address;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	memset(&srv_address, 0, sizeof(srv_address));

	srv_address.sin_family = AF_INET;
	srv_address.sin_addr.s_addr = inet_addr(args->address);
	srv_address.sin_port = htons(args->port);

	if(bind(sockfd, (struct sockaddr *)&srv_address, sizeof(srv_address)) < 0)
	{
		fprintf(stderr, "Failed to bind socket: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

/* Remember to align to 4 byte boundries! */
struct __attribute__((packed)) header
{
 	/* Magic: "I Love Anime!UwU" */
	unsigned char magic[16];
	uint32_t header_version; /* Current verion: 1 */
	uint32_t sender_id; /* 0 is user client, anything else is another server */
	uint32_t function;
	uint32_t args_size;
	/* args */
};


/* 0 on valid, non-zero on invalid header */
int read_header(const unsigned char *data, const size_t data_len)
{
	const struct header *header = (const struct header *)data;
	const unsigned char magic[16] = "I Love Anime!UwU";
	const uint32_t current_header_version = 1;

	/* Check if magic is valid */
	if(memcmp(magic, header->magic, sizeof(magic)))
	{
		fprintf(stderr, "Invalid magic\n");
		return -1;
	}

	/* Check if header version is supported */
	if(header->header_version > current_header_version)
	{
		fprintf(stderr, "Unsupported header version");
		return -1;
	}

	return 0;
}

void loop(void)
{

	ssize_t msg_len;
	socklen_t address_len;
	struct sockaddr_in client_addr;
	unsigned char msg_buffer[1024];

	while(1)
	{
		msg_len = recvfrom(sockfd, msg_buffer, sizeof(msg_buffer),
				MSG_WAITALL, (struct sockaddr *)&client_addr, &address_len);

		if(!read_header(msg_buffer, msg_len))
		{
			log_debug("Działa :p");
		}
	}
}

int main(int argc, char *argv[])
{
	selog_setup_default();

	log_info("Starting FOSSync server version %s...", FOSSYNC_VERSION);

	args_parse(argc, argv);

	log_info("Binding address %s:%d...", args->address, args->port);

	if(bind_udp())
	{
		log_error("Failed to bind socket. Exiting");
		exit(EXIT_FAILURE);
	}

	log_info("Sever started");

	loop();

	/* Clean-up */
	log_info("Stopping the server...");

	close(sockfd);

	log_info("Server stopped");
	return 0;
}

/*
    This file is part of FOSSync.

    FOSSync is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FOSSync is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FOSSync. If not, see <https://www.gnu.org/licenses/>.
*/

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

#include "types.h"
#include "args.h"
#include "worker.h"

#ifndef FOSSYNC_VERSION
	#define FOSSYNC_VERSION "UNKNOWN"
#endif

#define MAX_MSG_LEN	(long)(100 * 1024) /* 100KB */

struct sockaddr_in srv_address;
int sockfd;

int bind_udp(void)
{
	struct sockaddr_in srv_address;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		log_error("Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	memset(&srv_address, 0, sizeof(srv_address));

	srv_address.sin_family = AF_INET;
	srv_address.sin_addr.s_addr = inet_addr(args->address);
	srv_address.sin_port = htons(args->port);

	if(bind(sockfd, (struct sockaddr *)&srv_address, sizeof(srv_address)) < 0)
	{
		log_error("Failed to bind socket: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

/* Remember to align to 4 byte boundries! */
struct __attribute__((packed)) header
{
	unsigned char magic[16]; /* Magic: "I Love Anime!UwU" */
	uint32_t header_version; /* Current verion: 1 */
	uint32_t sender_id; /* 0 - user client, high half - server id, lower half - job_id (0 if no job) */
	uint32_t function_id;
	uint32_t args_count; /* Number of arguments */
	uint32_t args_size; /* Total size of all arguments */
	/* args */
};


/* 0 on valid, non-zero on invalid header */
int read_header(const unsigned char *data, const size_t data_len, struct msg_info *info)
{
	const struct header *header = (const struct header *)data;
	const unsigned char magic[16] = "I Love Anime!UwU";
	const uint32_t current_header_version = 1;

	/* Check if message is long enough for header */
	if(data_len < sizeof(header))
	{
		log_debug("Not enough space for header");
		return -1;
	}

	/* Check if magic is valid */
	if(memcmp(magic, header->magic, sizeof(magic)))
	{
		log_debug("Invalid magic\n");
		return -1;
	}

	/* Check if header version is supported */
	if(header->header_version > current_header_version)
	{
		log_debug("Unsupported header version");
		return -1;
	}

	/* Write information to msg_info struct */
	info->function_id = header->function_id;
	info->args_count = header->args_count;

	/* Higher 2 bytes */
	info->server_id = (header->sender_id & 0xffff0000) >> 16;
	/* Lower 2 bytes */
	info->job_id = header->sender_id & 0x0000ffff;


	if(info->server_id == 0)
	{
		if(info->job_id != 0)
		{
			log_debug("Invalid sender_id");
			return -1;
		}

		if(0/*TODO*/)
		{
			log_debug("Invalid function for user client");
			return -1;
		}
	}
	else if(0/*TODO*/)
	{
		log_debug("Invalid function for server client");
		return -1;
	}

	/* Header is valid */
	return 0;
}

void loop(void)
{
	ssize_t msg_len;
	socklen_t address_len;
	struct sockaddr_in client_addr;
	struct msg_info info;
	unsigned char *msg_buffer = malloc(MAX_MSG_LEN);

	while(1)
	{
		/* Check if length is <= MAX_MSG_LEN */
		msg_len = recvfrom(sockfd, msg_buffer, MAX_MSG_LEN,
				MSG_WAITALL | MSG_PEEK, (struct sockaddr *)&client_addr, &address_len);

		if(msg_len < 0)
		{
			log_error("Error reading socket: %s", strerror(errno));
			break;
		}

		if(msg_len > MAX_MSG_LEN)
		{
			log_debug("Message exceeded MAX_MSG_LEN (%ld Bytes)", MAX_MSG_LEN);
		}

		/* Write the message to buffer */
		msg_len = recvfrom(sockfd, msg_buffer, MAX_MSG_LEN,
				MSG_WAITALL, (struct sockaddr *)&client_addr, &address_len);

		if(msg_len < 0)
		{
			log_error("Error reading socket: %s", strerror(errno));
			break;
		}

		if(read_header(msg_buffer, msg_len, &info))
		{
			log_debug("Invalid packet detected");
		}

		/* Find/Create worker and give him the data */
	}

	free(msg_buffer);
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

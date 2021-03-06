#pragma once

#include <stdint.h>
#include <netinet/in.h>
#include "msg.h"

enum commands
{
	/* Ignored, always ACKed */
	COMMAND_NONE = 0,
	COMMAND_LIST_DIRS = 1,

	COMMAND_ACK = 128,
	COMMAND_NAK = 129,
};

void *command_get_function(uint8_t opcode);

void command_none(int connfd, const struct msg_header * msg);
void command_list_dirs(int connfd, const struct msg_header * msg);

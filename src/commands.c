#include <stddef.h>
#include <stdio.h>

#include "commands.h"

void *command_get_function(uint8_t opcode)
{
	switch (opcode)
	{
		case COMMAND_NONE:
			return NULL;

		case COMMAND_LIST_DIRS:
			return NULL;

		case COMMAND_ACK:
			return NULL;

		case COMMAND_NAK:
			return NULL;


		default:
		return NULL;
	}

}

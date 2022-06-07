#include "msg.h"
#include "commands.h"
#include <stddef.h>
#include <string.h>

int msg_check_integrity(size_t msg_size, const struct msg_header *msg)
{
	if(msg_size < sizeof(struct msg_header) || msg_size != sizeof(msg) + msg->size)
	{
		return -1;
	}

	if(!command_get_function(msg->opcode))
	{
		return -1;
	}



	return 0;
}

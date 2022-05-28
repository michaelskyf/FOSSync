#pragma once

#include <stdbool.h>

extern struct args
{
	const char *exe_name;
	const char *address;
	const unsigned int port;
}
* args;

void args_parse(int argc, char **argv);

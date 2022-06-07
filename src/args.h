#pragma once

#include <stdbool.h>
#include <stdint.h>

extern struct args
{
	const char *exe_name;
	const char *address;
	const uint32_t port;
}
* args;

void args_parse(int argc, char **argv);

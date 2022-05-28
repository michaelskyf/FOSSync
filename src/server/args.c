#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "args.h"

static void parse(int agc, char **argv);
static void init(char **argv);

static void print_help(void);

struct
{
	const char *exe_name;
	const char *address;
	unsigned int port;
}
__args;
struct args *args = (struct args *)&__args;

static void parse(int argc, char **argv)
{
	int option;
	int option_index = 0;
	int shouldParse = 1;

	/* https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html */
	struct option long_options[] =
	{
		{"help", no_argument, 0, 'h'},
		{"address", required_argument, 0, 'a'},
		{"p", required_argument, 0, 'p'},
		{0, 0, 0, 0},
	};

	while(shouldParse)
	{
		option = getopt_long(argc, argv, "ha:p:", long_options, &option_index);

		switch(option)
		{
			case 0:
			break;

			case 'h':
				print_help();
				exit(EXIT_SUCCESS);
			break;

			case 'a':
				__args.address = optarg;
			break;

			case 'p':
				__args.port = atoi(optarg);
			break;

			case '?':
				printf("Try '%s --help' for more information.\n", argv[0]);
				exit(EXIT_FAILURE);

			default:
				shouldParse = 0;
			break;
		}
	}
}

static void init(char **argv)
{
	/* Executable name */
	__args.exe_name = argv[0];

	/* If ip_address is not set, use localhost */
	if(args->address == NULL)
	{
		args->address = "127.0.0.1";
	}

	/* If port is not set, use default one */
	if(args->port == 0)
	{
		__args.port = 6969;
	}
}

void args_parse(int argc, char **argv)
{
	/* zero all arguments */
	memset(&__args, 0, sizeof(struct args));

	parse(argc, argv);
	init(argv);
}

static void print_help()
{
	printf(
		"This is a help message\n"
		"In the future it should be replaced\n"
		"With something more helpful\n"
	);
}

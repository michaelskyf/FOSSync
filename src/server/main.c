#include <stdio.h>
#include <selog/selog.h>

#ifndef FOSSYNC_VERSION
#define FOSSYNC_VERSION "UNKNOWN"
#endif

int main(int argc, char *argv[])
{
	selog_setup_default();

	log_info("Starting FOSSync server version %s", FOSSYNC_VERSION);

	return 0;
}

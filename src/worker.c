#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "worker.h"

void *worker_func(void *args)
{
	int connfd = (int)args;
	char buffer[1024];
	ssize_t read_bytes;

	memset(buffer, 0, sizeof(buffer));

	if((read_bytes = read(connfd, buffer, sizeof(buffer))) == -1)
	{
		fprintf(stderr, "Failed to read socket: %s\n", strerror(errno));
		pthread_exit(NULL);
	}

	while(1)
	{
		printf("%s\n", buffer);
		sleep(1);
	}

	pthread_exit(NULL);
}

int worker_create(int connfd)
{
	pthread_t thread_id;
	int ret = pthread_create(&thread_id, NULL, worker_func, (void*)connfd);
	if(ret)
	{
		fprintf(stderr, "Failed to create worker thread: %s", strerror(ret));
		return -1;
	}

	return 0;
}

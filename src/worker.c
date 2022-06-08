#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "msg.h"
#include "worker.h"
#include "list.h"
#include "types.h"

#define WORKER_EXIT(ret) \
	w->status = 0; \
	w->exit_code = ret; \
	close(w->connfd); \
	pthread_exit(NULL)

static struct list_head workers_head =
{
	.prev = &workers_head, .next = &workers_head
};

struct worker
{
	struct list_head workers;

	pthread_t thread_id;
	int connfd;
	int exit_code;
	int status; /* 0 - exited, 1 - working */
};

static void *worker_func(struct worker *w)
{
	ssize_t read_bytes;
	char buffer[sizeof(struct msg_header) + MAX_DATA_SIZE];

	if((read_bytes = read(w->connfd, buffer, sizeof(buffer))) == -1)
	{
		fprintf(stderr, "Failed to read socket: %s\n", strerror(errno));
		WORKER_EXIT(-1);
	}

	WORKER_EXIT(0);
}

void worker_cleanup(int blocking)
{
	struct worker *w;
	struct list_head *curr = workers_head.next, *next = curr->next;
	while(!list_is_head(curr, &workers_head))
	{
		w = list_entry(curr, typeof(*w), workers);
		if(blocking || w->status == 0)
		{
			pthread_join(w->thread_id, NULL);

			printf("Thread %ld exited with return value %d\n", w->thread_id, w->exit_code);

			list_del(&w->workers);
			free(w);
		}

		curr = next;
		next = next->next;
	}
}

int worker_create(int connfd)
{
	worker_cleanup(0);

	struct worker *w = malloc(sizeof(struct worker));
	if(!w)
	{
		fprintf(stderr, "Failed to malloc struct worker: %s", strerror(errno));
		return -1;
	}

	list_add(&w->workers, &workers_head);

	/* Init struct worker here */
	w->connfd = connfd;
	w->status = 1;

	int ret = pthread_create(&w->thread_id, NULL, (void *(*)(void*))worker_func, w);
	if(ret)
	{
		fprintf(stderr, "Failed to create worker thread: %s\n", strerror(ret));
		return -1;
	}

	return 0;
}

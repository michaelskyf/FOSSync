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

#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#include <selog/selog.h>

#include "container_of.h"
#include "types.h"
#include "worker.h"
#include "list.h"
#include "functions.h"

#define arg(index, type)	(type)get_argument(index, w->fn_info)

struct worker init_worker =
{
	.workers = {.prev = &init_worker.workers, .next = &init_worker.workers},
};

static inline void *get_argument(size_t index, struct function_info *info)
{
	struct arg_head *arg = (void*)info + sizeof(info);
	for(size_t i = 0; i < index; i++)
	{
		arg = (void*)arg + arg->arg_size;
	}

	return (void*)arg + sizeof(arg);
}

int worker_run_function(struct worker *w)
{
	int ret;

	switch(w->fn_info->function_id)
	{
		case FUNCTION_HELLOWORLD:
			ret = helloworld(arg(0, const char *));

			break;

		default:
			log_error("Function with id %u not found", w->fn_info->function_id);
			ret = -1;
	}

	return ret;
}

static inline void worker_cleanup(struct worker *w)
{
	pthread_mutex_lock(&w->worker_lock);

	/* Free function_info and its arguments */
	free(w->fn_info);

	/* Free remaining data_entries */
	struct list_head *h = w->data_queue_head.next;
	while(!list_is_head(h, &w->data_queue_head))
	{
		struct list_head *next = h->next;

		struct data_queue_entry *e = list_entry(h, typeof(*e), data_entries);

		free(e);

		h = next;
	}

	pthread_mutex_unlock(&w->worker_lock);
}

void *worker_function(void *args)
{
	struct worker *w = (struct worker *)args;
	int ret = 0;

	ret = worker_run_function(w);

	w->exit_status = ret;

	worker_cleanup(w);
	pthread_exit(NULL);
}

static struct worker *worker_init(struct function_info *fn_info)
{
	struct worker *w = malloc(sizeof(struct worker));
	if(w)
	{
		list_add_tail(&w->workers, &init_worker.workers);

		/* Init queue */
		w->data_queue_head.next = &w->data_queue_head;
		w->data_queue_head.prev = &w->data_queue_head;

		w->fn_info = fn_info;
	}
	else
	{
		log_error("Failed to create worker: %s", strerror(errno));
	}
	return w;
}

int job_create(struct function_info *fn_info)
{
	struct worker *w = worker_init(fn_info);
	if(!w)
	{
		return -1;
	}

	int ret = pthread_create(&w->thread_id, NULL, worker_function, w);
	if(ret)
	{
		/* TODO: print error string */
		free(w);
		return -1;
	}

	return ret;
}

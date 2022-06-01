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

#pragma once

#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

#include "types.h"

/*
   	 ------------------
	| args_info | data |
	 ------------------
*/
struct __attribute__((packed)) arg_info
{
	uint32_t arg_id; /* Argument id (eg. strncmp(0, 1, 2)) */
	uint32_t part_id; /* Current part id */
	uint32_t total_pats; /* Total number of parts */
	uint32_t data_size; /* Size of current data */
	uint64_t total_size; /* Total size of argument (size of all data parts) */
};

struct msg_info
{
	uint16_t server_id;
	uint16_t job_id;
	uint32_t function_id;
	uint32_t args_count;
};

struct arg_head
{
	size_t arg_size;
	size_t arg_id; /* First argument is 0, second is 1, ... */
	/* arg */
};

struct function_info
{
	id_t function_id;
	size_t args_count;
	size_t args_size;
	/* args */
};

struct data_queue_entry
{
	struct list_head data_entries;
	size_t data_size;
	struct tm *timestamp;
	/* data */
};

struct worker
{
	struct list_head workers;
	id_t job_id;
	pthread_t thread_id;

	struct function_info *fn_info;

	int exit_status;

	pthread_mutex_t worker_lock; /* Everything under is protected by this lock */

	struct list_head data_queue_head;
};

int job_create(struct function_info *fn_info);

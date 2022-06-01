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

#include <stdint.h>

#ifndef FOSSYNC_VERSION
	#define FOSSYNC_VERSION "UNKNOWN"
#endif

#define MAX_MSG_LEN	(long)(100 * 1024) /* 100KB */

struct __attribute__((packed)) header
{
	unsigned char magic[16]; /* Magic: "I Love Anime!UwU" */
	uint32_t header_version; /* Current verion: 1 */
	uint32_t sender_id; /* 0 - user client, high - server id, low - job_id (0 if no job) */
	uint32_t function_id;
	uint32_t args_count; /* Number of arguments */
	uint32_t args_size; /* Total size of all arguments */
	/* args */
};

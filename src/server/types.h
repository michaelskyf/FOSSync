#pragma once

#include <sys/types.h>

struct list_head
{
	struct list_head *prev, *next;
};

#pragma once

#include "compiler.h"

#define typeof_member(T, m)	typeof(((T*)0)->m)

#define container_of(ptr, type, member)						\
	__extension__								\
	 ({									\
		void *__mptr = (void *)(ptr);					\
		static_assert(__same_type(*(ptr), ((type *)0)->member) ||	\
			      __same_type(*(ptr), void),			\
			      "pointer type mismatch in container_of()");	\
			      ((type *)(__mptr - offsetof(type, member)));	\
	 })

#include <gtest/gtest.h>

extern "C"
{
#include "../worker.h"
}

TEST(wokrer, test_creating_and_exiting)
{
	int ret;

	for (int i = 0; i < 1000; i++)
	{
		ret = worker_create(-1);
		EXPECT_EQ(ret, 0);
	}

	worker_cleanup(1);
}

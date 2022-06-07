#include <gtest/gtest.h>

TEST(sneed, kindly_do_the_needful)
{
	EXPECT_EQ(2, 2);
	int *mem = (int*)malloc(12);
	*mem = 1;
//	free(mem);
	mem = NULL;
}

TEST(sneed, kindly_do_the_needful2)
{
	EXPECT_EQ(2, 2);
	int *mem = (int*)malloc(12);
	*mem = 1;
	free(mem);
	mem = NULL;
}

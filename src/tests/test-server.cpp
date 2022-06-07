#include <cstring>
#include <gtest/gtest.h>

extern "C"
{
#include "../packet.h"
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

struct test_data
{
	struct packet_header ph;
	struct arg_header h0;
	const char d0[128];
	struct arg_header h1;
	const char d1[128];
};

class test_packet : public testing::Test
{
	protected:
		void SetUp() override
		{

			packet_size = sizeof(test_data);
			packet = (struct test_data *)malloc(packet_size);

			memcpy(&packet->ph.magic, "I Love Anime!UwU", sizeof(packet->ph.magic));

			packet->ph.packet_size = packet_size;
			packet->ph.sender_id = 0;
			packet->ph.job_id = 0;
			packet->ph.command_id = 0;
			packet->ph.args_count = 1;
			packet->ph.data_size = packet_size - sizeof(struct packet_header);
			packet->ph.total_data_size = packet->ph.data_size;
		}

		void TearDown() override
		{
			free(packet);
		}

	public:
		struct test_data *packet;
		size_t packet_size;
};

TEST_F(test_packet, HeadIntegrity)
{
	EXPECT_EQ(0, packet_check_integrity(packet_size, (struct packet_header *)packet));
}

TEST_F(test_packet, DataIntegrity)
{
	EXPECT_EQ(0, packet_check_data_integrity((struct packet_header *)packet));
}

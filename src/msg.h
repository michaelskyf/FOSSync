#pragma once

#include <sys/types.h>
#include <stdint.h>

#define MAX_DATA_SIZE	((uint8_t)-1) /* 255 */


struct __attribute__((packed)) msg_header
{
	uint16_t seq;
	uint8_t session;
	uint8_t opcode;
	uint8_t size;
	uint8_t req_opcode;
	uint64_t offset;
	uint8_t data[];
};

/**
 * @brief Check integrity of received msg
 *
 * @param[in]		msg_size		Size of the message returned by recvfrom()
 * @param[in]		msg			Pointer to message
 *
 * @returns		0 on valid message, non-zero on failure
 */
__attribute__((warn_unused_result))
int msg_check_integrity(size_t msg_size, const struct msg_header *msg);

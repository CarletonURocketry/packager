/**
 * @file test_header_helpers.c
 * @brief Tests the helper functions for modifying/reading header structures.
 */
#include "../src/include/packet_types.h"
#include <string.h>

// Define appropriate variables for tracking test statistics within macros
#define TOTAL_COUNT total
#define FAIL_COUNT failed
// Include harness
#include "harness.h"

/**
 * Test that the helper function for getting the entire packet length (including the header) of a packet works
 * correctly.
 */
bool test_packet_header_get_length() {

    PacketHeader h;
    uint16_t packet_payload_len = 4;
    packet_header_init(&h, "VA3INI", packet_payload_len, 1, ROCKET, 12);

    LOG_ASSERT(packet_header_get_length(&h) == packet_payload_len + sizeof(PacketHeader));

    return true;
}

/**
 * Test that the helper function for setting the packet payload length works correctly when a different payload length
 * is set.
 */
bool test_packet_header_set_length() {

    PacketHeader h;
    packet_header_init(&h, "VA3INI", 8, 1, ROCKET, 12);

    LOG_ASSERT(packet_header_get_length(&h) == 8 + sizeof(PacketHeader));

    uint16_t packet_payload_len = 4;
    packet_header_set_length(&h, packet_payload_len);

    LOG_ASSERT(packet_header_get_length(&h) == packet_payload_len + sizeof(PacketHeader));

    return true;
}

/**
 * Test that the helper function for incrementing the packet length works correctly.
 */
bool test_packet_header_inc_length() {

    PacketHeader h;
    uint16_t packet_payload_len = 4;
    packet_header_init(&h, "VA3INI", packet_payload_len, 1, ROCKET, 12);

    LOG_ASSERT(packet_header_get_length(&h) == packet_payload_len + sizeof(PacketHeader));

    uint16_t added_amount = 8;
    packet_header_inc_length(&h, added_amount);

    LOG_ASSERT(packet_header_get_length(&h) == packet_payload_len + added_amount + sizeof(PacketHeader));

    return true;
}

/**
 * Test that the helper function for getting the block length from the block header works correctly.
 */
bool test_block_header_get_length() {

    BlockHeader h;
    uint16_t block_len = 12;
    block_header_init(&h, block_len, TYPE_DATA, DATA_TEMP, GROUNDSTATION);

    LOG_ASSERT(block_header_get_length(&h) == block_len + sizeof(BlockHeader));

    return true;
}

/**
 * Test that the helper function for setting the block length to a new payload size works correctly.
 */
bool test_block_header_set_length() {

    BlockHeader h;
    uint16_t block_len = 12;
    block_header_init(&h, block_len, TYPE_DATA, DATA_TEMP, GROUNDSTATION);

    LOG_ASSERT(block_header_get_length(&h) == block_len + sizeof(BlockHeader));

    block_len = 4;
    block_header_set_length(&h, block_len);
    LOG_ASSERT(block_header_get_length(&h) == block_len + sizeof(BlockHeader));

    return true;
}

int main(void) {

    // Track test statistics
    size_t total = 0;
    size_t failed = 0;

    RUN_TEST(test_packet_header_get_length);
    RUN_TEST(test_packet_header_set_length);
    RUN_TEST(test_packet_header_inc_length);
    RUN_TEST(test_block_header_get_length);
    RUN_TEST(test_block_header_set_length);

    HARNESS_RESULTS();

    return EXIT_SUCCESS;
}

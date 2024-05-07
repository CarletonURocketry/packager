/**
 * @file test_init.c
 * @brief Tests the init functions for packet encoding.
 */
#include "../src/include/packet_types.h"
#include "harness.h"
#include <string.h>

/**
 * Test that a packet header is initialized properly from provided parameters.
 */
bool test_packet_header_init(void) {

    PacketHeader p;
    packet_header_init(&p, "VA3INI", 32, 1, ROCKET, 12);

    LOG_ASSERT(packet_header_get_length(&p) == 32 + sizeof(p));
    LOG_ASSERT(p.version == 1);
    LOG_ASSERT(p.src_addr == ROCKET);
    LOG_ASSERT(p.packet_num == 12);
    LOG_ASSERT(!strcmp("VA3INI", p.call_sign));

    return true;
}

/**
 * Test that block headers are properly initialized from the provided parameters.
 */
bool test_block_header_init(void) {

    BlockHeader b;
    block_header_init(&b, 16, TYPE_DATA, DATA_ALT, GROUNDSTATION);

    LOG_ASSERT(block_header_get_length(&b) == 16 + sizeof(b));
    LOG_ASSERT(b.type == TYPE_DATA);
    LOG_ASSERT(b.subtype == DATA_ALT);
    LOG_ASSERT(b.dest_addr == GROUNDSTATION);
    return true;
}

/**
 * Test that an altitude block can be properly initialized using parameters.
 */
bool test_altitude_block_init(void) {
    AltitudeDB b;
    altitude_db_init(&b, 156, 12);
    // TODO: finish after struct conversion
    return true;
}

int main(void) {
    RUN_TEST(test_packet_header_init);
    RUN_TEST(test_block_header_init);
    return true;
}

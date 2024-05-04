/**
 * @file test_init.c
 * @brief Tests the init functions for packet encoding.
 */
#include "../src/include/packet_types.h"
#include <assert.h>
#include <string.h>

/**
 * Test that a packet header is initialized properly from provided parameters.
 */
void test_packet_header_init(void) {

    PacketHeader p;
    packet_header_init(&p, "VA3INI", 32, 1, ROCKET, 12);

    assert(packet_header_get_length(&p) == 32 + sizeof(p));
    assert(p.version == 1);
    assert(p.src_addr == ROCKET);
    assert(p.packet_num == 12);
    assert(!strcmp("VA3INI", p.call_sign));
}

/**
 * Test that block headers are properly initialized from the provided parameters.
 */
void test_block_header_init(void) {

    BlockHeader b;
    block_header_init(&b, 16, TYPE_DATA, DATA_ALT, GROUNDSTATION);

    assert(block_header_get_length(&b) == 16 + sizeof(b));
    assert(b.type == TYPE_DATA);
    assert(b.subtype == DATA_ALT);
    assert(b.dest_addr == GROUNDSTATION);
}

/**
 * Test that an altitude block can be properly initialized using parameters.
 */
void test_altitude_block_init(void) {
    AltitudeDB b;
    altitude_db_init(&b, 156, 12);
    // TODO: finish after struct conversion
}

int main(void) {
    test_packet_header_init();
    test_block_header_init();
}

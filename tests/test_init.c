/**
 * @file test_init.c
 * @brief Tests the init functions for packet encoding.
 */
#include "../src/packet_types.h"
#include <string.h>

// Define appropriate variables for tracking test statistics within macros
#define TOTAL_COUNT total
#define FAIL_COUNT failed
// Include harness
#include "harness.h"

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
    block_header_init(&b, 16, TYPE_DATA, DATA_ALT_SEA, GROUNDSTATION);

    LOG_ASSERT(block_header_get_length(&b) == 16 + sizeof(b));
    LOG_ASSERT(b.type == TYPE_DATA);
    LOG_ASSERT(b.subtype == DATA_ALT_SEA);
    LOG_ASSERT(b.dest_addr == GROUNDSTATION);
    return true;
}

/**
 * Test that an altitude block can be properly initialized using parameters.
 */
bool test_altitude_block_init(void) {

    AltitudeDB b;
    altitude_db_init(&b, 156, 12);

    LOG_ASSERT(b.mission_time == 156);
    LOG_ASSERT(b.altitude == 12);

    altitude_db_init(&b, 156, -12);
    LOG_ASSERT(b.altitude == -12);

    return true;
}

/** Test that a temperature data block can be properly initialized using parameters. */
bool test_temperature_block_init(void) {

    TemperatureDB b;
    temperature_db_init(&b, 1500, -2000);

    LOG_ASSERT(b.mission_time == 1500);
    LOG_ASSERT(b.temperature == -2000);

    temperature_db_init(&b, 1500, 3000);
    LOG_ASSERT(b.temperature == 3000);

    return true;
}

/** Test that a pressure data block can be properly initialized using parameters. */
bool test_pressure_block_init(void) {

    PressureDB b;
    pressure_db_init(&b, 1001, 55);

    LOG_ASSERT(b.mission_time == 1001);
    LOG_ASSERT(b.pressure == 55);

    return true;
}

/** Test that a humidity data block can be properly initialized using parameters. */
bool test_humidity_block_init(void) {

    HumidityDB b;
    humidity_db_init(&b, 0, 18);
    LOG_ASSERT(b.mission_time == 0);
    LOG_ASSERT(b.humidity == 18);

    humidity_db_init(&b, 0, 10000);
    LOG_ASSERT(b.humidity == 10000);

    return true;
}

/** Test that an angular velocity data block can be initialized using parameters. */
bool test_angular_velocity_block_init(void) {

    AngularVelocityDB b;
    angular_velocity_db_init(&b, 1, -1, 2, -3);

    LOG_ASSERT(b.mission_time == 1);
    LOG_ASSERT(b.x == -1);
    LOG_ASSERT(b.y == 2);
    LOG_ASSERT(b.z == -3);
    LOG_ASSERT(b._padding == 0);

    return true;
}

/** Test that an acceleration data block can be initialized using parameters. */
bool test_acceleration_block_init(void) {

    AccelerationDB b;
    acceleration_db_init(&b, 1, -1, 2, -3);

    LOG_ASSERT(b.mission_time == 1);
    LOG_ASSERT(b.x == -1);
    LOG_ASSERT(b.y == 2);
    LOG_ASSERT(b.z == -3);
    LOG_ASSERT(b._padding == 0);

    return true;
}

/** Test that a coordinate data block can be initialized using parameters. */
bool test_coordinate_block_init(void) {

    CoordinateDB b;
    coordinate_db_init(&b, 1200, 2, 1500);

    LOG_ASSERT(b.mission_time == 1200);
    LOG_ASSERT(b.latitude == 2);
    LOG_ASSERT(b.longitude == 1500);

    coordinate_db_init(&b, 1, -24, -3300);
    LOG_ASSERT(b.mission_time == 1);
    LOG_ASSERT(b.latitude == -24);
    LOG_ASSERT(b.longitude == -3300);

    return true;
}

/** Test that a voltage data block can be initialized using parameters. */
bool test_voltage_block_init(void) {

    VoltageDB b;
    voltage_db_init(&b, 1200, 2, 1500);

    LOG_ASSERT(b.mission_time == 1200);
    LOG_ASSERT(b.id == 2);
    LOG_ASSERT(b.voltage == 1500);

    voltage_db_init(&b, 1, 24, -3300);
    LOG_ASSERT(b.mission_time == 1);
    LOG_ASSERT(b.id == 24);
    LOG_ASSERT(b.voltage == -3300);

    return true;
}

int main(void) {

    // Track test statistics
    size_t total = 0;
    size_t failed = 0;

    RUN_TEST(test_packet_header_init);
    RUN_TEST(test_block_header_init);
    RUN_TEST(test_altitude_block_init);
    RUN_TEST(test_temperature_block_init);
    RUN_TEST(test_pressure_block_init);
    RUN_TEST(test_humidity_block_init);
    RUN_TEST(test_angular_velocity_block_init);
    RUN_TEST(test_acceleration_block_init);
    RUN_TEST(test_voltage_block_init);
    RUN_TEST(test_coordinate_block_init);

    HARNESS_RESULTS();

    return EXIT_SUCCESS;
}

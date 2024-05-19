#include "intypes.h"
#include "packet_types.h"
#include <errno.h>
#include <getopt.h>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define dref_cast(dtype, ptr) (*((dtype *)(ptr)))

/** The size of the buffer for reading sensor data input. */
#define BUFFER_SIZE 150
/** The maximum number of blocks that can be added to a packet before it is sent. */
#define BLOCK_LIMIT 4
/** The version of the packet encoding being used. */
#define VERSION 1
/** The name of the message queue for outputting encoded packets. */
#define OUTPUT_QUEUE "packager-out"
/** The name of the message queue for input sensor data. */
#define INPUT_QUEUE "fetcher/sensors"

/** Static variable to store the user HAM radio call sign. */
static char *callsign = NULL;
/** Static variable to store the file name to read input from instead of stdin. */
static char *infile = NULL;
/** Whether or not to print the encoded packets to stdout (false by default). */
static bool print_output = false;
/** Static buffer for reading input. */
static uint8_t buffer[BUFFER_SIZE] = {0};

/* --- CONSTRUCTING PACKETS --- */

/** Packet count tracker for encoding packets number. */
static uint16_t pkt_count = 0;

/** A buffer for constructing the current packet. */
static uint8_t packet[PACKET_MAX_SIZE];

/** The current position within the packet buffer. */
static uint8_t *packet_pos = packet;

void add_block_header(DataBlockType t, size_t size);
bool room_for_block(size_t b_len);

int main(int argc, char **argv) {

    /* Fetch command line arguments. */
    int c;
    while ((c = getopt(argc, argv, ":i:p")) != -1) {
        switch (c) {
        case 'i':
            infile = optarg;
            break;
        case 'p':
            print_output = true;
            break;
        case ':':
            fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            exit(EXIT_FAILURE);
        case '?':
            fprintf(stderr, "Unkown option -%c.\n", optopt);
            exit(EXIT_FAILURE);
        default:
            fputs("Please see the 'use' page for usage.", stderr);
            return EXIT_FAILURE;
        }
    }

    /* Check for positional arguments. */
    if (optind >= argc) {
        fprintf(stderr, "Call sign is required.\n");
        exit(EXIT_FAILURE);
    }
    callsign = argv[optind];

    /* Open input stream. */
    FILE *input = stdin;
    if (infile != NULL) {
        input = fopen(infile, "r");
        if (input == NULL) {
            fprintf(stderr, "File '%s' could not be opened for reading.\n", infile);
            exit(EXIT_FAILURE);
        }
    }

    /* Open input message queue. */
    mqd_t in_q = mq_open(INPUT_QUEUE, O_RDONLY);
    if (in_q == -1) {
        fprintf(stderr, "Could not open input message queue %s with error %s\n", INPUT_QUEUE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Open output message queue. */
    struct mq_attr q_attributes = {
        .mq_flags = 0,
        .mq_maxmsg = 15, // 15 packets is probably enough
        .mq_msgsize = PACKET_MAX_SIZE,
        .mq_curmsgs = 0,
        .mq_recvwait = 1,
        .mq_sendwait = 1,
    };
    mqd_t out_q = mq_open(OUTPUT_QUEUE, O_CREAT | O_WRONLY, S_IWOTH, &q_attributes);
    if (out_q == -1) {
        fprintf(stderr, "Could not open output queue %s with error %s\n", OUTPUT_QUEUE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    uint32_t last_time = 0;
    size_t just_added_block_size = 0;
    void *data = &buffer[1];
    while (1) {
        packet_header_init((PacketHeader *)packet, callsign, 0, VERSION, ROCKET, pkt_count);
        packet_pos += sizeof(PacketHeader); // We just added a packet header

        // TODO: need a better condition for ending packet construction that still maximizes buffer use
        // WARNING: Assumes AngularVelocityDB as largest possible block size
        while (room_for_block(sizeof(AngularVelocityDB))) {

            /* Read input data. */
            if (mq_receive(in_q, (char *)buffer, sizeof(buffer), NULL) == -1) {
                fprintf(stderr, "Could not read message from queue %s with error %s\n", INPUT_QUEUE, strerror(errno));
                continue;
            }

            just_added_block_size = 0;
            switch (buffer[0]) {
            case TAG_TIME:
                // Update with most recent time measurement to use as measurement time for other packets
                last_time = *(uint32_t *)(data);
                break;

            case TAG_TEMPERATURE:
                just_added_block_size = sizeof(TemperatureDB);
                add_block_header(DATA_TEMP, just_added_block_size);
                temperature_db_init((TemperatureDB *)packet_pos, last_time, 1000 * dref_cast(float, data));
                break;

            case TAG_PRESSURE:
                just_added_block_size = sizeof(PressureDB);
                add_block_header(DATA_PRESSURE, just_added_block_size);
                pressure_db_init((PressureDB *)packet_pos, last_time, 1000 * dref_cast(float, data));
                break;

            case TAG_HUMIDITY:
                just_added_block_size = sizeof(HumidityDB);
                add_block_header(DATA_HUMIDITY, just_added_block_size);
                humidity_db_init((HumidityDB *)packet_pos, last_time, 100 * dref_cast(float, data));
                break;

            case TAG_ALTITUDE_REL:
            case TAG_ALTITUDE_SEA:
                just_added_block_size = sizeof(AltitudeDB);
                add_block_header(DATA_ALT, just_added_block_size);
                altitude_db_init((AltitudeDB *)packet_pos, last_time, 1000 * dref_cast(float, data));
                break;

            case TAG_LINEAR_ACCEL_ABS:
            case TAG_LINEAR_ACCEL_REL:
                just_added_block_size = sizeof(AccelerationDB);
                add_block_header(DATA_ACCEL, just_added_block_size);
                acceleration_db_init((AccelerationDB *)packet_pos, last_time, dref_cast(vec3d_t, data).x * 100,
                                     dref_cast(vec3d_t, data).y * 100, dref_cast(vec3d_t, data).z * 100);
                break;

            case TAG_ANGULAR_VEL:
                just_added_block_size = sizeof(AngularVelocityDB);
                add_block_header(DATA_ANGULAR_VEL, just_added_block_size);
                angular_velocity_db_init((AngularVelocityDB *)packet_pos, last_time, dref_cast(vec3d_t, data).x * 10,
                                         dref_cast(vec3d_t, data).y * 10, dref_cast(vec3d_t, data).z * 10);
                break;

            default:
                fprintf(stderr, "Unknown input data type: %u\n", buffer[0]);
                continue; // Skip to next iteration without storing block
            }

            // Increment position in packet buffer to match most recently added block type
            packet_pos += just_added_block_size;
            packet_header_inc_length((PacketHeader *)packet, just_added_block_size);
        }

        pkt_count++; // One more packet constructed

        if (mq_send(out_q, (char *)packet, packet_header_get_length((PacketHeader *)packet), 0) == -1) {
            fprintf(stderr, "Failed to output encoded packet #%u with error: %s\n", pkt_count - 1, strerror(errno));
        }

        if (print_output) packet_print_hex(stdout, packet);

        packet_pos = packet; // Reset position to overwrite with next packet
    }

    return EXIT_SUCCESS;
}

/**
 * Adds a block header to the global packet buffer.
 * @param t The type of the data block.
 * @param size The size of the data block (in bytes) that will be stored in this block, not including its header.
 */
void add_block_header(DataBlockType t, size_t size) {
    block_header_init((BlockHeader *)packet_pos, size, TYPE_DATA, t, GROUNDSTATION);
    packet_pos += sizeof(BlockHeader); // We just added a block header

    // Update packet header length to include just added block header
    packet_header_inc_length((PacketHeader *)packet, sizeof(BlockHeader));
}

/**
 * Checks if there is room for a block in the global packet buffer.
 * @param b_len The size of the block (not including its header) to append to the packet buffer.
 * @return True if the append succeeded, false if there was no space to append the block.
 */
bool room_for_block(size_t b_len) {
    const uint16_t p_len = packet_header_get_length((PacketHeader *)packet);

    // Ensure that there is enough space for the block to be added to the packet
    if (p_len + b_len + sizeof(BlockHeader) > PACKET_MAX_SIZE) return false;
    return true;
}

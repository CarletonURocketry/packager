#include "packet_types.h"
#include <errno.h>
#include <getopt.h>
#include <mqueue.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Represents the different sensor data types that can be interpreted by packager. */
typedef enum {
    DTYPE_TEMPERATURE = 0, /**< Temperature */
    DTYPE_TIME = 1,        /**< Time */
    DTYPE_PRESSURE = 2,    /**< Pressure */
    DTYPE_HUMIDITY = 3,    /**< Humidity */
    DTYPE_ALTITUDE = 4,    /**< Altitude */
    DTYPE_DNE = 5,         /**< Data type does not exist */
} Dtype;

/** String representation of the possible data types. */
const char *DTYPES[] = {
    [DTYPE_TEMPERATURE] = "Temperature", [DTYPE_TIME] = "Time",         [DTYPE_PRESSURE] = "Pressure", [DTYPE_DNE] = "",
    [DTYPE_ALTITUDE] = "Altitude",       [DTYPE_HUMIDITY] = "Humidity",
};

/** The size of the buffer for reading sensor data input. */
#define BUFFER_SIZE 150
/** The maximum number of blocks that can be added to a packet before it is sent. */
#define BLOCK_LIMIT 4
/** The version of the packet encoding being used. */
#define VERSION 1
/** The name of the message queue for outputting encoded packets. */
#define OUTPUT_QUEUE "packager-out"

/** Static variable to store the user HAM radio call sign. */
static char *callsign = NULL;
/** Static variable to store the file name to read input from instead of stdin. */
static char *infile = NULL;
/** Whether or not to print the encoded packets to stdout (false by default). */
static bool print_output = false;
/** Static buffer for reading input. */
static char buffer[BUFFER_SIZE] = {0};

/* --- CONSTRUCTING PACKETS --- */

/** Packet count tracker for encoding packets number. */
static uint16_t pkt_count = 0;

/** A buffer for constructing the current packet. */
static uint8_t packet[PACKET_MAX_SIZE];

/** The current position within the packet buffer. */
static uint8_t *packet_pos = packet;

void add_block_header(DataBlockType t, size_t size);
Dtype dtype_from_str(const char *str);
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

    /** Open message queue. */
    mqd_t out_q = mq_open(OUTPUT_QUEUE, O_CREAT | O_WRONLY, S_IWOTH, NULL);
    if (out_q == -1) {
        fprintf(stderr, "Could not open output queue %s with error %s\n", OUTPUT_QUEUE, strerror(errno));
        exit(EXIT_FAILURE);
    }

    bool no_input = false;
    uint32_t last_time = 0;
    while (!no_input) {
        packet_header_init((PacketHeader *)packet, callsign, 0, VERSION, ROCKET, pkt_count);
        packet_pos += sizeof(PacketHeader); // We just added a packet header

        // TODO: need a better condition for ending packet construction that still maximizes buffer use
        // WARNING: Currently assumes largest possible block is AngularVelocityDB
        while (room_for_block(sizeof(AngularVelocityDB))) {

            /* Read input data. WARNING: No error handling for when text read is longer than buffer. */
            if (fgets(buffer, BUFFER_SIZE, input) == NULL) {
                no_input = true; // No more data
                break;           // Send the partially constructed packet before exiting
            }

            // Decide what contents to add to the block
            char *dtype_str = strtok(buffer, ":");
            Dtype dtype = dtype_from_str(dtype_str);
            size_t just_added_block_size;

            switch (dtype) {
            case DTYPE_TIME:
                // Update with most recent time measurement to use as measurement time for other packets
                last_time = strtoul(strtok(NULL, ":"), NULL, 10);
                break;

            case DTYPE_TEMPERATURE:
                if (!room_for_block(sizeof(TemperatureDB))) {
                    continue;
                }
                just_added_block_size = sizeof(TemperatureDB);
                add_block_header(DATA_TEMP, just_added_block_size);
                temperature_db_init((TemperatureDB *)packet_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                break;

            case DTYPE_PRESSURE:
                just_added_block_size = sizeof(PressureDB);
                add_block_header(DATA_PRESSURE, just_added_block_size);
                pressure_db_init((PressureDB *)packet_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                break;

            case DTYPE_HUMIDITY:
                just_added_block_size = sizeof(HumidityDB);
                add_block_header(DATA_HUMIDITY, just_added_block_size);
                humidity_db_init((HumidityDB *)packet_pos, last_time, 100 * strtod(strtok(NULL, ":"), NULL));
                break;

            case DTYPE_ALTITUDE:
                just_added_block_size = sizeof(AltitudeDB);
                add_block_header(DATA_ALT, just_added_block_size);
                altitude_db_init((AltitudeDB *)packet_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                break;

            default:
                fprintf(stderr, "Unknown input data type: %s\n", dtype_str);
                continue; // Skip to next iteration without storing block
            }

            // Increment position in packet buffer to match most recently added block type
            packet_pos += just_added_block_size;
            packet_header_inc_length((PacketHeader *)packet, just_added_block_size);
        }

        pkt_count++; // One more packet constructed

        if (mq_send(out_q, (char *)packet, (packet_pos - packet), 0) == -1) {
            fprintf(stderr, "Failed to output encoded packet #%u with error: %s\n", pkt_count - 1, strerror(errno));
        }

        if (print_output) packet_print_hex(stdout, packet);

        packet_pos = packet; // Reset position to overwrite with next packet
    }
    return EXIT_SUCCESS;
}

/**
 * Converts a string to a data type enumeration value.
 * @param str The string to match with an enumeration value.
 * @return The data type matching the string value.
 */
Dtype dtype_from_str(const char *str) {
    for (uint8_t i = 0; i < sizeof(DTYPES) / sizeof(DTYPES[0]); i++) {
        if (!strcmp(str, DTYPES[i])) return i;
    }
    return DTYPE_DNE;
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

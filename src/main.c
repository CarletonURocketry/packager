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

/** Memory for storing blocks in packet as it is constructed. */
static Block blocks[BLOCK_LIMIT];
/** Static buffer for storing the contents of packets as they are being created from input. */
static uint8_t block_contents[PACKET_MAX_SIZE];
/** The current position in the block_contents array at which to allocate new contents. */
static uint8_t *contents_pos = &block_contents[0];
/** Memory for storing new block headers as they are parsed. */
static Block block;
/** Packet count tracker for encoding packets number. */
static uint16_t pkt_count = 0;
/** The packet being constructed as input is read. */
static Packet packet = {.blocks = blocks, .block_count = 0};

/** TODO: Remove this in favour of a different implementation for Packet and Block types. */
static uint8_t hacky_output_buffer[1024];

void construct_block(Block *block, DataBlockType t, size_t size);
Dtype dtype_from_str(const char *str);

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
        // Construct packet out of BLOCK_LIMIT blocks
        packet.block_count = 0;
        packet_header_init(&packet.header, callsign, 0, VERSION, ROCKET, pkt_count);

        contents_pos = &block_contents[0];
        while (packet.block_count < BLOCK_LIMIT) {

            /* Read input data. WARNING: No error handling for when text read is longer than buffer. */
            if (fgets(buffer, BUFFER_SIZE, input) == NULL) {
                no_input = true; // No more data
                break;           // Send the partially constructed packet before exiting
            }

            // Decide what contents to add to the block
            char *dtype_str = strtok(buffer, ":");
            Dtype dtype = dtype_from_str(dtype_str);

            switch (dtype) {
            case DTYPE_TIME:
                // Update with most recent time measurement to use as measurement time for other packets
                last_time = strtoul(strtok(NULL, ":"), NULL, 10);
                break;
            case DTYPE_TEMPERATURE:
                temperature_db_init((TemperatureDB *)contents_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                construct_block(&block, DATA_TEMP, sizeof(TemperatureDB));
                break;
            case DTYPE_PRESSURE:
                pressure_db_init((PressureDB *)contents_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                construct_block(&block, DATA_PRESSURE, sizeof(PressureDB));
                break;
            case DTYPE_HUMIDITY:
                humidity_db_init((HumidityDB *)contents_pos, last_time, 100 * strtod(strtok(NULL, ":"), NULL));
                construct_block(&block, DATA_HUMIDITY, sizeof(HumidityDB));
                break;
            case DTYPE_ALTITUDE:
                altitude_db_init((AltitudeDB *)contents_pos, last_time, 1000 * strtod(strtok(NULL, ":"), NULL));
                construct_block(&block, DATA_ALT, sizeof(AltitudeDB));
                break;
            default:
                fprintf(stderr, "Unknown input data type: %s\n", dtype_str);
                continue; // Skip to next iteration without storing block
            }

            // Copy the newly made block into the packet and handle insufficient space
            if (!packet_append_block(&packet, block)) {
                fprintf(stderr, "Packet size exceeded before block limit exceeded! Sending packet.\n");
                break;
            }
        }
        pkt_count++;

        if (print_output) packet_print_hex(stdout, &packet);

        // Send encoded packet over message queue
        uint8_t *cur = hacky_output_buffer;
        memcpy(cur, &packet.header, sizeof(packet.header));
        cur += sizeof(packet.header);

        for (uint8_t i = 0; i < packet.block_count; i++) {
            memcpy(cur, &packet.blocks[i].header, sizeof(packet.blocks[i].header));
            cur += sizeof(packet.blocks[i].header);
            uint16_t content_len = block_header_get_length(&packet.blocks[i].header) - sizeof(packet.blocks[i].header);
            memcpy(cur, packet.blocks[i].contents, content_len);
            cur += content_len;
        }

        if (mq_send(out_q, (char *)hacky_output_buffer, (cur - hacky_output_buffer), 0) == -1) {
            fprintf(stderr, "Failed to output encoded packet #%u with error: %s\n", pkt_count - 1, strerror(errno));
        }
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
 * Updates a block with the required information for allocating a new data block.
 * @param b The block to be updated.
 * @param t The type of the data block.
 * @param size The size of the data block that will be stored in this block.
 */
void construct_block(Block *b, DataBlockType t, size_t size) {
    block_header_init(&b->header, size, TYPE_DATA, t, GROUNDSTATION);
    b->contents = contents_pos;
    contents_pos += size;
}

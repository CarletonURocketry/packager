#include "packet_types.h"
#include <assert.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Represents the different sensor data types that can be interpreted by packager. */
typedef enum {
    DTYPE_TEMPERATURE = 0, /**< Temperature */
    DTYPE_TIME = 1,        /**< Time */
    DTYPE_PRESSURE = 2,    /** Pressure */
    DTYPE_DNE = 3,         /**< Data type does not exist */
} Dtype;

/** String representation of the possible data types. */
const char *DTYPES[] = {
    [DTYPE_TEMPERATURE] = "Temperature", [DTYPE_TIME] = "Time", [DTYPE_PRESSURE] = "Pressure", [DTYPE_DNE] = ""};

/** The size of the buffer for reading sensor data input. */
#define BUFFER_SIZE 150
/** The maximum number of blocks that can be added to a packet before it is sent. */
#define BLOCK_LIMIT 3
/** The version of the packet encoding being used. */
#define VERSION 1

/** Static variable to store the user HAM radio call sign. */
static char *callsign = NULL;
/** Static variable to store the file name to read input from instead of stdin. */
static char *file = NULL;
/** Static buffer for reading input. */
static char buffer[BUFFER_SIZE] = {0};
/** Static buffer for storing the contents of packets as they are being created from input. */
static uint8_t block_contents[PACKET_MAX_SIZE];

void debug_print_bytes(uint8_t *bytes, size_t n_bytes, bool newline);
Dtype dtype_from_str(const char *str);

int main(int argc, char **argv) {

    /* Fetch command line arguments. */
    int c;
    while ((c = getopt(argc, argv, ":f:")) != -1) {
        switch (c) {
        case 'f':
            file = optarg;
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
    FILE *input;
    if (file != NULL) {
        input = fopen(file, "r");
        if (input == NULL) {
            fprintf(stderr, "File '%s' could not be opened.\n", file);
            exit(EXIT_FAILURE);
        }
    } else {
        input = stdin;
    }

    uint64_t pkt_count = 0;
    Block blocks[BLOCK_LIMIT]; // Memory for storing blocks in packet
    Block block;               // Memory for storing new block headers

    for (;;) {
        // Construct packet out of BLOCK_LIMIT blocks
        Packet packet;
        packet.block_count = 0;
        packet.blocks = blocks;
        packet_header_init(&packet.header, callsign, 0, VERSION, ROCKET, pkt_count);

        uint8_t *contents_ptr = &block_contents[0];
        while (packet.block_count < BLOCK_LIMIT) {

            /* Read input data. WARNING: No error handling for when text read is longer than buffer. */
            if (fgets(buffer, BUFFER_SIZE, input) == NULL) return EXIT_SUCCESS; // No more data

            // Decide what contents to add to the block
            char *dtype_str = strtok(buffer, ":");
            Dtype dtype = dtype_from_str(dtype_str);
            switch (dtype) {
            case DTYPE_TIME:
                break;
            case DTYPE_TEMPERATURE:
                block_header_init(&block.header, 0, false, TYPE_DATA, DATA_ALT, GROUNDSTATION);
                uint32_t temp = strtoul(strtok(NULL, ":"), NULL, 10);
                altitude_data_block_init((AltitudeDataBlock *)contents_ptr, 0, 0, temp, 0);
                block.contents = contents_ptr;
                contents_ptr += sizeof(AltitudeDataBlock);
                break;
            case DTYPE_PRESSURE:
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

        // Send the packet over stdout
        debug_print_bytes(packet.header.bytes, sizeof(packet.header.bytes), false);
        for (uint8_t i = 0; i < packet.block_count; i++) {
            debug_print_bytes(packet.blocks[i].header.bytes, sizeof(packet.blocks[i].header.bytes), false);
            uint16_t content_len = block_header_get_length(&packet.blocks[i].header) - sizeof(packet.blocks[i].header);
            debug_print_bytes(packet.blocks[i].contents, content_len, false);
        }
        putchar('\n');
    }
    return EXIT_SUCCESS;
}

void debug_print_bytes(uint8_t *bytes, size_t n_bytes, bool newline) {
    for (size_t i = 0; i < n_bytes; i++) {
        printf("%02x ", bytes[i]);
    }
    if (newline) putchar('\n');
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

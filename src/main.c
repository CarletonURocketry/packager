#include "packet_types.h"
#include <assert.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 150

static char *callsign = NULL;
static char *file = NULL;
static char buffer[BUFFER_SIZE] = {0};

void debug_print_bytes(uint8_t *bytes, size_t n_bytes);

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

    BlockHeader b;
    block_header_init(&b, 4, true, TYPE_DATA, DATA_ALT, GROUNDSTATION);
    debug_print_bytes(b.bytes, sizeof(b));
    printf("%u\n", block_header_get_length(&b));

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

    /* Read input data. */
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        printf("Packaged: %s", buffer);
    }

    return EXIT_SUCCESS;
}

void debug_print_bytes(uint8_t *bytes, size_t n_bytes) {
    for (size_t i = 0; i < n_bytes; i++) {
        printf("%02x ", bytes[i]);
    }
    putchar('\n');
}

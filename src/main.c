#include "packet_types.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 150

static char *callsign = NULL;
static char *file = NULL;
static char buffer[BUFFER_SIZE] = {0};

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
        fputs("Call sign is required.", stderr);
        exit(EXIT_FAILURE);
    }
    callsign = argv[optind];
    printf("Using callsign %s\n", callsign);

    /* Open input stream. */
    FILE *input;
    if (file != NULL) {
        input = fopen(file, "r");
    } else {
        input = stdin;
    }

    /* Read input data. */
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        printf("Packaged: %s", buffer);
    }

    return EXIT_SUCCESS;
}

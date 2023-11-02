#include "packet_types.h"
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static char *callsign = NULL;

int main(int argc, char **argv) {

    /* Fetch command line arguments. */
    int c;
    while ((c = getopt(argc, argv, ":c:")) != -1) {
        switch (c) {
        case 'c':
            callsign = optarg;
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

    PacketHeader h;
    packet_header_init(&h, "VA3INI", 0, 0, ROCKET, 1);

    printf("Using callsign %s\n", packet_callsign(h));
    return EXIT_SUCCESS;
}

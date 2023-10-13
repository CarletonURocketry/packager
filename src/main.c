#include "packet_types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static char *callsign = NULL;

int main(int argc, char **argv) {

    /* Fetch command line arguments. */
    /* int c; */
    /* while ((c = getopt(argc, argv, "c:")) != -1) { */
    /*     switch (c) { */
    /*     case 'c': */
    /*         callsign = optarg; */
    /*         break; */
    /*     default: */
    /*         fputs("Please see the 'use' page for usage.", stderr); */
    /*         return EXIT_FAILURE; */
    /*     } */
    /* } */

    PacketHeader h;
    packet_header_init(&h, "VA3INI", 0, 0, ROCKET, 1);

    printf("Using callsign %s\n", packet_callsign(h));
    return EXIT_SUCCESS;
}

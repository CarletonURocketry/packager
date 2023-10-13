#include "packet_types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    PacketHeader h;
    packet_header_init(&h, "VA3INI", 0, 0, ROCKET, 1);

    printf("Using callsign %s\n", packet_callsign(h));
    return 0;
}

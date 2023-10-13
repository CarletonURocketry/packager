#include "packet_types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    PacketHeader h = {.contents = {
                          .callsign = {'V', 'A', '3', 'I', 'N', 'I'},
                          .length = 0,
                          .version = 0,
                          .dead_space = 0,
                          .dead_space_2 = 0,
                          .src_addr = ROCKET,
                      }};

    printf("Using callsign %s\n", CALLSIGN(h));
    return 0;
}

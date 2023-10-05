#include "arguments.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    // Parse command line arguments
    if (argc != 2) {
        puts("Expected callsign.");
    }

    const char *CALLSIGN = callsign(argv);
    printf("Using callsign %s\n", CALLSIGN);

    // Read from stdin constantly for new data
    uint8_t buffer;
    for (;;) {
        fread(&buffer, sizeof(uint8_t), 1, stdin);
    }

    return 0;
}

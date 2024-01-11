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

/** Static variable to store the user HAM radio call sign. */
static char *callsign = NULL;
/** Static variable to store the file name to read input from instead of stdin. */
static char *file = NULL;
/** Static buffer for reading input. */
static char buffer[BUFFER_SIZE] = {0};

void debug_print_bytes(uint8_t *bytes, size_t n_bytes);
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

    /* Read input data. WARNING: No error handling for when text read is longer than buffer. */
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        char *dtype_str = strtok(buffer, ":");
        Dtype dtype = dtype_from_str(dtype_str);
        switch (dtype) {
        case DTYPE_TIME:
            printf("Time: %d\n", atoi(strtok(NULL, ":")));
            break;
        case DTYPE_TEMPERATURE:
            printf("Temp: %s\n", strtok(NULL, ":"));
            break;
        case DTYPE_PRESSURE:
            printf("Pres: %s\n", strtok(NULL, ":"));
            break;
        default:
            fprintf(stderr, "Unknown input data type: %s\n", dtype_str);
            continue;
        }
    }

    return EXIT_SUCCESS;
}

void debug_print_bytes(uint8_t *bytes, size_t n_bytes) {
    for (size_t i = 0; i < n_bytes; i++) {
        printf("%02x ", bytes[i]);
    }
    putchar('\n');
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

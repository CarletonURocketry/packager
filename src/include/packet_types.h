#ifndef _PACKET_TYPES_H
#define _PACKET_TYPES_H

#include <stdint.h>

/* Possible devices from which a packet could originate from. */
typedef enum device_address {
    GROUNDSTATION = 0x0,
    ROCKET = 0x1,
    MULTICAST = 0xF,
} DeviceAddress;

/*
 * Each radio packet will have a header in this format.
 * */
typedef union packet_header {
    uint8_t bytes[12]; // Each packet header is 12 bytes
    struct packet_header_contents {
        uint8_t callsign[6];
        uint8_t length : 7;
        uint8_t version : 5;
        uint8_t dead_space : 5;
        DeviceAddress src_addr : 4;
        uint16_t packet_number : 12;
        uint16_t dead_space_2;
    };
} PacketHeader;

#endif // _PACKET_TYPES_H

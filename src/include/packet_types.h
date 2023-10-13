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
 * Each radio packet will have a header in this format. Any attribute labelled as dead space should be zero filled.
 *
 * callsign: Amateur radio call sign of the operator
 * length: The length of the packet in bytes
 * version: The version of the radio packet format being used
 * src_addr: The source address of the packet
 * packet_number: Which packet number the packet is in the stream being sent over radio
 * */
typedef union packet_header {
    uint8_t bytes[12]; // Each packet header is 12 bytes
    uint32_t words[3]; // Each packet header is 3 32-bit words
    struct {
        uint8_t callsign[6];
        uint8_t length : 7;
        uint8_t version : 5;
        uint8_t dead_space : 5;
        DeviceAddress src_addr : 4;
        uint16_t packet_number : 12;
        uint16_t dead_space_2;
    } contents;
} PacketHeader;

/* Casts the 8-bit ASCII call sign value to a null-terminated string. */
#define CALLSIGN(p) ((char *)p.contents.callsign)

#endif // _PACKET_TYPES_H

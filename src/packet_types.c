/**
 * @file packet_types.c
 * @brief Contains the definitions for all functions required to create and manipulate packet types.
 *
 * Packet types should be created using their initialization functions.
 */
#include "packet_types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Initializes a packet header with the provided information.
 *
 * @param p The packet header to be initialized
 * @param callsign The HAM radio call sign to be included in the packet header
 * @param length The length of the packet this header is associated with (in bytes)
 * @param version The version of the packet encoding format being used
 * @param source The source of the packet this header is associated with
 * @param packet_number The number of this packet (how many were sent before it)
 */
void packet_header_init(PacketHeader *p, const char *callsign, const uint8_t length, const uint8_t version,
                        const DeviceAddress source, const uint16_t packet_number) {
    // Initialize with required parameters
    p->contents.length = length;
    p->contents.version = version;
    p->contents.src_addr = source;
    p->contents.packet_number = packet_number;

    // Zero all dead space
    p->contents._dead_space = 0;
    p->contents._dead_space_2 = 0;

    /* All Canadian call signs are 5-6 characters in length. In the case of 5 character lengths, the 6th character in
     * the packet header will be the null terminator. This will not cause any issues since the null terminator is
     * effectively zero-padding, which is what's expected by the packet encoding format.
     *
     * In the case of a 6 character call sign, the null terminator will not be included in the packet header, which is
     * equally fine and follows the packet encoding format for 6 character call signs.
     */
    for (uint8_t i = 0; i < 6; i++) {
        p->contents.callsign[i] = callsign[i];
    }
}

/**
 * Initializes a block header with the provided information.
 *
 * @param b The block header to be initialized
 * @param length The length of the block this header is associated with
 * @param has_sig Whether or not the block will have a cryptographic signature
 * @param type The type of the block to follow the header
 * @param subtype The sub type of the block to follow the header
 * @param dest The device address of the destination device
 */
void block_header_init(BlockHeader *b, const uint8_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest) {
    b->contents.length = length;
    b->contents.has_sig = has_sig;
    b->contents.type = type;
    b->contents.subtype = subtype;
    b->contents.dest = dest;
    b->contents._dead_space = 0;
}
/**
 * Initializes a signal report block with the provided information.
 * @param b the signal report to be initialized
 * @param snr the signal to noise ratio, in units of 1dB/LSB
 * @param rssi the recieved signal strength indication, in units of 1dB/LSB
 * @param radio the index of the radio that is making a request for the signal report
 * @param tx_power the power with which a signal report is sent
 * @param request 1 if this is a request for a report, 0 if this is a report
 */
void signal_report_init(SignalReportBlock *b, const int8_t snr, const int8_t rssi, const uint8_t radio,
                        const int8_t tx_power, const bool request) {
    b->contents.snr = snr;
    b->contents.rssi = rssi;
    b->contents.radio = radio;
    b->contents.tx_power = tx_power;
    b->contents.request = request;
    b->contents._dead_space = 0;
}

/**
 * @file packet_types.c
 * @brief Contains the definitions for all functions required to create and manipulate packet types.
 *
 * Packet types should be created using their initialization functions.
 */
#include "packet_types.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/** The maximum size of a packet in bytes. */
static const uint16_t PACKET_MAX_SIZE = 256;

/* Copies memory from source to destination in big endian format.
 * @param dest The destination buffer.
 * @param src The source buffer.
 * @param n_bytes The size of the source buffer in bytes.
 */
void memcpy_be(void *dest, const void *src, unsigned long n_bytes) {
    for (unsigned long i = n_bytes; i > 0; i--) {
        ((uint8_t *)dest)[n_bytes - i] = ((const uint8_t *)src)[i - 1];
    }
}

/**
 * Initializes a packet header with the provided information.
 *
 * @param p The packet header to be initialized
 * @param callsign The HAM radio call sign to be included in the packet header
 * @param length The length of the packet this header is associated with, in bytes, not including the header itself
 * @param version The version of the packet encoding format being used
 * @param source The source of the packet this header is associated with
 * @param packet_number The number of this packet (how many were sent before it)
 */
void packet_header_init(PacketHeader *p, const char *callsign, const uint16_t length, const uint8_t version,
                        const DeviceAddress source, const uint16_t packet_number) {

    /* All Canadian call signs are 5-6 characters in length. In the case of 5 character lengths, the 6th character in
     * the packet header will be the null terminator. This will not cause any issues since the null terminator is
     * effectively zero-padding, which is what's expected by the packet encoding format.
     *
     * In the case of a 6 character call sign, the null terminator will not be included in the packet header, which is
     * equally fine and follows the packet encoding format for 6 character call signs.
     */
    memcpy(p->bytes, callsign, 6);

    packet_header_set_length(p, length);
    p->bytes[6] |= (uint8_t)((version & 0x18) >> 3); // First two bits of version right after length
    p->bytes[7] = (uint8_t)((version & 0x07) << 5);  // Last three bits of version at start of byte
    // Remaining 5 bits in byte 8 are dead space
    p->bytes[8] = (uint8_t)((source & 0x0F) << 4);           // Last four bits are source
    p->bytes[8] |= (uint8_t)((packet_number & 0x0F00) >> 8); // First four bits of packet_number fill out rest of byte
    p->bytes[9] = (uint8_t)(packet_number & 0x00FF);         // Last 8 bits fill out byte
    p->bytes[10] = 0;                                        // Dead space
    p->bytes[11] = 0;                                        // Dead space
}

/**
 * Sets the length of the packet the header is associated with.
 * @param p The packet header to store the length in.
 * @param length The length of the packet in bytes, not including the packet header itself.
 */
void packet_header_set_length(PacketHeader *p, const uint16_t length) {
    uint8_t encoded_length = ((length + sizeof(PacketHeader)) / 4) - 1; // Include header length, 4 byte increments
    p->bytes[6] &= ~0xFC;                                               // Clear top 6 bits
    p->bytes[6] |= (encoded_length & 0x3F) << 2;                        // OR in bottom 6 bits of length, shifted left
}

/**
 * Gets the length of the packet header.
 * @param p The packet header to read the length from.
 * @return The length of the packet header in bytes, including itself.
 */
uint16_t packet_header_get_length(const PacketHeader *p) { return (((p->bytes[6] & 0xFC) >> 2) + 1) * 4; }

/**
 * Initializes a block header with the provided information.
 *
 * @param b The block header to be initialized
 * @param length The length of the block this header is associated with, in 4 byte increments
 * @param has_sig Whether or not the block will have a cryptographic signature
 * @param type The type of the block to follow the header
 * @param subtype The sub type of the block to follow the header
 * @param dest The device address of the destination device
 */
void block_header_init(BlockHeader *b, const uint16_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest) {

    b->bytes[0] = (uint8_t)(has_sig & 0x1) << 3; // Shift in signature indicator
    b->bytes[0] |= (uint8_t)(type & 0xC) >> 2;   // First two bits of type at end of byte
    block_header_set_length(b, length);
    b->bytes[1] = (uint8_t)(type & 0x3) << 6; // Last two bits of type at start of byte
    b->bytes[1] |= (uint8_t)(subtype & 0x3F); // 6 bits of sub type fill out byte
    b->bytes[2] = (uint8_t)(dest & 0xF) << 4; // Destination in top of byte
    b->bytes[3] = 0;                          // Dead space
}

/**
 * Sets the length of the block the block header is associated with.
 * @param b The block header to store the length in.
 * @param length The length of the block in bytes, not including the header itself. Must be a multiple of 4.
 */
void block_header_set_length(BlockHeader *b, const uint16_t length) {
    uint8_t encoded_length = ((length + sizeof(BlockHeader)) / 4) - 1; // Add header size, 4 byte increments
    encoded_length &= 0x1F;                                            // Last 5 bits are length
    b->bytes[0] &= ~0xF8;                                              // Clear the first 5 bits
    b->bytes[0] |= (encoded_length << 3);                              // Length shifted to start of byte
}

/**
 * Gets the length of the block header.
 * @param p The block header to read the length from.
 * @return The length of the block header in bytes, including itself.
 */
uint16_t block_header_get_length(const BlockHeader *b) { return (((b->bytes[0] & 0xF8) >> 3) + 1) * 4; }

/**
 * Initializes a signal report block with the provided information.
 * @param b The signal report to be initialized
 * @param snr The signal to noise ratio, in units of 1dB/LSB
 * @param rssi The received signal strength indication, in units of 1dB/LSB
 * @param radio The index of the radio that is making a request for the signal report
 * @param tx_power The power with which a signal report is sent
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
/**
 * Initializes an altitude data block with the provided information.
 * @param b The altitude data to be initialized
 * @param measurement_time The mission time at the taking of the measurement
 * @param pressure The measured pressure in terms of Pascals.
 * @param temperature The measured temperature in units of 1 millidegree Celsius/LSB.
 * @param altitude The calculated altitude in units of 1 mm/LSB.
 */
void altitude_data_block_init(AltitudeDataBlock *b, const uint32_t measurement_time, const int32_t pressure,
                              const int32_t temperature, const int32_t altitude) {
    memcpy(b->bytes, &measurement_time, sizeof(measurement_time));
    memcpy(b->bytes + sizeof(measurement_time), &pressure, sizeof(pressure));
    memcpy(b->bytes + sizeof(measurement_time) + sizeof(pressure), &temperature, sizeof(temperature));
    memcpy(b->bytes + sizeof(measurement_time) + sizeof(pressure) + sizeof(temperature), &altitude, sizeof(altitude));
}
/**
 * Initializes an angular velocity block with the provided information.
 * @param b The angular velocity block to be initialized.
 * @param measurement_time The mission time when the measurement was taken.
 * @param full_scale_range The full scale range of the gyroscope in degrees per second. This value represents the
 * maximum and minimum angular velocity that can be measured.
 * @param x_axis The angular velocity measurement for the x axis.
 * @param y_axis The angular velocity measurement for the y axis.
 * @param z_axis The angular velocity measurement for the z axis.
 */
void angular_velocity_block_init(AngularVelocityBlock *b, const uint32_t measurement_time,
                                 const int8_t full_scale_range, const int16_t x_axis, const int16_t y_axis,
                                 const int16_t z_axis) {
    memcpy(b->bytes, &measurement_time, sizeof(measurement_time));
    memcpy(b->bytes + sizeof(measurement_time), &full_scale_range, sizeof(full_scale_range));
    memcpy(b->bytes + sizeof(measurement_time) + sizeof(full_scale_range), &x_axis, sizeof(x_axis));
    memcpy(b->bytes + sizeof(measurement_time) + sizeof(full_scale_range) + sizeof(x_axis), &y_axis, sizeof(y_axis));
    memcpy(b->bytes + sizeof(measurement_time) + sizeof(full_scale_range) + sizeof(x_axis) + sizeof(y_axis), &z_axis,
           sizeof(z_axis));
}

/**
 * Initializes an acceleration data block with the provided information.
 * @param b The acceleration data block to be initialized.
 * @param measurement_time The mission time when the measurement was taken.
 * @param full_scale_range The full scale range of the gyroscope in degrees per second. This value represents the
 * maximum and minimum acceleration that can be measured.
 * @param x_axis The acceleration measurement for the x axis.
 * @param y_axis The acceleration measurement for the y axis.
 * @param z_axis The acceleration measurement for the z axis.
 * */
void acceleration_data_block_init(AccelerationDataBlock *b, const uint32_t measurement_time,
                                  const int8_t full_scale_range, const int16_t x_axis, const int16_t y_axis,
                                  const int16_t z_axis) {
    memcpy(b->bytes, &measurement_time, sizeof(measurement_time));
    memcpy(b->bytes + sizeof(measurement_time), &full_scale_range, sizeof(full_scale_range));
    // One byte of dead space after FSR
    memcpy(b->bytes + 1 + sizeof(measurement_time) + sizeof(full_scale_range), &x_axis, sizeof(x_axis));
    memcpy(b->bytes + 1 + sizeof(measurement_time) + sizeof(full_scale_range) + sizeof(x_axis), &y_axis,
           sizeof(y_axis));
    memcpy(b->bytes + 1 + sizeof(measurement_time) + sizeof(full_scale_range) + sizeof(x_axis) + sizeof(z_axis),
           &z_axis, sizeof(z_axis));
}

/**
 * Initializes a telemetry data block with the provided information.
 * @param b The telemetry request block to be initialized.
 * @param data_subtype_1 The first requested data type.
 * @param used_1 A boolean to determine if the first data request is valid.
 * @param data_subtype_2 The second requested data type.
 * @param used_2 A boolean to determine if the second data request is valid.
 * @param data_subtype_3 The third requested data type.
 * @param used_3 A boolean to determine if the third data request is valid.
 * @param data_subtype_4 The fourth requested data type.
 * @param used_4 A boolean to determine if the fourth data request is valid.
 * */
void telemetry_request_block_init(TelemetryRequestBlock *b, const uint8_t data_subtype_1, const uint8_t used_1,
                                  const uint8_t data_subtype_2, const uint8_t used_2, const uint8_t data_subtype_3,
                                  const uint8_t used_3, const uint8_t data_subtype_4, const uint8_t used_4) {

    b->bytes[0] = data_subtype_1 << 2;
    b->bytes[0] |= used_1 && 0x01;
    b->bytes[1] = data_subtype_2 << 2;
    b->bytes[1] |= used_2 && 0x01;
    b->bytes[2] = data_subtype_3 << 2;
    b->bytes[2] |= used_3 && 0x01;
    b->bytes[3] = data_subtype_4 << 2;
    b->bytes[3] |= used_4 && 0x01;
}
/**
 * Appends a block to a packet. WARNING: This function assumes that there is sufficient memory in the packet to store
 * the block.
 * @param p The packet to be appended to.
 * @param b The block to append.
 * @return True if the append succeeded, false if there was no space to append the block.
 */
bool packet_append_block(Packet *p, const Block b) {

    const uint16_t p_len = packet_header_get_length(&p->header);
    const uint16_t b_len = block_header_get_length(&b.header);

    // Ensure that there is enough space for the block to be added to the packet
    if (p_len + b_len > PACKET_MAX_SIZE) return false;

    // If packet is just a header, first block goes in slot 0
    if (p_len == sizeof(PacketHeader)) {
        p->blocks[0] = b;
        packet_header_set_length(&p->header, b_len); // The packet is now the length of the first block
        return true;
    }

    uint16_t remaining_len = p_len;
    remaining_len -= sizeof(PacketHeader);                          // Subtract packet header length
    remaining_len -= block_header_get_length(&p->blocks[0].header); // Subtract length of first block

    // While there is still content left in the packet, get the next block
    uint16_t i = 1;
    for (; remaining_len > 0; i++) {
        remaining_len -= block_header_get_length(&p->blocks[i].header);
    }
    p->blocks[i] = b; // Add block at correct spot
    // Packet length is now equal to its previous length + the new block
    packet_header_set_length(&p->header, p_len - sizeof(PacketHeader) + b_len);
    return true;
}

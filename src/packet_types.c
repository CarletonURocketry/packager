/**
 * @file packet_types.c
 * @brief Contains the definitions for all functions required to create and manipulate packet types.
 *
 * Packet types should be created using their initialization functions.
 */
#include "packet_types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
                        const DeviceAddress source, const uint32_t packet_number) {

    // Copying the entire call sign, excluding null terminator
    uint8_t call_sign_char_count = strlen(callsign);
    memcpy(p->call_sign, callsign, call_sign_char_count);
    // Fill remaining chars with null terminator
    memset(&p->call_sign[call_sign_char_count], 0, sizeof(p->call_sign) - call_sign_char_count);

    packet_header_set_length(p, length); // Set length
    p->version = version;
    p->src_addr = source;
    p->packet_num = packet_number;
}

/**
 * Initializes a block header with the provided information.
 *
 * @param b The block header to be initialized
 * @param length The length of the block this header is associated with, in bytes, not including the header itself
 * @param type The type of the block to follow the header
 * @param subtype The sub type of the block to follow the header
 * @param dest The device address of the destination device
 */
void block_header_init(BlockHeader *b, const uint16_t length, const BlockType type, const BlockSubtype subtype,
                       const DeviceAddress dest) {

    block_header_set_length(b, length);
    b->type = type;
    b->subtype = subtype;
    b->dest_addr = dest;
}

/**
 * Initializes an altitude data block with the provided information.
 * @param b The altitude data to be initialized
 * @param mission_time The mission time at the taking of the measurement
 * @param altitude The calculated altitude in units millimetres above/below launch height.
 */
void altitude_db_init(AltitudeDB *b, const uint32_t mission_time, const int32_t altitude) {
    b->mission_time = mission_time;
    b->altitude = altitude;
}

/**
 * Initializes a temperature data block with the provided information.
 * @param b The temperature data block to be initialized.
 * @param mission_time The mission time at the taking of the measurement
 * @param temperature The calculated temperature in units of millidegrees Celsius.
 */
void temperature_db_init(TemperatureDB *b, const uint32_t mission_time, const int32_t temperature) {
    b->mission_time = mission_time;
    b->temperature = temperature;
}

/**
 * Initializes a temperature data block with the provided information.
 * @param b The temperature data block to be initialized.
 * @param mission_time The mission time at the taking of the measurement
 * @param pressure The calculated pressure in units of Pascals.
 */
void pressure_db_init(PressureDB *b, const uint32_t mission_time, const int32_t pressure) {
    b->mission_time = mission_time;
    b->pressure = pressure;
}

/**
 * Initializes an angular velocity block with the provided information.
 * @param b The angular velocity block to be initialized.
 * @param mission_time The mission time when the measurement was taken.
 * @param x_axis The angular velocity measurement for the x axis in tenths of degrees per second.
 * @param y_axis The angular velocity measurement for the y axis in tenths of degrees per second.
 * @param z_axis The angular velocity measurement for the z axis in tenths of degrees per second.
 */
void angular_velocity_db_init(AngularVelocityDB *b, const uint32_t mission_time, const int16_t x_axis,
                              const int16_t y_axis, const int16_t z_axis) {
    b->mission_time = mission_time;
    b->x = x_axis;
    b->y = y_axis;
    b->z = z_axis;
}

/**
 * Initializes an acceleration data block with the provided information.
 * @param b The acceleration data block to be initialized.
 * @param mission_time The mission time when the measurement was taken.
 * @param x_axis The acceleration measurement for the x axis in centimetres per second squared.
 * @param y_axis The acceleration measurement for the y axis in centimetres per second squared.
 * @param z_axis The acceleration measurement for the z axis in centimetres per second squared.
 * */
void acceleration_db_init(AccelerationDB *b, const uint32_t mission_time, const int16_t x_axis, const int16_t y_axis,
                          const int16_t z_axis) {
    b->mission_time = mission_time;
    b->x = x_axis;
    b->y = y_axis;
    b->z = z_axis;
}

/**
 * Initializes a humidity data block with the provided information.
 * @param b The humidity data block to be initialized.
 * @param mission_time The mission time at the taking of the measurement
 * @param humidity The calculated humidity in ten thousandths of a percent.
 */
void humidity_db_init(HumidityDB *b, const uint32_t mission_time, const uint32_t humidity) {
    b->mission_time = mission_time;
    b->humidity = humidity;
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
        p->block_count++;
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
    p->block_count++;
    return true;
}

#define write_bytes_sized(stream, bytes, size)                                                                         \
    for (size_t hjkl = 0; hjkl < size; hjkl++) {                                                                       \
        fprintf(stream, "%02x", bytes[hjkl]);                                                                          \
    }

/**
 * Prints a packet to the output stream in hexadecimal representation.
 * @param stream The output stream to which the packet should be printed.
 * @param packet The packet to be printed.
 */
void packet_print_hex(FILE *stream, Packet *packet) {
    write_bytes_sized(stream, ((uint8_t *)(&packet->header)), sizeof(packet->header));
    for (uint8_t i = 0; i < packet->block_count; i++) {
        write_bytes_sized(stream, ((uint8_t *)(&packet->blocks[i].header)), sizeof(packet->blocks[i].header));
        uint16_t content_len = block_header_get_length(&packet->blocks[i].header) - sizeof(packet->blocks[i].header);
        write_bytes_sized(stream, packet->blocks[i].contents, content_len);
    }
    fputc('\n', stream);
}

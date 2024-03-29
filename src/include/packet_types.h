/**
 * @file packet_types.h
 * @brief Contains the types and functions required for constructing packets.
 *
 * This header file contains all function prototypes for packet_types.c, as well as the enums specified in the CUInSpace
 * packet encoding spec for device addresses, block types and sub-types.
 *
 * All packet types are tightly packed in order to fit in their exact specified byte size. Contents of the packet types
 * can be accessed individually or as a byte array.
 */

#ifndef _PACKET_TYPES_H
#define _PACKET_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/** The maximum size a packet can be in bytes. */
#define PACKET_MAX_SIZE 256

/** The maximum size a block can be in bytes. */
#define BLOCK_MAX_SIZE 128

// Don't confuse Doxygen documenting with the attribute macro
#if __DOXYGEN__
#define TIGHTLY_PACKED
#else
/** Makes a struct/unions members be aligned as tightly as possible. */
#define TIGHTLY_PACKED __attribute__((packed, aligned(1)))
#endif

void memcpy_be(void *dest, const void *src, unsigned long n_bytes);

/** Possible devices from which a packet could originate or be sent to. */
typedef enum device_address {
    GROUNDSTATION = 0x0, /**< Ground station */
    ROCKET = 0x1,        /**< The rocket */
    MULTICAST = 0xFF,    /**< Any device which is listening */
} DeviceAddress;

/** Possible types of radio packet blocks that could be sent. */
typedef enum block_type {
    TYPE_DATA = 0x0, /**< Data block */
} BlockType;

/** Possible sub-types of data blocks that can be sent. */
typedef enum data_block_type {
    DATA_DBG_MSG = 0x0,     /**< Debug message */
    DATA_ALT = 0x1,         /**< Altitude data */
    DATA_TEMP = 0x2,        /**< Temperature data */
    DATA_PRESSURE = 0x3,    /**< Pressure data */
    DATA_ACCEL = 0x4,       /**< Acceleration data */
    DATA_ANGULAR_VEL = 0x5, /**< Angular velocity data */
    DATA_GNSS_LOC = 0x6,    /**< GNSS location data */
    DATA_GNSS_META = 0x7,   /**< GNSS metadata */
    DATA_HUMIDITY = 0x8,    /**< Humidity data */
} DataBlockType;

/** Any block sub-type from DataBlockType, CtrlBlockType or CmdBlockType. */
typedef uint8_t BlockSubtype;

/** Each radio packet will have a header in this format. */
typedef struct {
    /** The HAM radio call sign with trailing null characters. */
    char call_sign[9];
    /** The packet length in multiples of 4 bytes. */
    uint8_t len;
    /** The version of InSpace radio packet encoding being used. */
    uint8_t version;
    /** The source address of the packet. */
    uint8_t src_addr;
    /** Which number this packet is in the stream of sent packets. */
    uint32_t packet_num;
} PacketHeader;

void packet_header_init(PacketHeader *p, const char *callsign, const uint16_t length, const uint8_t version,
                        const DeviceAddress source, const uint32_t packet_number);

/** Each block in the radio packet will have a header in this format. */
typedef struct {
    /** The block header accessed as a bytes array. */
    uint8_t len;
    /** The type of this block. */
    uint8_t type;
    /** The sub type of this block. */
    uint8_t subtype;
    /** The address of this blocks destination device. */
    uint8_t dest_addr;
} BlockHeader;

void block_header_init(BlockHeader *b, const uint16_t length, const BlockType type, const BlockSubtype subtype,
                       const DeviceAddress dest);

/** Signal report for the last block that was sent by the block's destination device */
typedef struct signal_report_block {
    /** The signal block report accessed as a bytes array */
    uint8_t bytes[4];
} SignalReportBlock;

void signal_report_init(SignalReportBlock *b, const int8_t snr, const int8_t rssi, const uint8_t radio,
                        const int8_t tx_power, const bool request);

/** A data block containing information about altitude. */
typedef struct {
    /** The altitude data block accessed as a bytes array */
    uint8_t bytes[8];
} AltitudeDB;

void altitude_db_init(AltitudeDB *b, const uint32_t measurement_time, const int32_t altitude);

/** A data block containing information about temperature. */
typedef struct {
    /** The temperature data block can be accessed as a bytes array. */
    uint8_t bytes[8];
} TemperatureDB;

void temperature_db_init(TemperatureDB *b, const uint32_t measurement_time, const int32_t temperature);

/** A data block containing information about humidity. */
typedef struct {
    /** The humidity data block can be accessed as a bytes array. */
    uint8_t bytes[8];
} HumidityDB;

void humidity_db_init(HumidityDB *b, const uint32_t measurement_time, const uint32_t humidity);

/** A data block containing information about pressure. */
typedef struct {
    /** The pressure data block can be accessed as a bytes array. */
    uint8_t bytes[8];
} PressureDB;

void pressure_db_init(PressureDB *b, const uint32_t measurement_time, const int32_t pressure);

/** A data block containing information about angular velocity. */
typedef struct {
    /** The angular velocity block accessed as a bytes array */
    uint8_t bytes[12];
} AngularVelocityDB;

void angular_velocity_db_init(AngularVelocityDB *b, const uint32_t measurement_time, const int8_t full_scale_range,
                              const int16_t x_axis, const int16_t y_axis, const int16_t z_axis);

/** A data block containing information about acceleration. */
typedef struct acceleration_data_block {
    uint8_t bytes[12];
} AccelerationDB;

void acceleration_db_init(AccelerationDB *b, const uint32_t measurement_time, const int8_t full_scale_range,
                          const int16_t x_axis, const int16_t y_axis, const int16_t z_axis);

typedef struct telemetry_request_block {
    /** The telemetry request block accessed as a bytes array. */
    uint8_t bytes[4];
} TelemetryRequestBlock;

void telemetry_request_block_init(TelemetryRequestBlock *b, const uint8_t data_subtype_1, const uint8_t used_1,
                                  const uint8_t data_subtype_2, const uint8_t used_2, const uint8_t data_subtype_3,
                                  const uint8_t used_3, const uint8_t data_subtype_4, const uint8_t used_4);

/** A data block containing location information provided by a GNSS sensor */
typedef struct gnss_location_data_block {
    uint8_t bytes[32];
} GNSSLocationDB;

void gnss_location_db_init(GNSSLocationDB *b, const uint32_t fix_time, const int32_t latitude, const int32_t longitude,
                           const uint32_t utc_time, const int32_t altitude, int16_t speed, int16_t course,
                           uint16_t pdop, uint16_t hdop, uint16_t vdop, uint8_t sats, uint8_t fix);

/** Represents a radio packet block with variable length contents. */
typedef struct {
    /** The block header. Block length is encoded here. */
    BlockHeader header;
    /** The block contents up to a length of 128 bytes. */
    uint8_t *contents;
} Block;

/** Represents a packet with a variable number of blocks. Maximum of 256 bytes. */
typedef struct {
    /** The packet header. Packet length is encoded here. */
    PacketHeader header;
    /** Packet contents in blocks, up to 256 bytes long. */
    Block *blocks;
    /** The number of blocks in this packet. */
    uint8_t block_count;
} Packet;

bool packet_append_block(Packet *p, const Block b);
void packet_print_hex(FILE *stream, Packet *packet);

/**
 * Sets the length of the packet the header is associated with.
 * @param p The packet header to store the length in.
 * @param length The length of the packet in bytes, not including the packet header itself.
 */
static inline void packet_header_set_length(PacketHeader *p, const uint16_t length) {
    p->len = ((length + sizeof(PacketHeader)) / 4) - 1;
}

/**
 * Gets the length of the packet header.
 * @param p The packet header to read the length from.
 * @return The length of the packet header in bytes, including itself.
 */
static inline uint16_t packet_header_get_length(const PacketHeader *p) { return (p->len + 1) * 4; }

/**
 * Sets the length of the block the block header is associated with.
 * @param b The block header to store the length in.
 * @param length The length of the block in bytes, not including the header itself. Must be a multiple of 4.
 */
static inline void block_header_set_length(BlockHeader *b, const uint16_t length) {
    b->len = ((length + sizeof(BlockHeader)) / 4) - 1;
}

/**
 * Gets the length of the block header.
 * @param p The block header to read the length from.
 * @return The length of the block header in bytes, including itself.
 */
static inline uint16_t block_header_get_length(const BlockHeader *b) { return (b->len + 1) * 4; }

#endif // _PACKET_TYPES_H

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
    MULTICAST = 0xF,     /**< Any device which is listening */
} DeviceAddress;

/** Possible types of radio packet blocks that could be sent. */
typedef enum block_type {
    TYPE_CTRL = 0x0, /**< Control block */
    TYPE_CMD = 0x1,  /**< Command block */
    TYPE_DATA = 0x2, /**< Data block */
} BlockType;

/** Possible sub-types of control blocks that can be sent. */
typedef enum control_block_type {
    CTRL_SIGNAL_REPORT = 0x0,   /**< Signal report block */
    CTRL_CMD_ACK = 0x1,         /**< Command acknowledgement block */
    CTRL_CMD_NONCE_RQST = 0x2,  /**< Command nonce request block */
    CTRL_CMD_NONCE = 0x3,       /**< Command nonce block */
    CTRL_BEACON = 0x4,          /**< Beacon block (searching for peer device) */
    CTRL_BEACON_RESPONSE = 0x5, /**< Beacon response block (response from peer device) */
} CtrlBlockType;

/** Possible sub-types of command blocks that can be sent. */
typedef enum command_block_type {
    CMD_RST_ROCKET_AV = 0X0,   /**< Reset rocket avionics */
    CMD_RQST_TELEM_DATA = 0X1, /**< Request for telemetry data */
    CMD_DEPLOY_CHUTE = 0X2,    /**< Deploy parachutes */
    CMD_TARE_SENSORS = 0X3,    /**< Tare sensors */
} CmdBlockType;

/** Possible sub-types of data blocks that can be sent. */
typedef enum data_block_type {
    DATA_DBG_MSG = 0x0,          /**< Debug message */
    DATA_STATUS = 0x1,           /**< Debug status */
    DATA_STARTUP_MSG = 0x2,      /**< Startup message */
    DATA_ALT = 0x3,              /**< Altitude data */
    DATA_ACCEL = 0x3,            /**< Acceleration data */
    DATA_ANGULAR_VEL = 0x4,      /**< Angular velocity data */
    DATA_GNSS_LOC = 0x5,         /**< GNSS location data */
    DATA_GNSS_META = 0x6,        /**< GNSS metadata */
    DATA_PWR_INFO = 0x7,         /**< Power info */
    DATA_TEMP = 0x8,             /**< Temperature data */
    DATA_MPU9250_IMU = 0x9,      /**< MPU9250-IMU data */
    DATA_KX134_1211_ACCEL = 0xA, /**< KX134-1211 accelerometer data */
} DataBlockType;

/** Any block sub-type from DataBlockType, CtrlBlockType or CmdBlockType. */
typedef uint8_t BlockSubtype;

/** Each radio packet will have a header in this format. */
typedef struct packet_header {
    /** The packet header accessed as a bytes array. */
    uint8_t bytes[12];
} PacketHeader;

void packet_header_init(PacketHeader *p, const char *callsign, const uint16_t length, const uint8_t version,
                        const DeviceAddress source, const uint16_t packet_number);

/** Each block in the radio packet will have a header in this format. */
typedef struct block_header {
    /** The block header accessed as a bytes array. */
    uint8_t bytes[4];
} BlockHeader;

void block_header_init(BlockHeader *b, const uint16_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest);

/** Signal report for the last block that was sent by the block's destination device */
typedef struct signal_report_block {
    /** The signal block report accessed as a bytes array */
    uint8_t bytes[4];
} SignalReportBlock;

void signal_report_init(SignalReportBlock *b, const int8_t snr, const int8_t rssi, const uint8_t radio,
                        const int8_t tx_power, const bool request);

/** A data block containing information about altitude. */
typedef struct altitude_data_block {
    /** The altitude data block accessed as a bytes array */
    uint8_t bytes[16];
} AltitudeDataBlock;

void altitude_data_block_init(AltitudeDataBlock *b, const uint32_t measurement_time, const int32_t pressure,
                              const int32_t temperature, const int32_t altitude);

/** A data block containing information about angular velocity. */
typedef struct angular_velocity_block {
    /** The angular velocity block accessed as a bytes array */
    uint8_t bytes[12];
} AngularVelocityBlock;

void angular_velocity_block_init(AngularVelocityBlock *b, const uint32_t measurement_time,
                                 const int8_t full_scale_range, const int16_t x_axis, const int16_t y_axis,
                                 const int16_t z_axis);

/** A data block containing information about acceleration. */
typedef struct acceleration_data_block {
    uint8_t bytes[12];
} AccelerationDataBlock;

void acceleration_data_block_init(AccelerationDataBlock *b, const uint32_t measurement_time,
                                  const int8_t full_scale_range, const int16_t x_axis, const int16_t y_axis,
                                  const int16_t z_axis);

typedef struct telemetry_request_block {
    /** The telemetry request block accessed as a bytes array. */
    uint8_t bytes[4];
} TelemetryRequestBlock;

void telemetry_request_block_init(TelemetryRequestBlock *b, const uint8_t data_subtype_1, const uint8_t used_1,
                                  const uint8_t data_subtype_2, const uint8_t used_2, const uint8_t data_subtype_3,
                                  const uint8_t used_3, const uint8_t data_subtype_4, const uint8_t used_4);

/** Represents a radio packet block with variable length contents. */
typedef struct {
    /** The block header. Block length is encoded here. */
    BlockHeader header;
    /** The block contents up to a length of 128 bytes. */
    uint8_t *contents;
} TIGHTLY_PACKED Block;

/** Represents a packet with a variable number of blocks. Maximum of 256 bytes. */
typedef struct {
    /** The packet header. Packet length is encoded here. */
    PacketHeader header;
    /** Packet contents in blocks, up to 256 bytes long. */
    Block *blocks;
    /** The number of blocks in this packet. */
    uint8_t block_count;
} TIGHTLY_PACKED Packet;

bool packet_append_block(Packet *p, const Block b);

/**
 * Sets the length of the packet the header is associated with.
 * @param p The packet header to store the length in.
 * @param length The length of the packet in bytes, not including the packet header itself.
 */
inline void packet_header_set_length(PacketHeader *p, const uint16_t length) {
    uint8_t encoded_length = ((length + sizeof(PacketHeader)) / 4) - 1; // Include header length, 4 byte increments
    p->bytes[6] &= ~0xFC;                                               // Clear top 6 bits
    p->bytes[6] |= (encoded_length & 0x3F) << 2;                        // OR in bottom 6 bits of length, shifted left
}

/**
 * Gets the length of the packet header.
 * @param p The packet header to read the length from.
 * @return The length of the packet header in bytes, including itself.
 */
inline uint16_t packet_header_get_length(const PacketHeader *p) { return (((p->bytes[6] & 0xFC) >> 2) + 1) * 4; }

/**
 * Sets the length of the block the block header is associated with.
 * @param b The block header to store the length in.
 * @param length The length of the block in bytes, not including the header itself. Must be a multiple of 4.
 */
inline void block_header_set_length(BlockHeader *b, const uint16_t length) {
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
inline uint16_t block_header_get_length(const BlockHeader *b) { return (((b->bytes[0] & 0xF8) >> 3) + 1) * 4; }

#endif // _PACKET_TYPES_H

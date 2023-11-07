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

// Don't confuse Doxygen documenting with the attribute macro
#if __DOXYGEN__
#define TIGHTLY_PACKED
#else
/** Makes a struct/unions members be aligned as tightly as possible. */
#define TIGHTLY_PACKED __attribute__((packed, aligned(1)))
#endif

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
uint16_t packet_header_get_length(const PacketHeader *p);
void packet_header_set_length(PacketHeader *p, uint16_t length);

/** Each block in the radio packet will have a header in this format. */
typedef struct block_header {
    /** The block header accessed as a bytes array. */
    uint8_t bytes[4];
} BlockHeader;

void block_header_init(BlockHeader *b, const uint16_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest);
uint16_t block_header_get_length(const BlockHeader *p);
void block_header_set_length(BlockHeader *p, const uint16_t length);

/** Signal report for the last block that was sent by the block's destination device */
typedef union signal_report_block {
    /** The signal block report accessed as a bytes array */
    uint8_t bytes[4];
    /** Individually accessible components of the signal report */
    struct {
        /** The signal to noise ratio, in units of 1dB/LSB */
        int8_t snr : 8;
        /** The recieved signal strength indication, in units of 1dB/LSB */
        int8_t rssi : 8;
        /** The index of the radio that sends the request for a report*/
        uint8_t radio : 2;
        /** Transmit power with which this report was sent in units of 1dB/LSB */
        int8_t tx_power : 6;
        /** Reserved bits */
        uint8_t _dead_space : 7;
        /** When set, indicates this block is a request, reciever should respond*/
        bool request : 1;
    } TIGHTLY_PACKED contents;
} SignalReportBlock;

void signal_report_init(SignalReportBlock *b, const int8_t snr, const int8_t rssi, const uint8_t radio,
                        const int8_t tx_power, const bool request);

typedef struct altitude_data_block {
    /**The altitude data block accessed as a bytes array*/
    uint8_t bytes[16];
} AltitudeDataBlock;

void altitude_data_block_init(AltitudeDataBlock *b, const uint32_t measurement_time, const int32_t pressure,
                              const uint32_t temperature, const uint32_t altitude);

typedef struct angular_velocity_block {
    /**The angular velocity block accessed as a bytes array*/
    uint8_t bytes[12];
} AngularVelocityBlock;

void angular_velocity_block_init(AngularVelocityBlock *b, const uint32_t measurement_time,
                                 const int8_t full_scale_range, const int16_t x_axis, const int16_t y_axis,
                                 const int16_t z_axis);

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
} TIGHTLY_PACKED Packet;

bool packet_append_block(Packet *p, Block *b);

#endif // _PACKET_TYPES_H

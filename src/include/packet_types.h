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
typedef union packet_header {
    /** The packet header accessed as a bytes array. */
    uint8_t bytes[12];
    /** Individually accessible components of the block header's contents. */
    struct {
        /** Amateur radio call sign of the operator. */
        uint8_t callsign[6];
        /** The length of the packet in bytes (including the header). */
        uint8_t length : 6;
        /** The version of the radio packet format being used. */
        uint8_t version : 5;
        uint8_t _dead_space : 5;
        /** The source address of the packet. */
        DeviceAddress src_addr : 4;
        /** Which packet number the packet is in the stream being sent over radio. */
        uint16_t packet_number : 12;
        uint16_t _dead_space_2 : 16;
    } TIGHTLY_PACKED contents;
} PacketHeader;

/** Casts the 8-bit ASCII call sign value to a null-terminated string. */
#define packet_callsign(p) ((char *)p.contents.callsign)

void packet_header_init(PacketHeader *p, const char *callsign, const uint8_t length, const uint8_t version,
                        const DeviceAddress source, const uint16_t packet_number);

/** Each block in the radio packet will have a header in this format. */
typedef union block_header {
    /** The block header accessed as a bytes array. */
    uint8_t bytes[4];
    /** Individually accessible components of the packet header. */
    struct {
        /** The length of the block in bytes. */
        uint8_t length : 5;
        /** Whether or not the block has a cryptographic signature. */
        bool has_sig : 1;
        /** The type of block. */
        BlockType type : 4;
        /** The block type's subtype. */
        BlockSubtype subtype : 6;
        /** The device address of the destination. */
        DeviceAddress dest : 4;
        uint16_t _dead_space : 12;
    } TIGHTLY_PACKED contents;
} BlockHeader;

void block_header_init(BlockHeader *b, const uint8_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest);

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

typedef struct altitude_data_block{
    /**The altitude data block accessed as a bytes array*/
    uint8_t bytes[4];
    /**The time at which the altitude was measured*/
    int8_t measurment_type : 32;
    /**The pressure at the measured atltitude*/
    int8_t pressure : 32;
    /**The temperature at the measured altitude*/
    int8_t temperature : 32;
    /**The measured altitude*/
    int8_t altitude : 32;
} TIGHTLY_PACKED AltitudeDataBlock; 

void altitude_data_block_init(AltitudeDataBlock *b, const int8_t measurment_type, const uint8_t pressure, const uint8_t temperature,
                              const uint8_t altitude);
#endif // _PACKET_TYPES_H

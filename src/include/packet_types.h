#ifndef _PACKET_TYPES_H
#define _PACKET_TYPES_H

#include <stdbool.h>
#include <stdint.h>

/* Possible devices from which a packet could originate from. */
typedef enum device_address {
    GROUNDSTATION = 0x0,
    ROCKET = 0x1,
    MULTICAST = 0xF,
} DeviceAddress;

/* Possible types of radio packet blocks that could be sent. */
typedef enum block_type {
    TYPE_CTRL = 0x0,
    TYPE_CMD = 0x1,
    TYPE_DATA = 0x2,
} BlockType;

/* Possible sub-types of control blocks that can be sent. */
typedef enum control_block_type {
    CTRL_SIGNAL_REPORT = 0x0,
    CTRL_CMD_ACK = 0x1,
    CTRL_CMD_NONCE_RQST = 0x2,
    CTRL_CMD_NONCE = 0x3,
    CTRL_BEACON = 0x4,
    CTRL_BEACON_RESPONSE = 0x5,
} CtrlBlockType;

typedef enum command_block_type {
    CMD_RST_ROCKET_AV = 0X0,
    CMD_RQST_TELEM_DATA = 0X1,
    CMD_DEPLOY_CHUTE = 0X2,
    CMD_TARE_SENSORS = 0X3,
} CmdBlockType;

typedef enum data_block_type {
    DATA_DBG_MSG = 0x0,
    DATA_STATUS = 0x1,
    DATA_STARTUP_MSG = 0x2,
    DATA_ALT = 0x3,
    DATA_ACCEL = 0x3,
    DATA_ANGULAR_VEL = 0x4,
    DATA_GNSS_LOC = 0x5,
    DATA_GNSS_META = 0x6,
    DATA_PWR_INFO = 0x7,
    DATA_TEMP = 0x8,
    DATA_MPU9250_IMU = 0x9,
    DATA_KX134_1211_ACCEL = 0xA,
} DataBlockType;

/* Allow any block sub-type from DataBlockType, CtrlBlockType or CmdBlockType. */
typedef uint8_t BlockSubtype;

/* Each radio packet will have a header in this format. Any attribute labelled as dead space should be zero filled. */
typedef union packet_header {
    uint8_t bytes[12]; // Each packet header is 12 bytes
    uint32_t words[3]; // Each packet header is 3 32-bit words
    struct {
        uint8_t callsign[6]; // Amateur radio call sign of the operator.
        uint8_t length : 6;  // The length of the packet in bytes (including the header).
        uint8_t version : 5; // The version of the radio packet format being used.
        uint8_t _dead_space : 5;
        DeviceAddress src_addr : 4;  // The source address of the packet.
        uint16_t packet_number : 12; // Which packet number the packet is in the stream being sent over radio.
        uint16_t _dead_space_2 : 16;
    } __attribute__((packed, aligned(1))) contents;
} PacketHeader;

/* Casts the 8-bit ASCII call sign value to a null-terminated string. */
#define packet_callsign(p) ((char *)p.contents.callsign)

void packet_header_init(PacketHeader *p, const char *callsign, const uint8_t length, const uint8_t version,
                        const DeviceAddress source, const uint16_t packet_number);

/* Each block in the radio packet will have a header in this format. Any attribute labelled as dead space should be
 * zero-filled.
 */
typedef union block_header {
    uint8_t bytes[4];
    uint32_t words[1];
    struct {
        uint8_t length : 5;       // The length of the block in bytes.
        bool has_sig : 1;         // Whether or not the block has a cryptographic signature.
        BlockType type : 4;       // The type of block.
        BlockSubtype subtype : 6; // The block type's subtype.
        DeviceAddress dest : 4;   // The device address of the destination.
        uint16_t _dead_space : 12;
    } __attribute__((packed, aligned(1))) contents;
} BlockHeader;

void block_header_init(BlockHeader *b, const uint8_t length, const bool has_sig, const BlockType type,
                       const BlockSubtype subtype, const DeviceAddress dest);

#endif // _PACKET_TYPES_H

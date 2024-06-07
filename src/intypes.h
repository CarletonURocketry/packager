/**
 * @file intypes.h
 * @brief The input data types that packager can receive from fetcher.
 */

#ifndef _INTYPES_H_
#define _INTYPES_H_

#include <stdint.h>

/** Type for a 2 dimensional vector with x, y components. */
typedef struct {
    /** X component. */
    float x;
    /** Y component. */
    float y;
} vec2d_t;

/** Type for a 2 dimensional vector with integer x, y components */
typedef struct {
    /** X component */
    int32_t x;
    /** Y component */
    int32_t y;
} vec2d_i32_t;

/** Type for a 3 dimensional vector with x, y, z components. */
typedef struct {
    /** X component. */
    float x;
    /** Y component. */
    float y;
    /** Z component. */
    float z;
} vec3d_t;

/** Describes possible data types that fetcher is capable of producing. */
typedef enum {
    TAG_TEMPERATURE = 0x0,      /**< Temperature in degrees Celsius */
    TAG_PRESSURE = 0x1,         /**< Pressure in kilo Pascals */
    TAG_HUMIDITY = 0x2,         /**< Humidity in % relative humidity */
    TAG_TIME = 0x3,             /**< Time in milliseconds */
    TAG_ALTITUDE_SEA = 0x4,     /**< Altitude above sea level in meters */
    TAG_ALTITUDE_REL = 0x5,     /**< Altitude above launch height in meters */
    TAG_ANGULAR_VEL = 0x6,      /**< Angular velocity in degrees per second */
    TAG_LINEAR_ACCEL_REL = 0x7, /**< Relative linear acceleration in meters per second squared */
    TAG_LINEAR_ACCEL_ABS = 0x8, /**< Absolute linear acceleration in meters per second squared */
    TAG_COORDS = 0x9,           /**< Latitude and longitude in degrees */
    TAG_VOLTAGE = 0xa,          /**< Voltage in volts with a unique ID. */
    TAG_FIX = 0xb,              /**< Fix type representing the type of fix a gps has */
} SensorTag;

/** Describes a message that can be sent on a message queue and recognized by both fetcher and packager */
typedef struct {
    uint8_t type; /**< Measurement type */
    union {
        float FLOAT;
        uint32_t U32;
        uint16_t U16;
        uint8_t U8;
        int32_t I32;
        int16_t I16;
        int8_t I8;
        vec3d_t VEC3D;
        vec2d_i32_t VEC2D_I32;
        vec2d_t VEC2D;
    } data; /**< The way the contents of this struct should be interpreted */
} common_t;

#endif // _INTYPES_H_

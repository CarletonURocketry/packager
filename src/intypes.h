/**
 * @file intypes.h
 * @brief The input data types that packager can receive from fetcher.
 */

#ifndef _INTYPES_H_
#define _INTYPES_H_

/** Type for a 2 dimensional vector with x, y components. */
typedef struct {
    /** X component. */
    float x;
    /** Y component. */
    float y;
} vec2d_t;

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
    TAG_VOLTAGE = 0x10,         /**< Voltage in volts with a unique ID. */
} SensorTag;

#endif // _INTYPES_H_

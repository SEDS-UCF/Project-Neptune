/* sensor_types contains shared definitions for sensor measurements.
* Each measurement groups its values, time stamp, and validity flag.
*/
#ifndef NEPTUNE_SENSOR_TYPES_HPP
#define NEPTUNE_SENSOR_TYPES_HPP

#include <cstdint>

// This identifies which accelerometer channel produced a measurement.
enum class AccelerometerSource : std::uint8_t {
	Unspecified,
	LowG,
	HighG
};
// One accelerometer sample containing three acceleration components.
struct AccelerometerSample {
	std::uint64_t timestamp_us = 0;

	float x_mps2 = 0.0F;
	float y_mps2 = 0.0F;
	float z_mps2 = 0.0F;

	// The producer will identify the channel that supplied this measurement (LowG or HighG.
	AccelerometerSource source = AccelerometerSource::Unspecified;
	bool valid = false;
};

struct GyroscopeSample {

    // Measurement time in microseconds, using the application's shared clock.
    std::uint64_t timestamp_us = 0;

    // Rotation rate about each axis, in radians per second.
    float x_rad_per_s = 0.0F;
    float y_rad_per_s = 0.0F;
    float z_rad_per_s = 0.0F;

    // Starts invalid until the producer supplies a measurement.
    bool valid = false;
};

/*
 * One barometer measurement containing pressure and temperature.
 */
struct BarometerSample {

    // Measurement time in microseconds, using the application's shared clock.
    std::uint64_t timestamp_us = 0;

    // Absolute pressure, in pascals.
    float pressure_pa = 0.0F;

    // Temperature reported by the barometer, in degrees Celsius.
    float temperature_c = 0.0F;

    // True only when the producer has supplied a usable measurement.
    bool valid = false;
};

/*
 * One GNSS navigation update.
 *
 * Position, altitude, and velocity have separate validity flags.
 */
struct GnssSample {

    // Measurement time in the application's shared clock, not UTC calendar time.
    std::uint64_t timestamp_us = 0;

    // Geographic coordinates in degrees: north and east are positive.
    double latitude_deg = 0.0;
    double longitude_deg = 0.0;

    // Altitude above mean sea level, in meters;
    float altitude_msl_m = 0.0F;

    /*
     * Velocity in meters per second, relative to local north, east, and down.
     * Positive down velocity means descending; negative means ascending.
     */
    float velocity_north_mps = 0.0F;
    float velocity_east_mps = 0.0F;
    float velocity_down_mps = 0.0F;

    // Receiver reported accuracy estimates.
    float horizontal_accuracy_m = 0.0F;
    float vertical_accuracy_m = 0.0F;
    float speed_accuracy_mps = 0.0F;

    // Number of satellites used to calculate the navigation solution.
    std::uint8_t satellites_used = 0;

    /*
     * The driver sets these only after checking the receiver's status
     * and the availability of the corresponding data.
     */
    bool horizontal_position_valid = false;
    bool altitude_valid = false;
    bool velocity_valid = false;
};

#endif

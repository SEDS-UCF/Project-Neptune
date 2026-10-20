#ifndef NEPTUNE_SENSOR_CHECKS_HPP
#define NEPTUNE_SENSOR_CHECKS_HPP

#include <cstdint>
#include "sensor_types.hpp"

/*
 * This function rejects a measurement that the producer marked invalid.
 */
bool is_accelerometer_sample_fresh(const AccelerometerSample& sample, std::uint64_t now_us, std::uint64_t max_age_us);

#endif

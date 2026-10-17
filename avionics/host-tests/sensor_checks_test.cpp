#include <cassert>
#include <iostream>
#include "sensor_checks.hpp"

/*
 * This file tests the accelerometer freshness check using controlled time stamps.
 *
 */
int main()
{
    AccelerometerSample sample{};
    sample.timestamp_us = 1000;
    sample.valid = true;

    assert(is_accelerometer_sample_fresh(sample, 1050, 100) == true);
    assert(is_accelerometer_sample_fresh(sample, 900, 100 ) == false);
    assert(is_accelerometer_sample_fresh(sample, 1100, 100) == true);
    assert(is_accelerometer_sample_fresh(sample, 1101, 100 ) == false);
    sample.valid = false;
    assert(is_accelerometer_sample_fresh(sample, 1050, 100) == false);

    std::cout << "Sample tests passed\n";
    return 0;
}

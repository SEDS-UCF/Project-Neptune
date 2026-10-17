#include "sensor_checks.hpp"

/* This determines whether an accelerometer sample is valid and recent enough.
 * Parameters:
 * sample: the sample measurement to be inspected.
 * now_us: The current time in microseconds.
 * max_age_us: The oldest permitted measurement age in microseconds.
 */
bool is_accelerometer_sample_fresh(const AccelerometerSample& sample, std::uint64_t now_us, std::uint64_t max_age_us) {
	if (sample.valid != true) {
		return false;
	}
	if (sample.timestamp_us > now_us) {
		return false;
	}
	if (max_age_us < now_us - sample.timestamp_us) {
		return false;
	}
	return true;
}

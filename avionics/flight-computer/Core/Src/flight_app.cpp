/*
 * flight_app.cpp
 *
 *
 *
 *  Project Neptune Flight computer application
 *
 *  This provides the application level initialization and coordinates the flight computer's tasks and data flow.
 *
 *  The current implementation:
 *
 *  flight_app_init() creates the measurement queue and acquisition task.
 *  acquisition_task(): This produces time stamped  synthetic accelerometer samples.
 *  flight_app_step(): This receives one queued sample per call and checks the validity, age, and counts it as accepted or rejected.
 *
 *	Synthetic measurements are currently being used while physical sensor integration is being developed.
 *
 *  Created on: Oct 13, 2026
 *  Author: sammy
 */
#include "flight_app.h"
#include "sensor_types.hpp"
#include "sensor_checks.hpp"
#include "cmsis_os2.h"
#include "main.h"

#include <cstdint>

namespace {
	constexpr std::uint32_t queue_capacity = 8; /*The maximum number of measurements waiting in the queue. */
	constexpr std::uint32_t acquisition_delay_ticks = 10; /* This is the wait between producing measurements. The current configured rate is 1000 RTOS ticks per second, 10 is about 10 milliseconds.  */
	constexpr std::uint64_t maximum_sample_age_us = 50000; /* This is the maximum accepted measurement age. 50ms */

	osMessageQueueId_t sample_queue = nullptr; /* This is the identifier used to access the measurement queue.  */

	 /* Counter for debugger inspection where each counter is updated by only one task. */
	volatile std::uint32_t samples_queued = 0; /*Successfully added to the queue */
	volatile std::uint32_t samples_dropped = 0; /*Discarded due to the queue being full */
	volatile std::uint32_t samples_accepted = 0; /*Received and passed the checks */
	volatile std::uint32_t samples_rejected = 0; /*Received but failed the checks */

	/* This function returns the RTOS tick count in microseconds. The RTOS configured tick rate is 1000 ticks per second. We multiply by 1000 to represent it in microseconds. */
	std::uint64_t rtos_time_us() {
		 return static_cast<std::uint64_t>(osKernelGetTickCount()) * 1000ULL;
	}

	/*This produces timestamped accelerometer samples  then sends them to the processing task through the measurement queue.*/
	void acquisition_task(void *) {
		/* This for loop keeps producing measurement while the task is running.*/
		for (;;) {
			AccelerometerSample sample{};
			sample.timestamp_us = rtos_time_us();
			sample.x_mps2 = 1.0F;
			sample.y_mps2 = 2.0F;
			sample.z_mps2 = 3.0F;
			sample.valid = true;

			// Copies the sample into the queue. The last zero means do not wait for space if the queue is full.
			const osStatus_t result = osMessageQueuePut(sample_queue, &sample, 0, 0);

			if (result == osOK) {
				//Successfully  queued
				++samples_queued;
			}
			else if (result == osErrorResource) {
				//Queue is full, discard this new sample and count the loss.
				++samples_dropped;
			}
			else {
				//Unexpected error, invoke the error handlers.
				Error_Handler();
			}
			//Wait 10 RTOS ticks before producing the next sample. Other tasks can run while this waits.
			if(osDelay(acquisition_delay_ticks) != osOK) {
				Error_Handler();
			}
		}
	}

}

/*
 * Set up the applications measurement queue and acquisition task.
 * This is called once by the default RTOS task, before its processing loop.
 */
void flight_app_init(void)
{
	// osKernalGetTickFreq asks the RTOS how many ticks are configured per second.
	// rtos_time_us() assumes 1000 ticks per second, meaning each tick is 1000 microseconds. If the configured frequency is different we invoke the error handler.
	if (osKernelGetTickFreq() != 1000U) {
		Error_Handler();
	}

	// Create a queue that holes up to the queue_capacity measurements. Each entry contains one complete AccelerometeSample. nullptr specifies the default queue attributes.
	sample_queue = osMessageQueueNew(queue_capacity, sizeof(AccelerometerSample), nullptr);

	// If the measurement queue was not created, invoke the error handler and stop initialization.
	if (sample_queue == nullptr)
	{
		Error_Handler();
	}

	//This initializes the task attributes before assigning the values we need.
	osThreadAttr_t attributes{};
	attributes.name = "acquisition";

	//  This sets the acquisition task priority so that it runs ahead of the default processing task, if both are ready.
	attributes.priority = osPriorityAboveNormal;

	// This reserves the 2,048 bytes of stack for local variables and function calls.
	attributes.stack_size = 2048;

	// This will create the acquisition task after its queue is ready. A null return value means the task creation failed.
	if (osThreadNew(acquisition_task, nullptr, &attributes) == nullptr)
	{
		Error_Handler();
	}
}

/*
 * This receives one queued accelerometer sample and checks whether it is usable.
 * It counts it as accepted or rejected based on its validity and its age.
 * The default RTOS task calls this function repeatedly.
 * */
void flight_app_step(void) {

	// Local variable to hold the measurement received from the queue
	AccelerometerSample sample {};

	// osMessageQueueGet receives and removes on measurement from the queue.
	const osStatus_t result = osMessageQueueGet(sample_queue, &sample, nullptr, osWaitForever);

	// Checks if the receive operation failed.
	if (result != osOK) {
		Error_Handler();
		return;
	}
	// This checks if the producer marked the measurement as valid, its timestamp isn't in the future and its age does not exceed the allowed limit.
	if(is_accelerometer_sample_fresh(sample, rtos_time_us(), maximum_sample_age_us))
	{
		++samples_accepted;
	} else
	{
		++samples_rejected;
	}
}




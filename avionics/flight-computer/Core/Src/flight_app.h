/*
 * This file serves as the connection between the generated C code and the C++ flight application
 *
 * The header file declares two functions:
 * flight_app_init(): This sets up the application's resources once.
 * flight_app_step(): This receives and process sensor samples. It currently processes one queued accelerometer sample.
 */

#ifndef FLIGHT_APP_H
#define FLIGHT_APP_H

/*
 * When compiling the C++, use C compatible function names so the generated C code can call these functions.
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Create the measurements queue and start the acquisition task.
 * This is called once by the default RTOS task, before the processing loop.
 */
void flight_app_init(void);

/*
 * Receives queued measurements, checks its validity, age and count it as accepted or rejected.
 * This is called repeatedly by the default RTOS task.
 * If the queue is empty, wait while other tasks can run.
 */
void flight_app_step(void);
#ifdef __cplusplus
}
#endif

#endif

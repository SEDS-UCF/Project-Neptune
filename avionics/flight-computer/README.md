# Flight Computer

## Purpose

This project contains the Project Neptune flight computer application. The bench development uses the NUCLEO-H5E5ZJ board.

STM32CubeMX-generated initialization and board-support code are in C. Handwritten application code uses C++.

## Development Tools

The current project uses:

- STM32CubeIDE 2.2.0
- STM32CubeMX 6.18.1
- STM32CubeH5 firmware package 1.7.0
- The ARM compiler bundled with STM32CubeIDE
- X-CUBE-FREERTOS, providing FreeRTOS with the CMSIS-RTOS2 API

STM32CubeIDE is used to build and debug the firmware. STM32CubeMX is used to configure peripherals and regenerate initialization code.

## Building

1. Clone the repository.
2. Open STM32CubeIDE.
3. Use File > Import > General > Existing Projects into Workspace to import the existing project named `neptune_avionics_bench`.
4. Leave "Copy projects into workspace" unchecked so the project remains in your repository clone.
5. Select the Debug configuration and build the project.

When editing generated code, place handwritten additions inside the designated USER CODE sections. Also keep the "Keep User Code when re-generating" enabled in CubeMX.

## Current Status

The application uses FreeRTOS tasks and a fixed size measurement queue. 

- `flight_app_init()` runs once from the default RTOS task and creates the measurement queue and acquisition task.
- The acquisition task generates timestamped synthetic accelerometer measurements and sends them to the queue.
- The default RTOS task repeatedly calls `flight_app_step()`, which receives one measurement and checks its validity flag and age.



## Integration Notes

Start with the [setup guide](../../StartHere.md) and [sensor driver list](../../docs/sensor-drivers.md). Our first device is the LSM6DSV320X IMU, using the proposed SPI4 connection. Its STM32 functions need timeouts and error reporting.

Keep generated code in `Core`, ST support in `Drivers` and imported drivers in `ThirdParty`. Our own code can go in `Core/Inc/neptune` and `Core/Src/neptune`. Architecture and connection notes go in the repo's `docs` folder. Sensor communication still needs hardware testing.
# Flight computer

This is the STM32 flight computer project for the NUCLEO-H5E5ZJ. Open this folder in STM32CubeIDE as `neptune_avionics_bench`.

Start with the [setup guide](../../StartHere.md) and [sensor driver list](../../docs/sensor-drivers.md). Our first device is the LSM6DSV320X IMU, using the proposed SPI4 connection. Its STM32 functions need timeouts and error reporting.

Keep generated code in `Core`, ST support in `Drivers` and imported drivers in `ThirdParty`. Our own code can go in `Core/Inc/neptune` and `Core/Src/neptune`. Architecture and connection notes go in the repo's `docs` folder. Sensor communication still needs hardware testing.

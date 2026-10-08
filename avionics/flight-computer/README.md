# Flight computer

This is the STM32 flight computer project. Open this folder in STM32CubeIDE as `neptune_avionics_bench`. We're using the NUCLEO-H5E5ZJ for development.

We're starting with the LSM6DSV320X IMU. SPI4 is the proposed connection. First, check the pin assignments in CubeMX and generate the matching code. Then add ST's driver and the functions that connect it to STM32 HAL. Transfers need a timeout and must report errors.

Keep generated code in `Core` and ST support files in `Drivers`. Put imported sensor drivers in `ThirdParty`, with their licenses and source versions. Our own headers and source files can go in `Core/Inc/neptune` and `Core/Src/neptune`.

For now, get the project compiling one step at a time. Actual sensor communication still needs hardware testing. Keep the architecture and connection notes in `docs` at the repository root, and mark proposals clearly.

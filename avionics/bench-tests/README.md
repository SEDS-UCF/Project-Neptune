# H533RE bench tests

Open `neptune_h533re_bench.ioc` in STM32CubeMX for the NUCLEO-H533RE bench board. It is a minimal STM32H533RET6 configuration saved with CubeMX 6.18.1, with STM32CubeIDE as the target toolchain. Board peripherals, sensor buses and test firmware still need to be configured.

The empty IMU, barometer, GNSS, LoRa and storage folders are for the planned device tests. Each contains only a `.gitkeep` file so Git retains the folder. The [driver list](../docs/sensor-drivers.md) contains the existing source links; no device drivers or tests have been implemented here.

The existing H5E5 configuration remains at [flight-computer/neptune_avionics_bench.ioc](../flight-computer/neptune_avionics_bench.ioc) for reference. Its pin assignments need checking and adapting for the H533RE.

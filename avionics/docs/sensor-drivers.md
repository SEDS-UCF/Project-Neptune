# Sensor driver sources

These links come from Sensors and PCB Parts. For now, this page lists the sources. The driver code still needs to be added and tested on STM32.

| Device | Source and files | Work needed for STM32 |
| --- | --- | --- |
| LSM6DSV320X IMU | [ST driver](https://github.com/STMicroelectronics/lsm6dsv320x-pid): `lsm6dsv320x_reg.c` and `.h` | Connect its read/write functions to the proposed SPI4 bus, handle chip-select and configure the sensor.
| MS5803-01BA barometer | [MS5803_01](https://github.com/millerlp/MS5803_01): `MS5803_01.cpp` and `.h` | Adapt the Arduino Wire calls to STM32, or write the commands from TE's datasheet. Check pressure compensation and the calibration-data checksum (PROM CRC).
| MAX-M10S GNSS | [PX4 GPS drivers](https://github.com/PX4/PX4-GPSDrivers): `ubx.cpp`, `ubx.h` and their supporting files | This is our C++ driver option. Add UART and timing functions and the data definitions it needs. Give reception a timeout so other work can continue.
| LR1121 radio | [Semtech SWDR001](https://github.com/Lora-net/SWDR001/tree/master/src): LR11xx `.c/.h` files and `lr11xx_hal.h` | Add SPI, reset, BUSY and interrupt handling. Set the clock and RF options to match our circuit. |
| XTSD04GLGEAG storage | [ST SD-over-SPI reference](https://github.com/STMicroelectronics/STM32CubeG4/blob/master/Drivers/BSP/Adafruit_Shield/adafruit_802_sd.c): `adafruit_802_sd.c`, `.h` and board support | Adapt the G4 board/bus code to H5E5 and add transfer/write timeouts. XTSD communicates using SD commands over SPI.

The [MS5611 driver](https://github.com/Zakrzewiaczek/ms5611-stm32) is for our fallback barometer. MS5803 is our current choice. The document also lists [SparkFun GNSS v3](https://github.com/sparkfun/SparkFun_u-blox_GNSS_v3) for Arduino and [u-blox ubxlib](https://github.com/u-blox/ubxlib) as other GNSS options.

These are the source versions and licenses checked.

| Source | Reviewed revision | License |
| --- | --- | --- |
| ST IMU | [7fdcb10d2fe2](https://github.com/STMicroelectronics/lsm6dsv320x-pid/tree/7fdcb10d2fe2195635413c95b4a2fb59642c466b) | BSD-3-Clause |
| MS5803 reference | [c63d5ddaad39](https://github.com/millerlp/MS5803_01/tree/c63d5ddaad39038d407ce870dc0aff1184529421) | GPL-3.0 |
| PX4 GNSS | [f6f5c2d89a30](https://github.com/PX4/PX4-GPSDrivers/tree/f6f5c2d89a30f779694f1a429baea3de07882e2c) | BSD-3-Clause |
| Semtech radio | [a333238acfa0](https://github.com/Lora-net/SWDR001/tree/a333238acfa0a9dee9ce2824ce52e89b98f3d24b) | Clear BSD |
| ST storage reference | [140db648b016](https://github.com/STMicroelectronics/STM32CubeG4/tree/140db648b016ba49b05ca790b5b66e3477807839) | BSD-3-Clause for the referenced component |

Keep the license and source version with each driver we add. The MS5803 reference uses GPL-3.0 and has possible arithmetic problems noted in the earlier review. Its calculations still need checking on STM32. We'll start with the IMU and add the other devices one at a time.

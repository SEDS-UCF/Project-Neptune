# Sensor driver sources

These are the driver sources listed in Sensors and PCB Parts, with the current MS5803 reference from our earlier review. No sensor driver code is imported by this change, and communication with the hardware still needs testing.

| Device | Source and files | Work needed for STM32 |
| --- | --- | --- |
| LSM6DSV320X IMU | [ST driver](https://github.com/STMicroelectronics/lsm6dsv320x-pid): `lsm6dsv320x_reg.c` and `.h` | Connect the read/write callbacks to the proposed SPI4 bus, handle chip-select and configure the sensor. |
| MS5803-01BA barometer | [MS5803_01](https://github.com/millerlp/MS5803_01): `MS5803_01.cpp` and `.h` | Arduino reference, not a ready STM32 driver. Replace Wire calls or implement the TE datasheet commands. Check compensation and PROM CRC. |
| MAX-M10S GNSS | [PX4 GPS drivers](https://github.com/PX4/PX4-GPSDrivers): `ubx.cpp`, `ubx.h` and their supporting files | Candidate C++ integration. Supply UART, time and data definitions; keep reception from blocking other work. |
| LR1121 radio | [Semtech SWDR001](https://github.com/Lora-net/SWDR001/tree/master/src): LR11xx `.c/.h` files and `lr11xx_hal.h` | Supply SPI, reset, BUSY and interrupt handling. Match the clock and RF configuration to our actual circuit. |
| XTSD04GLGEAG storage | [ST SD-over-SPI reference](https://github.com/STMicroelectronics/STM32CubeG4/blob/master/Drivers/BSP/Adafruit_Shield/adafruit_802_sd.c): `adafruit_802_sd.c`, `.h` and board support | Adapt the G4 board/bus code to H5E5 and bound transfer/write waits. XTSD uses SD commands, not raw NAND commands. |

The document also lists the [MS5611 driver](https://github.com/Zakrzewiaczek/ms5611-stm32). Keep that as a fallback reference; it is not the driver for our current MS5803. Its other GNSS links are [SparkFun GNSS v3](https://github.com/sparkfun/SparkFun_u-blox_GNSS_v3), an Arduino option, and [u-blox ubxlib](https://github.com/u-blox/ubxlib). They are not additional drivers we are importing now.

Earlier reviewed revisions and licenses are below. These identify the references reviewed on 3 October 2026, not a completed team firmware integration.

| Source | Reviewed revision | License |
| --- | --- | --- |
| ST IMU | [7fdcb10d2fe2](https://github.com/STMicroelectronics/lsm6dsv320x-pid/tree/7fdcb10d2fe2195635413c95b4a2fb59642c466b) | BSD-3-Clause |
| MS5803 reference | [c63d5ddaad39](https://github.com/millerlp/MS5803_01/tree/c63d5ddaad39038d407ce870dc0aff1184529421) | GPL-3.0 |
| PX4 GNSS | [f6f5c2d89a30](https://github.com/PX4/PX4-GPSDrivers/tree/f6f5c2d89a30f779694f1a429baea3de07882e2c) | BSD-3-Clause |
| Semtech radio | [a333238acfa0](https://github.com/Lora-net/SWDR001/tree/a333238acfa0a9dee9ce2824ce52e89b98f3d24b) | Clear BSD |
| ST storage reference | [140db648b016](https://github.com/STMicroelectronics/STM32CubeG4/tree/140db648b016ba49b05ca790b5b66e3477807839) | BSD-3-Clause for the referenced component |

Keep licenses and source versions with any code we import. The MS5803 reference has GPL terms and known arithmetic concerns from the earlier review; do not treat it as a verified H5 driver. We will start with the IMU and add the other devices one at a time.

# H533RE bench tests

Includes the H533RE CubeIDE project, CubeMX file and test folders for the IMU, barometer, GNSS, LoRa and storage.

| Test folder | Breakout board | Chip | Technical documentation |
| --- | --- | --- | --- |
| `imu` | ST STEVAL-MKI251A | LSM6DSV320X | [Board schematic](https://www.st.com/resource/en/schematic_pack/steval-mki251a-schematic.pdf), [datasheet](https://www.st.com/resource/en/datasheet/lsm6dsv320x.pdf) |
| `barometer` | Rakstore MS5803-01BA breakout (B09LY4BG1Z) | MS5803-01BA | [TE sensor datasheet](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocLang=English&DocNm=MS5803-01BA&DocType=Data+Sheet) |
| `gnss` | SparkFun MAX-M10S Qwiic (GPS-18037) | MAX-M10S | [Board guide and schematic](https://docs.sparkfun.com/SparkFun_u-blox_MAX-M10S/), [datasheet](https://content.u-blox.com/sites/default/files/MAX-M10S_DataSheet_UBX-20035208.pdf) |
| `lora` | Waveshare Core1121-HF | LR1121 | [Board guide](https://www.waveshare.com/wiki/Core1121-XF), [datasheet](https://files.waveshare.com/wiki/Core1121/LR1121_H2_DS_v2_0.pdf) |
| `storage` | Adafruit SPI Flash SD Card, 512 MB (4899) | XTSD04GLGEAG | [Board schematic and guide](https://learn.adafruit.com/adafruit-spi-flash-sd-card/downloads), [datasheet](https://cdn-shop.adafruit.com/product-files/4899/2005251034_XTX-XTSD04GLGEAG_C558839%282%29.pdf) |

Import the `STM32CubeIDE` folder into STM32CubeIDE. Start editing in `Core/Src/main.c`.

Open `neptune_h533re_bench.ioc` in STM32CubeMX for hardware configuration.

[Development workflow](../../StartHere.md#development-workflow) | [Driver references](../docs/sensor-drivers.md) | [H5E5 configuration](../flight-computer/neptune_avionics_bench.ioc)

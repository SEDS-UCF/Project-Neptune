# H533RE bench tests

Includes the H533RE CubeIDE project, CubeMX file and test folders for the IMU, barometer, GNSS, LoRa and storage.

| Test folder | Breakout board | Chip |
| --- | --- | --- |
| `imu` | [ST STEVAL-MKI251A](https://www.st.com/en/evaluation-tools/steval-mki251a.html) | LSM6DSV320X |
| `barometer` | [Rakstore MS5803-01BA breakout (B09LY4BG1Z)](https://www.amazon.com/dp/B09LY4BG1Z) | MS5803-01BA |
| `gnss` | [SparkFun MAX-M10S Qwiic (GPS-18037)](https://www.sparkfun.com/products/18037) | MAX-M10S |
| `lora` | [Waveshare Core1121-HF](https://www.waveshare.com/product/core1121-xf.htm) | LR1121 |
| `storage` | [Adafruit SPI Flash SD Card, 512 MB (4899)](https://www.adafruit.com/product/4899) | XTSD04GLGEAG |

Import the `STM32CubeIDE` folder into STM32CubeIDE. Start editing in `Core/Src/main.c`.

Open `neptune_h533re_bench.ioc` in STM32CubeMX for hardware configuration.

[Driver references](../docs/sensor-drivers.md) | [H5E5 configuration](../flight-computer/neptune_avionics_bench.ioc)

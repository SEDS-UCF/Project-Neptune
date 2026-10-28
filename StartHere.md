# Start here

This is the shared Project Neptune repo. Start with your subteam's folder and keep your work in one local copy.

| Folder | What goes here |
| --- | --- |
| `avionics/` | Flight computer and power system firmware, hardware and tests. |
| `ground-systems/` | Telemetry, ground displays and launch control. |
| `test-systems/` | Test stand and strand burner software. |
| `simulation/` | Flight models and vehicle parameters. |
| `docs/` | Architecture, connections and setup notes. |

## Flight computer

Our plan is to read the sensors, estimate the rocket's motion, save the data and send telemetry to the ground. We're starting with the IMU and adding the other devices one at a time.

Open `avionics/flight-computer` in STM32CubeIDE as `neptune_avionics_bench`. We're using the NUCLEO-H5E5ZJ for development. The [driver list](avionics/docs/sensor-drivers.md) has the source links and the work each device needs.

1. Use GitHub Desktop to update your clone and make a branch. If you're contributing through a fork, sync it with the team repo first.
2. Build the existing project. Then review the IMU connections in its `.ioc`, configure the proposed SPI4 bus in CubeMX and check the generated changes.
3. Add ST's IMU driver and the small functions that connect it to STM32 HAL. Build again, then test the sensor when the hardware is available. Record the version and what passed.
4. Review the changed files in GitHub Desktop, commit, push and open a pull request to the team repo.

CubeIDE and GitHub Desktop use the same local files. Keep generated code in `Core`, ST support in `Drivers` and imported sensor drivers in `ThirdParty`. Our own device code can go in `Core/Inc/neptune` and `Core/Src/neptune`. Keep each driver's license and source version with it.

The driver code and sensor tests are still ahead of us. SPI4 is the current IMU proposal; we need to check it against CubeMX and the schematic as we set it up.

# Start here

This is the shared Project Neptune repo. Start with your subteam's folder and keep your work in one local copy.

| Folder | What goes here |
| --- | --- |
| `avionics/` | Flight computer and power system firmware, hardware and tests. |
| `ground-systems/` | Telemetry, ground displays and launch control. |
| `test-systems/` | Test stand and strand burner software. |
| `simulation/` | Flight models and vehicle parameters. |
| `docs/` | Architecture, connections and setup notes. |

## Firmware projects

| Project | Board | Folder to import into CubeIDE | Project name |
| --- | --- | --- | --- |
| Breakout bench tests | NUCLEO-H533RE | `avionics/bench-tests/STM32CubeIDE` | `neptune_h533re_bench` |
| Flight computer | NUCLEO-H5E5ZJ | `avionics/flight-computer` | `neptune_avionics_bench` |

See the [bench README](avionics/bench-tests/README.md) for breakout boards and technical references, or the [flight computer README](avionics/flight-computer/README.md) for its tools and application.

## Development workflow

1. Clone the repository once. For fork contributions, clone your fork and keep it synced with the team repository. Update your local branch before starting, then create a branch for your change.
2. In CubeIDE, use **File > Import > General > Existing Projects into Workspace** and select the project folder above. Leave **Copy projects into workspace** unchecked so CubeIDE edits the files in your Git clone.
3. Build the existing project before changing firmware. These projects currently use CubeIDE's managed Make build.
4. Configure pins, clocks and peripherals in the project's `.ioc` using CubeMX. Enable **Keep User Code when re-generating**, generate the code and review the changed files. The `.ioc` determines the generated peripheral setup and HAL module selection.
5. Add your device or test code in small modules. Make sure new source files and folders are included in the CubeIDE build.
6. For firmware changes, build Debug and Release. Run the relevant board tests when hardware is available, and record what you tested.
7. Review the diff, commit and push your branch. Open a pull request into the team repository and describe the change and its validation.

## Code layout

- `Core`: generated initialization and our application modules. Keep application additions to generated C files inside their `USER CODE` sections.
- `Drivers`: supplied ST HAL and Arm CMSIS support.
- `ThirdParty`: imported device drivers, with their license and source revision. See the [sensor driver list](avionics/docs/sensor-drivers.md).
- Our device modules can use `Core/Inc/neptune` and `Core/Src/neptune`. Keep `main.c` short and call those modules from it.

Keep the `.ioc`, source files, linker scripts and IDE project files in Git. Build folders and local workspace metadata stay excluded by `.gitignore`.

## Git tools

GitHub Desktop, terminal Git and CubeIDE with the optional EGit plugin can work with the same clone. Choose the interface you prefer; it does not change how the firmware builds.

EGit provides Git Staging, commit, pull and push actions inside CubeIDE. See the [EGit guide](https://help.eclipse.org/latest/topic/org.eclipse.egit.doc/help/EGit/User_Guide/Tasks.html).

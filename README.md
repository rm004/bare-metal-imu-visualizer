# Bare Metal IMU Visualizer (Work in progress)
This repository tracks the code of a Bare Metal IMU Visualizer as I write it from scratch. The goal is to build professional STM32F446RE bare metal drivers for GPIO, I2C, UART, and the MPU6050. Then to use those drivers to get the orientation of the IMU, send it to a PC, and visualize the orientation in a 3D python program.

## Build
I have setup the project to build using the `arm-gnu-toolchain`, `make` and `openocd`. Therefore, those dependenies must be installed in order build and flash this project. Here is a list of the available `make` commands:

| Command        | Description                                                                         |
|----------------|-------------------------------------------------------------------------------------|
| make           | Compiles and links source files into executable located at `build/bin/`             |
| make clean     | Removes the `build/` directory which contains object files and the final executable |
| make flash     | Builds the executable if not built already and flashes it to the connected MCU      |
| make cppcheck  | Runs static analysis on all of source files                                         |
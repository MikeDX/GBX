# GBX

GBX is a gameboy emulator written in C using the SDL2 library. It was created by [MikeDX](https://github.com/MikeDX) and [Assistant](https://github.com/openai/assistant).

[![Build Status](https://github.com/MikeDX/GBX/actions/workflows/cmake.yml/badge.svg)](https://github.com/MikeDX/GBX/actions)

The project is a work-in-progress, but the goal is to create a fully-featured emulator that can run gameboy and gameboy color games on modern computers. It includes code for loading the ROM, initializing the emulator, and running the emulator in a loop.

To build and run the emulator, you will need to install the SDL2 library on your system. You can then run `cmake .` to generate the build files, and `make` to build the emulator. You can then run `./gbx <rom_file>` to run a gameboy ROM file.

We hope you enjoy using GBX, and we welcome any contributions or feedback to improve the emulator.

## License

GBX is released under the MIT license. See [LICENSE](LICENSE) for details.

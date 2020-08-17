# Bit Matrix
### Raspberry Pi Dot Matrix Display Controller

- Controls a configuration of one or multiple serial chains of [MAX7219](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf) driven [dot matrix displays](https://www.aliexpress.com/item/4000080461908.html).
- Individual display modules can be mapped freely onto an image canvas.
- Supports rendering of 8x8 bit fonts.

## Prerequisites

### Build Tools
Building BitMatrix requires a C++ tool chain, GIT and CMake. To install them, enter
```
sudo apt install build-essential git
```

Also, install CMake via [Snap](https://snapcraft.io/install/cmake/raspbian).

### WiringPi
Bit Matrix uses the [WiringPi GPIO library](https://wiringpi.com). The library comes pre-installed with the full Raspberry Pi OS. If you're using Raspberry Pi OS Lite, you need to install it manually:

```
sudo apt install wiringpi
```

## Installation and Build

Clone the Github repository:
```
git clone https://github.com/framefactory/bit-matrix
```

Build using CMake:

```
cd bit-matrix
mkdir build
cd build
cmake ..
make
```

The executable can be found in `bit-matrix/bin` and started like so:

```
./bin/BitMatrix
```
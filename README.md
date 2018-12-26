# armcan

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

##Firmware/Library Versions
* ugfx v2.8
* STM HAL v1.12
* FreeRTOS v9

## Prerequisites

### ARM
* gcc-arm-none-eabi-*
* make
* st-util

### Linux
* libsdl2-dev
* build-essential
* gcc-multilib


#### Only for Ubuntu 18.04:
Currently libsdl2-dev is broken on 18.04. Follow this guide to downgrade libsdl2-dev ([source](https://www.gog.com/forum/xenonauts/ubuntu_1804_lts_bionic_beaver_dbus_error_solution))
```
1a. Remove current version

sudo apt remove --purge libsdl2-2.0-0:i386

1b. Remove Cross Dependencies (optional)

sudo apt autoremove

2. Add Xenial repo

sudo apt-add-repository "deb http://archive.ubuntu.com/ubuntu/ xenial main restricted universe"

3. Update

sudo apt update

4. Install xenial version

sudo apt install libsdl2-dev:i386=2.0.4+dfsg1-2ubuntu2
```

## Building
### ARM

First set ARM GCC toolchain path in Makefile (ARCH=...), if it was not added to PATH.

If the arm toolchain was installed via the ubuntu apt repository and you are using Ubuntu 18.04 you have to download the toolchain manually.
Currently there is an issue with the sources ([more infos](https://bugs.launchpad.net/gcc-arm-embedded/+bug/1772332)).

Next download ugfx 2.8 from the ugfx.io page and extract the contents into the folder "ugfx", located right in the project folder.


```
make
```

### Linux
```
make -f Makefile_linux
```

##Debug
###ARM
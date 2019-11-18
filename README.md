# Arduino Custom AVR Build and Upload Process


## Description:
This project provides a low-level Arduino compile and upload process using the `avr-gcc` and `avrdude` cli utilities, for a sample of RGB LED examples.


## Details
This project provides a template for building larger, custom Arduino projects which demand a more complex code base than those afforded from a simple **.ino** sketch file.
It also arose from a personal desire to understand what's going on "under-the-hood" of the Arduino IDE.

Note that the **cpp** code in *src* can still be manually reassembled and placed into a **.ino**, if one wishes to use the Ardunio IDE.
This project was build on Linux Mint, but the setup and build process should adapt easily for other Linux distros or Mac.
Additionally, this project is built specifically for the Arduino Uno (Fully Qualified Board Name - arduino:avr:uno);
however it should readily extend to other Arduino boards by appropriately updating the **gcc** and **avrdude** flags.

The entire build and upload process is cataloged in the *Makefile*; it can be run in its entirety by executing `make`.


## Setup
1. Purchase an Arduino Uno (e.g. https://store.arduino.cc/usa/arduino-uno-rev3)
2. Connect the Arduino UNO board to your computer with an appropriate A->B USB cable (e.g. https://store.arduino.cc/usa/usb-2-0-cable-type-a-b)
3. Install the necessary avr utilies for the build and upload process:
```
sudo apt install \
    avr-libc \
    avrdude \
    binutils-avr \
    gcc-avr \
    gdb-avr
```
On Linux, the usb device mount is */dev/ttyACM0*. Note that **avrdude** requires sudo access to write to this device; this can also be accomplished by running `sudo chmod a+rw /dev/ttyACM0`.  Running this command is actually necessary when using the Arduino IDE; unfortunately this permission needs to be granted every time the usb cable is reconnected.


## Arduino Library
The manual build process requires compiling and linking the necessary Arduino source code.  This is accomplished by fetching the latest master branch of https://github.com/arduino/ArduinoCore-avr.  Running `make import-lib` accomplishes this, along with refreshing *src/main.cpp* with the latest ArduinoCore main file, to ensure that the project integrates correctly with the ArduinoCore library.  All necessary ArduinoCore code is then compiled and archived into a single "lib.a" object library file via `make build-lib`.


## Build Process
Running `make build-project` accomplishes:
1. Compiling all project source code from *src*
2. Linking these object files together with the library file described above
3. Converting the outputted "ELF" file into the necessary "hex" file required for uploading to the Arduino


## Upload Process
Running `make deploy` uploads the hex build artifact to the Arduino via the **avrdude** cli utility, using the board-specific configurations.


## Monitoring
The following code allows monitoring the serial output from the Arduino:
```
sudo stty -F /dev/ttyACM0 9600 raw -clocal -echo
sudo cat /dev/ttyACM0
```
Note: deployment fails if this monitoring is active

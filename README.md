# Arduino Custom AVR Dockerized Build and Upload Process


## Description:
This project provides a fully Dockerized "low-level" Arduino compile and upload process using the `avr-gcc` and `avrdude` cli utilities for a sample of RGB LED examples.


## Details
This project provides a template for building larger, custom Arduino projects which demand a more complex code base than those afforded from a simple **.ino** sketch file.
It also arose from a personal desire to understand what's going on "under-the-hood" of the Arduino IDE.

Note that the **cpp** code in *src* can still be manually reassembled and placed into a **.ino**, if one wishes to use the Ardunio IDE.
This project is built specifically for the Arduino Uno (Fully Qualified Board Name - arduino:avr:uno);
however it should readily extend to other Arduino boards by appropriately updating the **gcc** and **avrdude** flags.

The entire build and upload process is cataloged in the *Makefile*; it can be run in its entirety by executing `make`.

This process actually has no strict dependence on Arduino, it only relies on the avr cli utilities.
The moniker "Arduino" was simply designated due to the ecosystem's popularity (i.e. most people using this process will be deploying to an Arduino).


## Build
Running `make build` handles the entire gcc build process in a stable Dockerized environment with the following steps:
1. Downloading the latest Ardunio source code at https://github.com/arduino/ArduinoCore-avr
2. Compiling all necessary Arduino source code files and archiving into a single "lib.a" object library file
3. Compiling all project source code from *src*
4. Linking the library and source code object files
5. Converting the outputted "ELF" file into the necessary "hex" file required for uploading to the Arduino


## Deploy
Running `make deploy` uploads the Dockerized "hex" build artifact to the Arduino via the **avrdude** cli utility, using the board-specific configurations.
(Note that sudo access is necessary to writing to the usb device.  Also upon first connecting the usb cable, the deploy step can fail because the device is "busy": simply wait a few seconds and try again.)


## Monitoring
Running `make monitor` allows listening to the Serial output from the Arduino.
(Note that deployment fails if this monitoring is active)


## Examples
The RGB LED examples are actually somewhat interesting: the main challenge was utilizing analog write PWD functionality correctly to achieve arbitrary light colors, and then more challengely to attain smooth color transitions.

The core RGB LED light code is in *src/utils.cpp*: the two important functions there implement static light colors and smooth color transitioning.
The color multiplexing interval and duty cycle constants defined in *src/color_settings.h* are essential to achieving stable, non-flickering color light output.
Some time was spent experimenting with these values for my device:
you may have to slightly adjust these values until you are satisfied with the light stability.

Note that the "Serial" functionality is disabled for performance reasons, but should be turned on during development: it's quite useful while testing.

The RGB LED functionality is demoed through several "light shows":
* Boring - shows a given color with a specified on and off interval
* Simple Transition - transitions smoothly between two given colors at a specified interval
* Random Transition - transition smoothly between random colors at a specified interval

These shows are setup in *src/shows.cpp* and chosen in the *setup_loop.cpp* entrypoint.
The random is by far the most interesting to me: I actually enjoy staring at it as some sort of mood light :)

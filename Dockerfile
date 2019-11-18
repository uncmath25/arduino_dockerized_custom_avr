FROM ubuntu:18.04
LABEL maintainer="Colton Willig <coltonwillig@gmail.com>"

USER root

RUN apt-get update \
    && apt-get install -yqq \
        avr-libc \
        avrdude \
        binutils-avr \
        gcc-avr \
        gdb-avr \
        wget \
        zip \
    && rm -rf /var/lib/apt/lists/*

ARG PROJECT_DIR=/project
RUN mkdir PROJECT_DIR

ARG CODE_DIR=$PROJECT_DIR/src
ARG LIB_DIR=$PROJECT_DIR/lib
ARG LIB_BUILD_DIR=$LIB_DIR/build
ARG BUILD_DIR=$PROJECT_DIR/build
ARG SCRIPTS_DIR=$PROJECT_DIR/scripts

ARG ARDUINO_LIBRARY_PATH=$LIB_BUILD_DIR/lib.a
ARG ARDUINO_CORE_PATH=arduino_core/cores/arduino
ARG ARDUINO_VARIANT_PATH=arduino_core/variants/standard

ARG ARDUINO_USB_DEVICE=/dev/ttyACM0
ARG BAUD_RATE=115200
ARG AVR_MCU_ARCHITECTURE=atmega328p

ARG GCC_COMPILE_FLAGS="-MMD -mmcu=$AVR_MCU_ARCHITECTURE -Wall -DF_CPU=16000000L -Wall -Os"
ARG GCC_LINKING_FLAGS="-mmcu=$AVR_MCU_ARCHITECTURE -Wl,--gc-sections -Os"
ARG HEX_CONVERSION_FLAGS="-O ihex -R .eeprom"

ARG ARDUINO_CORE_C_FILES="hooks wiring wiring_analog wiring_digital"
ARG ARDUINO_CORE_CPP_FILES="HardwareSerial HardwareSerial0 Print WMath"

WORKDIR $PROJECT_DIR

RUN mkdir $LIB_DIR \
    && wget https://github.com/arduino/ArduinoCore-avr/archive/master.zip -O arduino_core.zip \
        && unzip arduino_core.zip \
        && mv ArduinoCore-avr-master $LIB_DIR/arduino_core \
        && rm arduino_core.zip

RUN rm -rf $LIB_BUILD_DIR \
    && mkdir $LIB_BUILD_DIR \
    && for file in $ARDUINO_CORE_C_FILES; do \
        avr-gcc \
            -c \
            -I $LIB_DIR/$ARDUINO_CORE_PATH \
            -I $LIB_DIR/$ARDUINO_VARIANT_PATH \
            -o $LIB_BUILD_DIR/$file.o \
            $GCC_COMPILE_FLAGS \
            -v \
            $LIB_DIR/$ARDUINO_CORE_PATH/$file.c; \
    done \
    && for file in $ARDUINO_CORE_CPP_FILES; do \
        avr-g++ \
            -c \
            -I $LIB_DIR/$ARDUINO_CORE_PATH \
            -I $LIB_DIR/$ARDUINO_VARIANT_PATH \
            -x c++ \
            -o $LIB_BUILD_DIR/$file.o \
            $GCC_COMPILE_FLAGS \
            -v \
            $LIB_DIR/$ARDUINO_CORE_PATH/$file.cpp; \
    done \
    && avr-ar rcs \
        $ARDUINO_LIBRARY_PATH \
        $LIB_BUILD_DIR/*.o \
        -v

COPY src $CODE_DIR

RUN rm -f $CODE_DIR/main.cpp \
    && cp $LIB_DIR/$ARDUINO_CORE_PATH/main.cpp $CODE_DIR/main.cpp \
    && mkdir $BUILD_DIR \
	&& avr-g++ \
		-c \
		-I $LIB_DIR/$ARDUINO_CORE_PATH \
		-I $LIB_DIR/$ARDUINO_VARIANT_PATH \
		-x c++ \
		$GCC_COMPILE_FLAGS \
		-v \
		$CODE_DIR/*.cpp \
	&& mv *.o $BUILD_DIR \
	&& mv *.d $BUILD_DIR \
	&& avr-gcc \
		-lc -lm \
		$GCC_LINKING_FLAGS \
		-o $BUILD_DIR/main.elf \
		-v \
		$BUILD_DIR/*.o $ARDUINO_LIBRARY_PATH \
	&& avr-objcopy \
		$HEX_CONVERSION_FLAGS \
		-v \
		$BUILD_DIR/main.elf \
		$BUILD_DIR/main.hex

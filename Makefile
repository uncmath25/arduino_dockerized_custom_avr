.PHONY: clean import-lib build-lib build-project deploy

CODE_DIR=src
LIB_DIR=lib
BUILD_DIR=build

ARDUINO_LIBRARY_PATH=$(LIB_DIR)/$(BUILD_DIR)/lib.a
ARDUINO_CORE_PATH=arduino_core/cores/arduino
ARDUINO_VARIANT_PATH=arduino_core/variants/standard

ARDUINO_USB_DEVICE=/dev/ttyACM0
BAUD_RATE=115200
AVR_MCU_ARCHITECTURE=atmega328p

GCC_COMPILE_FLAGS=-MMD -mmcu=$(AVR_MCU_ARCHITECTURE) -Wall -DF_CPU=16000000L -Wall -Os
GCC_LINKING_FLAGS=-mmcu=$(AVR_MCU_ARCHITECTURE) -Wl,--gc-sections -Os
HEX_CONVERSION_FLAGS=-O ihex -R .eeprom

ARDUINO_CORE_C_FILES = hooks wiring wiring_digital wiring_analog

all: clean import-lib build-lib build-project deploy

clean:
	@echo "*** Cleaning repo of build artifacts ***"
	rm -rf $(BUILD_DIR)
	rm -f *.o
	rm -f *.d
	rm -f *.a

import-lib:
	@echo "*** Importing arduino library source code from Github ***"
	rm -rf $(LIB_DIR)
	mkdir $(LIB_DIR)
	wget https://github.com/arduino/ArduinoCore-avr/archive/master.zip -O arduino_core.zip \
		&& unzip arduino_core.zip \
		&& mv ArduinoCore-avr-master $(LIB_DIR)/arduino_core \
		&& rm arduino_core.zip
	rm -f $(CODE_DIR)/main.cpp
	cp $(LIB_DIR)/$(ARDUINO_CORE_PATH)/main.cpp $(CODE_DIR)/main.cpp

build-lib:
	@echo "*** Building arduino library ***"
	rm -rf $(LIB_DIR)/${BUILD_DIR}
	mkdir $(LIB_DIR)/$(BUILD_DIR)
	for file in $(ARDUINO_CORE_C_FILES); do \
		avr-gcc \
			-c \
			-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
			-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
			-o $(LIB_DIR)/$(BUILD_DIR)/$$file.o \
			$(GCC_COMPILE_FLAGS) \
			-v \
			$(LIB_DIR)/$(ARDUINO_CORE_PATH)/$$file.c; \
	done
	avr-ar rcs \
		$(ARDUINO_LIBRARY_PATH) \
		$(LIB_DIR)/$(BUILD_DIR)/*.o \
		-v

build-project: clean
	@echo "*** Building rgb led project ***"
	mkdir $(BUILD_DIR)
	avr-g++ \
		-c \
		-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
		-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
		-I $(CODE_DIR) \
		-x c++ \
		$(GCC_COMPILE_FLAGS) \
		-v \
		$(CODE_DIR)/*.cpp
	mv *.o $(BUILD_DIR)
	mv *.d $(BUILD_DIR)
	avr-gcc \
		-lc -lm \
		$(GCC_LINKING_FLAGS) \
		-o $(BUILD_DIR)/main.elf \
		-v \
		$(BUILD_DIR)/*.o $(ARDUINO_LIBRARY_PATH)
	avr-objcopy \
		$(HEX_CONVERSION_FLAGS) \
		-v \
		$(BUILD_DIR)/main.elf \
		$(BUILD_DIR)/main.hex

deploy:
	@echo "*** Deploying compiled project to Arduino ***"
	sudo avrdude \
		-U flash:w:$(BUILD_DIR)/main.hex \
		-p $(AVR_MCU_ARCHITECTURE) \
		-c arduino \
		-P $(ARDUINO_USB_DEVICE) \
		-b $(BAUD_RATE) \
		-D \
		-v

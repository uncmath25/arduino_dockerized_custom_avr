.PHONY: clean import-lib build-lib build-project deploy

CODE_DIR=src
LIB_DIR=lib
BUILD_DIR=build

ARDUINO_LIBRARY_PATH=$(LIB_DIR)/$(BUILD_DIR)/lib.a
ARDUINO_CORE_PATH=arduino_core/cores/arduino
ARDUINO_VARIANT_PATH=arduino_core/variants/standard

all: clean import-lib build-lib build-project deploy

clean:
	@echo "*** Cleaning repo of build artifacts ***"
	rm -rf $(BUILD_DIR)
	rm -f *.o
	rm -f *.d
	rm -f *.a

import-lib:
	@echo "*** Importing arduino library source code from Github ***"
	rm -rf lib
	mkdir lib
	wget https://github.com/arduino/ArduinoCore-avr/archive/master.zip -O arduino_core.zip \
		&& unzip arduino_core.zip \
		&& mv ArduinoCore-avr-master lib/arduino_core \
		&& rm arduino_core.zip
	rm -f $(CODE_DIR)/main.cpp
	cp $(LIB_DIR)/$(ARDUINO_CORE_PATH)/main.cpp $(CODE_DIR)/main.cpp

build-lib:
	@echo "*** Building arduino library ***"
	rm -rf $(LIB_DIR)/${BUILD_DIR}
	mkdir $(LIB_DIR)/$(BUILD_DIR)
	avr-g++ \
		-c \
		-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
		-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
		-x c \
		-o $(LIB_DIR)/$(BUILD_DIR)/hooks.o \
		-MMD -mmcu=atmega328p -Wall -DF_CPU=16000000L -Wall -Os \
		-v \
		$(LIB_DIR)/$(ARDUINO_CORE_PATH)/hooks.c
	avr-g++ \
		-c \
		-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
		-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
		-x c++ \
		-o $(LIB_DIR)/$(BUILD_DIR)/wiring.o \
		-MMD -mmcu=atmega328p -Wall -DF_CPU=16000000L -Wall -Os \
		-v \
		$(LIB_DIR)/$(ARDUINO_CORE_PATH)/wiring.c
	avr-g++ \
		-c \
		-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
		-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
		-x c++ \
		-o $(LIB_DIR)/$(BUILD_DIR)/wiring_digital.o \
		-MMD -mmcu=atmega328p -Wall -DF_CPU=16000000L -Wall -Os \
		-v \
		$(LIB_DIR)/$(ARDUINO_CORE_PATH)/wiring_digital.c
	avr-g++ \
		-c \
		-I $(LIB_DIR)/$(ARDUINO_CORE_PATH) \
		-I $(LIB_DIR)/$(ARDUINO_VARIANT_PATH) \
		-x c++ \
		-o $(LIB_DIR)/$(BUILD_DIR)/wiring_analog.o \
		-MMD -mmcu=atmega328p -Wall -DF_CPU=16000000L -Wall -Os \
		-v \
		$(LIB_DIR)/$(ARDUINO_CORE_PATH)/wiring_analog.c
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
		-MMD -mmcu=atmega328p -Wall -DF_CPU=16000000L -Wall -Os \
		-v \
		$(CODE_DIR)/*.cpp
	mv *.o $(BUILD_DIR)
	mv *.d $(BUILD_DIR)
	avr-gcc \
		-lc -lm \
		-mmcu=atmega328p -Wl,--gc-sections -Os \
		-o $(BUILD_DIR)/main.elf \
		-v \
		$(BUILD_DIR)/*.o $(ARDUINO_LIBRARY_PATH)
	avr-objcopy \
		-O ihex -R .eeprom \
		-v \
		$(BUILD_DIR)/main.elf \
		$(BUILD_DIR)/main.hex

deploy:
	@echo "*** Deploying compiled project to Arduino ***"
	sudo avrdude \
		-U flash:w:$(BUILD_DIR)/main.hex \
		-p atmega328p \
		-c arduino \
		-P /dev/ttyACM0 \
		-b 115200 \
		-D \
		-v

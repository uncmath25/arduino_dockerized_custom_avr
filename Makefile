.PHONY: build deploy monitor

IMAGE_NAME := uncmath25/arduino-custom-avr

ARDUINO_USB_DEVICE=/dev/ttyACM0
BAUD_RATE=115200
AVR_MCU_ARCHITECTURE=atmega328p

all: build deploy

build:
	@echo "*** Building project in Docker image ***"
	docker build -t $(IMAGE_NAME) .

deploy:
	@echo "*** Deploying to Arduino using Docker image build artifacts ***"
	sudo docker run \
		-it \
		--rm \
		--device=$(ARDUINO_USB_DEVICE) \
		$(IMAGE_NAME) \
		avrdude \
		-U flash:w:build/main.hex \
		-p $(AVR_MCU_ARCHITECTURE) \
		-c arduino \
		-P $(ARDUINO_USB_DEVICE) \
		-b $(BAUD_RATE) \
		-D \
		-v

monitor:
	@echo "*** Monitoring Arduino on usb device ***"
	sudo stty -F /dev/ttyACM0 9600 raw -clocal -echo
	sudo cat /dev/ttyACM0

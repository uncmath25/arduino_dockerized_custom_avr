#include "pins.h"
#include "utils.h"

#include "Arduino.h"


void setup() {
    pinMode(LED_ANODE, OUTPUT);
    pinMode(LED_RED_CATHODE, OUTPUT);
    pinMode(LED_GREEN_CATHODE, OUTPUT);
    pinMode(LED_BLUE_CATHODE, OUTPUT);

    // Serial.begin(9600);
    // Serial.println(COLOR_RESOLUTION_CYCLES_PER_MILLISECOND);

    turnAllOff();
}

void loop() {
    int color[3] = {0, 0, 128};
    showColor(color, 4 * 1000);
    delay(1 * 1000);
}

#include "board.h"
#include "shows.h"
#include "utils.h"

#include "Arduino.h"

void setup() {
    pinMode(LED_ANODE, OUTPUT);
    pinMode(LED_RED_CATHODE, OUTPUT);
    pinMode(LED_GREEN_CATHODE, OUTPUT);
    pinMode(LED_BLUE_CATHODE, OUTPUT);

    Serial.begin(BAUD_RATE);

    turnAllOff();
}

void loop() {
    // Serial.println("Testing LED...");
    // int color[3] = {0, 0, 128};
    // showColor(color, 4 * 1000);
    // delay(1 * 1000);

    runRandomShow();

    // int startColor[3] = {128, 0, 0};
    // int endColor[3] = {0, 0, 128};
    // transitionColors(startColor, endColor, 5 * 1000);
    // delay(2 * 1000);
}

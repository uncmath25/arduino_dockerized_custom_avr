#include "board.h"
#include "shows.h"
#include "utils.h"

#include "Arduino.h"

void setup() {
    pinMode(LED_ANODE, OUTPUT);
    pinMode(LED_RED_CATHODE, OUTPUT);
    pinMode(LED_GREEN_CATHODE, OUTPUT);
    pinMode(LED_BLUE_CATHODE, OUTPUT);

    // Serial.begin(BAUD_RATE);

    turnAllOff();
}

void loop() {
    // runBoringShow(128, 0, 0, 3, 1);
    // runSimpleTransitionShow(128, 0, 0, 0, 0, 128, 10);
    runRandomTransitionShow(10);
}

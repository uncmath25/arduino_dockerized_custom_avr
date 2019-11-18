#include "pins.h"
#include "utils.h"

#include "Arduino.h"


const int LED_DUTY_CYCLES = 30;

const int COLOR_INTERVAL_MICROSECONDS = 100;
const float COLOR_INTERVAL_SCALING = 0.77;
const int COLOR_RESOLUTION_CYCLES_PER_MILLISECOND = (1000 / COLOR_INTERVAL_MICROSECONDS) * COLOR_INTERVAL_SCALING;

const int COLOR_TRANSITION_INTERVAL_MILLISECONDS = 100;


void turnAllOff() {
    digitalWrite(LED_RED_CATHODE, HIGH);
    digitalWrite(LED_GREEN_CATHODE, HIGH);
    digitalWrite(LED_BLUE_CATHODE, HIGH);
}

void showColor(int rgbColor[3], int durationMilliseconds) {
    turnAllOff();
    analogWrite(LED_ANODE, LED_DUTY_CYCLES);
    float scaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (rgbColor[0] + rgbColor[1] + rgbColor[2]);
    int redInterval = int(rgbColor[0] * scaleFactor);
    int greenInterval = int(rgbColor[1] * scaleFactor);
    int blueInterval = int(rgbColor[2] * scaleFactor);
    // Serial.println(redInterval);
    // Serial.println(greenInterval);
    // Serial.println(blueInterval);
    for (int i = 0; i < durationMilliseconds; i++) {
        for (int j = 0; j < COLOR_RESOLUTION_CYCLES_PER_MILLISECOND; j++) {
            digitalWrite(LED_RED_CATHODE, LOW);
            delayMicroseconds(redInterval);
            digitalWrite(LED_RED_CATHODE, HIGH);
            digitalWrite(LED_GREEN_CATHODE, LOW);
            delayMicroseconds(greenInterval);
            digitalWrite(LED_GREEN_CATHODE, HIGH);
            digitalWrite(LED_BLUE_CATHODE, LOW);
            delayMicroseconds(blueInterval);
            digitalWrite(LED_BLUE_CATHODE, HIGH);
        }
    }
    turnAllOff();
}

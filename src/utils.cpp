#include "board.h"
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

void transitionColors(int startRgbColor[3], int endRgbColor[3], int transitionMilliseconds) {
    int transitionSteps = transitionMilliseconds / COLOR_TRANSITION_INTERVAL_MILLISECONDS;
    // Serial.println(transitionSteps);
    turnAllOff();
    analogWrite(LED_ANODE, LED_DUTY_CYCLES);
    float scaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (startRgbColor[0] + startRgbColor[1] + startRgbColor[2]);
    float redInterval = startRgbColor[0] * scaleFactor;
    float greenInterval = startRgbColor[1] * scaleFactor;
    float blueInterval = startRgbColor[2] * scaleFactor;
    float endScaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (endRgbColor[0] + endRgbColor[1] + endRgbColor[2]);
    float endRedInterval = endRgbColor[0] * endScaleFactor;
    float endGreenInterval = endRgbColor[1] * endScaleFactor;
    float endBlueInterval = endRgbColor[2] * endScaleFactor;
    float redIntervalUpdate = (endRedInterval - redInterval) / transitionSteps;
    float greenIntervalUpdate = (endGreenInterval - greenInterval) / transitionSteps;
    float blueIntervalUpdate = (endBlueInterval - blueInterval) / transitionSteps;
    // Serial.println(redInterval);
    // Serial.println(greenInterval);
    // Serial.println(blueInterval);
    Serial.println(redIntervalUpdate);
    Serial.println(greenIntervalUpdate);
    Serial.println(blueIntervalUpdate);
    for (int i = 0; i < transitionSteps; i++) {
        redInterval += redIntervalUpdate;
        greenInterval += greenIntervalUpdate;
        blueInterval += blueIntervalUpdate;
        for (int j = 0; j < COLOR_RESOLUTION_CYCLES_PER_MILLISECOND * COLOR_TRANSITION_INTERVAL_MILLISECONDS; j++) {
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

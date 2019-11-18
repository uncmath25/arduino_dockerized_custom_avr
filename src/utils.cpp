#include "board.h"
#include "color_settings.h"
#include "utils.h"

#include "Arduino.h"

void runRgbCycle(float, float, float);

void turnAllOff() {
    digitalWrite(LED_RED_CATHODE, HIGH);
    digitalWrite(LED_GREEN_CATHODE, HIGH);
    digitalWrite(LED_BLUE_CATHODE, HIGH);
}

void showColor(int rgbColor[RGB_CHANNELS], int durationMilliseconds) {
    turnAllOff();
    analogWrite(LED_ANODE, LED_DUTY_CYCLES);
    const float scaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (rgbColor[0] + rgbColor[1] + rgbColor[2]);
    const float redInterval = rgbColor[0] * scaleFactor;
    const float greenInterval = rgbColor[1] * scaleFactor;
    const float blueInterval = rgbColor[2] * scaleFactor;
    // Serial.println(redInterval);
    // Serial.println(greenInterval);
    // Serial.println(blueInterval);
    for (int i = 0; i < durationMilliseconds; i++) {
        for (int j = 0; j < COLOR_RESOLUTION_CYCLES_PER_MILLISECOND; j++) {
            runRgbCycle(redInterval, greenInterval, blueInterval);
        }
    }
    turnAllOff();
}

void transitionColors(int startRgbColor[RGB_CHANNELS], int endRgbColor[RGB_CHANNELS], int transitionMilliseconds) {
    turnAllOff();
    analogWrite(LED_ANODE, LED_DUTY_CYCLES);
    const int transitionSteps = transitionMilliseconds / COLOR_TRANSITION_INTERVAL_MILLISECONDS;
    const float scaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (startRgbColor[0] + startRgbColor[1] + startRgbColor[2]);
    float redInterval = startRgbColor[0] * scaleFactor;
    float greenInterval = startRgbColor[1] * scaleFactor;
    float blueInterval = startRgbColor[2] * scaleFactor;
    const float endScaleFactor = float(COLOR_INTERVAL_MICROSECONDS) / (endRgbColor[0] + endRgbColor[1] + endRgbColor[2]);
    const float endRedInterval = endRgbColor[0] * endScaleFactor;
    const float endGreenInterval = endRgbColor[1] * endScaleFactor;
    const float endBlueInterval = endRgbColor[2] * endScaleFactor;
    const float redIntervalUpdate = (endRedInterval - redInterval) / transitionSteps;
    const float greenIntervalUpdate = (endGreenInterval - greenInterval) / transitionSteps;
    const float blueIntervalUpdate = (endBlueInterval - blueInterval) / transitionSteps;
    // Serial.println(redIntervalUpdate);
    // Serial.println(greenIntervalUpdate);
    // Serial.println(blueIntervalUpdate);
    const int transitionStepCycles = COLOR_RESOLUTION_CYCLES_PER_MILLISECOND * COLOR_TRANSITION_INTERVAL_MILLISECONDS;
    for (int i = 0; i < transitionSteps; i++) {
        redInterval += redIntervalUpdate;
        greenInterval += greenIntervalUpdate;
        blueInterval += blueIntervalUpdate;
        for (int j = 0; j < transitionStepCycles; j++) {
            runRgbCycle(redInterval, greenInterval, blueInterval);
        }
    }
    turnAllOff();
}

void runRgbCycle(float redInterval, float greenInterval, float blueInterval) {
    digitalWrite(LED_RED_CATHODE, LOW);
    delayMicroseconds(redInterval);
    digitalWrite(LED_RED_CATHODE, HIGH);
    digitalWrite(LED_GREEN_CATHODE, LOW);
    delayMicroseconds(greenInterval);
    digitalWrite(LED_GREEN_CATHODE, HIGH);
    digitalWrite(LED_BLUE_CATHODE, LOW);
    delayMicroseconds(blueInterval);
    digitalWrite(LED_BLUE_CATHODE, HIGH);;
}

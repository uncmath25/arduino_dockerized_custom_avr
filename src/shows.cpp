#include "color_settings.h"
#include "shows.h"
#include "utils.h"

#include "Arduino.h"

void runBoringShow(int red, int green, int blue, int colorIntervalSeconds, int offIntervalSeconds) {
    // Serial.println("Running Boring Show Again...");
    int rgbColor[RGB_CHANNELS] = {red, green, blue};
    showColor(rgbColor, colorIntervalSeconds * 1000);
    delay(offIntervalSeconds * 1000);
}

void runSimpleTransitionShow(int startRed, int startGreen, int startBlue, int endRed, int endGreen, int endBlue, int transitionSeconds) {
    int startRgbColor[RGB_CHANNELS] = {startRed, startGreen, startBlue};
    int endRgbColor[RGB_CHANNELS] = {endRed, endGreen, endBlue};
    const int transitionMilliSeconds = transitionSeconds * 1000;
    while (true) {
        // Serial.println("Starting Color Transition Again...");
        transitionColors(startRgbColor, endRgbColor, transitionMilliSeconds);
        transitionColors(endRgbColor, startRgbColor, transitionMilliSeconds);
    }
}

void runRandomTransitionShow(int transitionSeconds) {
    randomSeed(25);
    int oldColor[RGB_CHANNELS];
    int newColor[RGB_CHANNELS];
    for (int i = 0; i < RGB_CHANNELS; i++) {
        oldColor[i] = int(random(ANALOG_WRITE_MAX));
    }
    const int transitionMilliSeconds = transitionSeconds * 1000;
    while (true) {
        // Serial.println("Starting Random Color Transition Again From Old RGB:");
        // Serial.println(oldColor[0]);
        // Serial.println(oldColor[1]);
        // Serial.println(oldColor[2]);
        for (int i = 0; i < RGB_CHANNELS; i++) {
            newColor[i] = int(random(ANALOG_WRITE_MAX));
        }
        transitionColors(oldColor, newColor, transitionMilliSeconds);
        memcpy(oldColor, newColor, sizeof(newColor));
    }
}
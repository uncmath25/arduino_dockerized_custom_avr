#include "shows.h"
#include "utils.h"

#include "Arduino.h"

void runRandomShow(){
    randomSeed(25);
    int oldColor[3];
    int newColor[3];
    for (int i = 0; i < 3; i++) {
        oldColor[i] = int(random(255));
    }
    while (true) {
        for (int i = 0; i < 3; i++) {
            newColor[i] = int(random(255));
        }
        transitionColors(oldColor, newColor, 10 * 1000);
        memcpy(oldColor, newColor, sizeof(newColor));
        Serial.println(oldColor[0]);
        Serial.println(oldColor[1]);
        Serial.println(oldColor[2]);
    }
}
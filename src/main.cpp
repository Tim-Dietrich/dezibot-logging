#include <Arduino.h>
#include <Dezibot.h>

Dezibot dezibot;

int i = 0;

void setup() {
    dezibot.begin();
    dezibot.debugServer.setup();
}

void loop() {
    //dezibot.colorDetection.getAmbientLight();
    dezibot.colorDetection.getColorValue(VEML_RED);
    dezibot.display.println(i);
    i++;
    delay(30000);
}


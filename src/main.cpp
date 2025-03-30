#include <Arduino.h>
#include <Dezibot.h>
#include <logger/Logger.h>

Dezibot dezibot;

int i = 0;

void setup() {
    dezibot.begin();
    dezibot.debugServer.setup();
}

void loop() {
    // dezibot.colorDetection.getAmbientLight();
    dezibot.colorDetection.getColorValue(VEML_RED);
    dezibot.display.println(i);
    i++;
    Logger::getInstance().logDebug("Hello World");
    Logger::getInstance().logError("Hello World");
    Logger::getInstance().logWarning("Hello World");
    delay(30000);

    // dezibot.motion.move();
    // delay(2500);
    // dezibot.motion.stop();
    // dezibot.display.print("Moved");
    // delay(5000);
    // dezibot.display.clear();
    // dezibot.display.print("Waited for 5000ms");
    // delay(1000);
    // dezibot.display.clear();
}


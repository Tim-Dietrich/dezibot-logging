#include <Arduino.h>
#include <Dezibot.h>

Dezibot dezibot;

void setup() {
    dezibot.begin();
    dezibot.debugServer.setup();
}

void loop() {
    dezibot.debugServer.handleClient();
    delay(1);
}


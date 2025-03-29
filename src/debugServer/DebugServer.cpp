/**
 * @file DebugServer.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the DebugServer class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DebugServer.h"
#include <Dezibot.h>

#include "PageProvider.h"
#include "MainPage.h"
#include <WebServer.h>
#include <WiFi.h>
#include <SPIFFS.h>

WebServer server;
extern Dezibot dezibot;

DebugServer::DebugServer():server(80) {
    mainPage = new MainPage(&server);
    loggingPage = new LoggingPage(&server);
    settingsPage = new SettingsPage(&server);
    liveDataPage = new LiveDataPage(&server);
}

void DebugServer::setup() {
    // set wi-fi credentials
    const char* SSID = "Debug-Server";
    const char* PSK = "PW4studProj";

    // set IP configuration
    const IPAddress local_ip(192,168,1,1);
    const IPAddress gateway(192,168,1,1);
    const IPAddress subnet(255,255,255,0);

    // initialize SPIFFS for file access
    // changes in html files require "pio run -t uploadfs" or "Upload Filesystem Image" in plugin to take effect
    SPIFFS.begin();

    // setup as wi-fi access point
    WiFi.softAP(SSID, PSK);
    WiFi.softAPConfig(local_ip, gateway, subnet);

    // set uri and handler for each page
    // Main Page
    server.on("/", [this] {
        mainPage->handler();
    });

    server.on("/css/mainPageStyle.css", [this] {
       mainPage->cssHandler();
    });

    // Logging
    server.on("/logging", [this] {
        loggingPage->handler();
    });

    server.on("/css/loggingPageStyle.css", [this] {
        loggingPage->cssHandler();
    });

    server.on("/js/loggingPageScript.js", [this] {
        loggingPage->jsHandler();
    });

    // Live Data
    server.on("/livedata", [this] {
        liveDataPage->handler();
    });

    server.on("/lib/canvasjs.min.js", [this] {
        liveDataPage->canvasjsHandler();
    });

    server.on("/js/liveDataPageScript.js", [this] {
        liveDataPage->jsHandler();
    });

    server.on("/css/liveDataPageStyle.css", [this] {
       liveDataPage->cssHandler();
    });

    // Settings
    server.on("/settings", [this] {
        settingsPage->handler();
    });

    server.on("/js/settingsPageScript.js", [this] {
        settingsPage->jsHandler();
    });

    server.on("/css/settingsPageStyle.css", [this] {
       settingsPage->cssHandler();
    });

    // TODO: we also need this, it should always return a 404
    server.onNotFound([this] {
        mainPage->errorPageHandler();
    });

    // initialize color sensor
    Sensor colorSensor("Color Sensor", "ColorDetection");
    SensorFunction getAmbientLight("getAmbientLight()",
        [] { return std::to_string(dezibot.colorDetection.getAmbientLight()); });
    SensorFunction getColorValueRed("getColorValue(RED)",
        [] { return std::to_string(dezibot.colorDetection.getColorValue(VEML_RED)); });
    SensorFunction getColorValueGreen("getColorValue(GREEN)",
        [] { return std::to_string(dezibot.colorDetection.getColorValue(VEML_GREEN)); });
    SensorFunction getColorValueBlue("getColorValue(BLUE)",
        [] { return std::to_string(dezibot.colorDetection.getColorValue(VEML_BLUE)); }) ;
    SensorFunction getColorValueWhite("getColorValue(WHITE)",
        [] { return std::to_string(dezibot.colorDetection.getColorValue(VEML_WHITE)); });
    colorSensor.addFunction(getAmbientLight);
    colorSensor.addFunction(getColorValueRed);
    colorSensor.addFunction(getColorValueGreen);
    colorSensor.addFunction(getColorValueBlue);
    colorSensor.addFunction(getColorValueWhite);
    addSensor(colorSensor);

    // initialize light sensor
    Sensor lightSensor("Light Sensor", "LightDetection");
    SensorFunction getValueIrFront("getValue(IR_FRONT)",
        [] { return std::to_string(LightDetection::getValue(IR_FRONT)); });
    SensorFunction getValueIrLeft("getValue(IR_LEFT)",
        [] { return std::to_string(LightDetection::getValue(IR_LEFT)); });
    SensorFunction getValueIrRight("getValue(IR_RIGHT)",
        [] { return std::to_string(LightDetection::getValue(IR_RIGHT)); });
    SensorFunction getValueIrBack("getValue(IR_BACK)",
        [] { return std::to_string(LightDetection::getValue(IR_BACK)); });
    SensorFunction getValueDlBottom("getValue(DL_BOTTOM)",
        [] { return std::to_string(LightDetection::getValue(DL_BOTTOM)); });
    SensorFunction getValueDlFront("getValue(DL_FRONT)",
        [] { return std::to_string(LightDetection::getValue(DL_FRONT)); });
    lightSensor.addFunction(getValueIrFront);
    lightSensor.addFunction(getValueIrLeft);
    lightSensor.addFunction(getValueIrRight);
    lightSensor.addFunction(getValueIrBack);
    lightSensor.addFunction(getValueDlBottom);
    lightSensor.addFunction(getValueDlFront);
    addSensor(lightSensor);

    // initialize motor
    Sensor motor("Motor", "Motion");
    SensorFunction getSpeedLeft("left.getSpeed()",
        [] { return std::to_string(Motion::left.getSpeed()); });
    SensorFunction getSpeedRight("right.getSpeed()",
        [] { return std::to_string(Motion::right.getSpeed()); });
    motor.addFunction(getSpeedLeft);
    motor.addFunction(getSpeedRight);
    addSensor(motor);

    // initialize motion sensor
    Sensor motionSensor("Motion Sensor", "MotionDetection");
    SensorFunction getAcceleration("getAcceleration()",
        [] { IMUResult result = Motion::detection.getAcceleration();
            return "x: " + std::to_string(result.x) + ", y: " + std::to_string(result.y) + ", z: " +
                std::to_string(result.z); });
    SensorFunction getRotation("getRotation()",
        [] { IMUResult result = Motion::detection.getRotation();
            return "x: " + std::to_string(result.x) + ", y: " + std::to_string(result.y) + ", z: " +
                std::to_string(result.z); });
    SensorFunction getTemperature("getTemperature()",
        [] { return std::to_string(Motion::detection.getTemperature()); });
    SensorFunction getWhoAmI("getWhoAmI()",
        [] { return std::to_string(Motion::detection.getWhoAmI()); });
    SensorFunction getTilt("getTilt()",
        [] { Orientation result = Motion::detection.getTilt();
            return "x: " + std::to_string(result.xRotation) + ", y: " + std::to_string(result.yRotation); });
    SensorFunction getTiltDirection("getTiltDirection()",
        [] { Direction result = Motion::detection.getTiltDirection();
            return std::to_string(result); });
    motionSensor.addFunction(getAcceleration);
    motionSensor.addFunction(getRotation);
    motionSensor.addFunction(getTemperature);
    motionSensor.addFunction(getWhoAmI);
    motionSensor.addFunction(getTilt);
    motionSensor.addFunction(getTiltDirection);
    addSensor(motionSensor);

    // start webserver and run client handler on separate thread
    server.begin();
    beginClientHandle();
};

void DebugServer::addSensor(const Sensor& sensor) {
    sensors.push_back(sensor);
}

std::vector<Sensor>& DebugServer::getSensors() {
    return sensors;
}

// create a FreeRTOS task to handle client requests
void DebugServer::beginClientHandle() {
    xTaskCreate(
        handleClientTask,
        "DebugServerTask",
        4096,
        this,
        20,
        nullptr
    );
}

// handle client requests
void DebugServer::handleClientTask(void* parameter) {
     auto debugServer = static_cast<DebugServer*>(parameter);

    // continuously handle client requests
    while (debugServer->serveractive) {
        debugServer->server.handleClient();
        delay(10);
    }

    // Delete task when server is no longer active
    vTaskDelete(nullptr);
}

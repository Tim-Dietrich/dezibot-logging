/**
 * @file LiveDataPage.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the LiveDataPage class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "LiveDataPage.h"
#include <Dezibot.h>
#include <ArduinoJson.h>
#include <logger/Logger.h>

#include "Utility.h"

extern Dezibot dezibot;

LiveDataPage::LiveDataPage(WebServer* server): serverPointer(server)
{
    server->on("/getEnabledSensorValues", [this]() {
        getEnabledSensorValues();
    });
}

// send the html content of the LiveDataPage
void LiveDataPage::handler() {
    serveFileFromSpiffs(serverPointer, "/liveDataPage.html", "text/html");
};

void LiveDataPage::canvasjsHandler() {
    serveFileFromSpiffs(serverPointer, "/lib/canvasjs.min.js", "text/javascript");
}

void LiveDataPage::jsHandler() {
    serveFileFromSpiffs(serverPointer, "/js/liveDataPageScript.js", "text/javascript");
}

void LiveDataPage::cssHandler() {
    serveFileFromSpiffs(serverPointer, "/css/liveDataPageStyle.css", "text/css");
}

// read values from enabled sensors and send them as json
void LiveDataPage::getEnabledSensorValues() {
    JsonDocument jsonDoc;
    JsonArray sensorArray = jsonDoc.to<JsonArray>();

    // disable logging to prevent unwanted entries
    Logger::getInstance().setLoggingEnabled(false);

    // iterate over all sensors and sensorfunctions and add the values to the json
    auto& sensors = dezibot.debugServer.getSensors();
    for (auto& sensor : sensors) {
        for (auto& sensorFunction : sensor.getSensorFunctions()) {
            if (sensorFunction.getSensorState()) {
                // read and add value if sensorfunction is enabled
                JsonObject sensorJson = sensorArray.add<JsonObject>();
                sensorJson["name"] = sensorFunction.getFunctionName();
                sensorJson["value"] = sensorFunction.getStringValue();
            }
        }
    }

    // enable logging again
    Logger::getInstance().setLoggingEnabled(true);

    // send json response
    String jsonResponse;
    serializeJson(jsonDoc, jsonResponse);
    serverPointer->send(200, "application/json", jsonResponse);
}
/**
 * @file SensorFunction.cpp
 * @author Tim Dietrich, Felix Herrling
 * @brief Implementation of the SensorFunction class.
 * @version 1.0
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "SensorFunction.h"

SensorFunction::SensorFunction(std::string name, std::function<std::string()> func)
    : name(std::move(name)), function(std::move(func)) {
    sensorState = false; // disabled by default
}

void SensorFunction::setSensorState(bool state) {
    sensorState = state;
}

bool SensorFunction::getSensorState() {
    return sensorState;
}

std::string& SensorFunction::getFunctionName() {
    return name;
}

std::string SensorFunction::getStringValue() {
    return function();
}
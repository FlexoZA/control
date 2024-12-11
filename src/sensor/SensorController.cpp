#include "SensorController.h"

SensorController::SensorController(int pin) : sensorPin(pin), isActive(false) {
    pinMode(sensorPin, INPUT);
}

bool SensorController::checkActive() {
    // Read the sensor pin to determine if it's active
    int reading = digitalRead(sensorPin);
    isActive = (reading == HIGH);
    return isActive;
}

bool SensorController::getIsActive() const {
    return isActive;
}

void SensorController::update() {
    checkActive();
}
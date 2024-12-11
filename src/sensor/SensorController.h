#ifndef SENSOR_CONTROLLER_H
#define SENSOR_CONTROLLER_H

#include <Arduino.h>
#include "../Config.h"

class SensorController {
private:
    const int sensorPin;
    bool isActive;

public:
    SensorController(int pin);
    bool checkActive();
    bool getIsActive() const;
    void update();
};

#endif // SENSOR_CONTROLLER_H
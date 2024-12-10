#pragma once

#include <Config.h>

class StepperController {
public:
    // Constructor
    StepperController();

    // Rotate motor with specified steps and direction
    void rotate(int steps, bool clockwise);

    // Additional methods like setSpeed can be added here
    void setStepDelay(int delay);
    void setTotalSteps(int steps);

private:
    // Motor configuration parameters
    int stepDelay;
    int totalSteps;

    // Internal methods for motor control
    void setMotorState(int a, int b, int c, int d);
    void halfStepSequence(int step);
};
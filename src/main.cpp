#include <Arduino.h>
#include "stepper/StepperController.h"

// Create a global stepper controller instance
StepperController stepper;

void setup() {
   
}

void loop() {
    // Rotate 360 degrees clockwise
    stepper.rotate(4096, true);
    delay(1000); // Wait 1 second

    // Rotate 360 degrees counter-clockwise
    stepper.rotate(2048, false);
    delay(1000); // Wait 1 second
}

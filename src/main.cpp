#include <Arduino.h>
#include <Config.h>
#include <stepper/StepperController.h>

// Function prototype for setMotorState
void setMotorState(int a, int b, int c, int d);

// Set motor coil states
void setMotorState(int a, int b, int c, int d) {
  digitalWrite(IN1, a);
  digitalWrite(IN2, b);
  digitalWrite(IN3, c);
  digitalWrite(IN4, d);
}

// Define the motor sequence for half-step mode
void halfStepSequence(int step) {
  switch (step % 8) { // 8 steps in half-step mode
    case 0: setMotorState(1, 0, 0, 0); break; // IN1
    case 1: setMotorState(1, 1, 0, 0); break; // IN1 + IN2
    case 2: setMotorState(0, 1, 0, 0); break; // IN2
    case 3: setMotorState(0, 1, 1, 0); break; // IN2 + IN3
    case 4: setMotorState(0, 0, 1, 0); break; // IN3
    case 5: setMotorState(0, 0, 1, 1); break; // IN3 + IN4
    case 6: setMotorState(0, 0, 0, 1); break; // IN4
    case 7: setMotorState(1, 0, 0, 1); break; // IN4 + IN1
  }
}

// Rotate motor in the specified direction
void rotateMotor(int steps, bool clockwise) {
  for (int i = 0; i < steps; i++) {
    int stepIndex = clockwise ? i : steps - i - 1;
    halfStepSequence(stepIndex);
    delay(stepDelay);
  }
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Rotate 360 degrees clockwise
  rotateMotor(totalSteps, true);
  delay(1000); // Wait 1 second

  // Rotate 360 degrees counter-clockwise
  rotateMotor(totalSteps, false);
  delay(1000); // Wait 1 second
}
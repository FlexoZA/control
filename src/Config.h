#pragma once

// Define GPIO pins

//Stepper1
#define IN1 15 //GPIO 15
#define IN2 2 //GPIO 2
#define IN3 4 //GPIO 4
#define IN4 16 // GPIO 16

//Stepper config
int stepDelay = 1.1;  // Delay between steps in milliseconds
int totalSteps = 2048;  // Full rotation for 28BYJ-48 (360 degrees)
#include <Arduino.h>
#include "stepper/StepperController.h"
#include "API/ApiController.h"
#include "comms/WifiController.h"

WifiController wifi;
ApiController apiController(PRINTER_IP, wifi);

// Create a global stepper controller instance
StepperController stepper;

// Variable to track last WiFi test time
unsigned long lastWifiTestTime = 0;
const unsigned long WIFI_TEST_INTERVAL = 30000; // Test WiFi every 30 seconds

// Variables for rotation control
bool hasRotated = false;
bool isPaused = false;
String currentState = "";

void setup() {
    Serial.begin(115200);
    wifi.begin();
    apiController.begin();
}

void loop() {
    // Check and maintain WiFi connection
    wifi.checkConnection();
    
    // Periodically test WiFi connection details
    unsigned long currentTime = millis();
    if (currentTime - lastWifiTestTime >= WIFI_TEST_INTERVAL) {
        wifi.testConnection();
        lastWifiTestTime = currentTime;
    }

    // Update printer status
    apiController.updateStatus();
    
    // Get current printer state
    if (apiController.getPrinterStatus()) {
        currentState = apiController.getCurrentState();
        Serial.print("Current State: ");
        Serial.println(currentState);
        
        // If printer is paused and we haven't rotated yet
        if (currentState == "paused" && !hasRotated) {
            Serial.println("Printer paused - starting rotation sequence");
            
            // Rotate 360 degrees clockwise (4096 steps)
            stepper.rotate(TotalSteps, true);
            delay(1000); // Wait 1 second
            
            // Rotate 360 degrees counterclockwise
            stepper.rotate(TotalSteps, false);
            
            // Mark that we've completed the rotation
            Serial.println("Rotation complete");
            hasRotated = true;
            
            // Resume the print
            if (apiController.resumePrint()) {
                Serial.println("Print resumed after rotation");
            } else {
                Serial.println("Failed to resume print");
            }
        }
        // Reset the rotation flag if the printer is no longer paused
        else if (currentState != "paused") {
            hasRotated = false;
        }
    }
}

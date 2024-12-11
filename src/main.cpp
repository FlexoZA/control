#include <Arduino.h>
#include "stepper/StepperController.h"
#include "API/ApiController.h"
#include "comms/WifiController.h"
#include "sensor/SensorController.h"
#include "Config.h"

WifiController wifi;
ApiController apiController(PRINTER_IP, wifi);

// Create a global stepper controller instance
StepperController stepper;

// Create a global sensor controller instance
SensorController sensor(FILAMENT_SENSOR_PIN);

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
    stepper.begin();
}

void loop() {
    // Check and maintain WiFi connection
    wifi.checkConnection();

    // Update sensor status
    sensor.update();
    
    // Get current printer state
    if (apiController.getPrinterStatus()) {
        currentState = apiController.getCurrentState();
        
        // Check if printer is paused and sensor is active
        if (currentState == "paused" && sensor.getIsActive()) {
            // Rotate stepper anticlockwise continuously until sensor becomes inactive
            while (sensor.getIsActive()) {
                stepper.rotate(100, false);  // false for anticlockwise
                sensor.update();  // Update sensor status
                delay(10);  // Small delay to prevent overwhelming the system
            }
            
            // Wait for sensor to become active again
            while (!sensor.getIsActive()) {
                sensor.update();
                delay(100);  // Check every 100ms
            }
            
            // Once sensor is active again, rotate clockwise 360 degrees
            stepper.rotate(TotalSteps, true);  // true for clockwise, TotalSteps for 360 degrees
            
            // Send resume command to printer
            apiController.resumePrint();
        }
    }

    // Periodically test WiFi connection details
    unsigned long currentTime = millis();
    if (currentTime - lastWifiTestTime >= WIFI_TEST_INTERVAL) {
        wifi.testConnection();
        lastWifiTestTime = currentTime;
    }
}

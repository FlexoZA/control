#pragma once

// Define GPIO pins
//Stepper1
#define IN1 15 //GPIO 15
#define IN2 2 //GPIO 2
#define IN3 4 //GPIO 4
#define IN4 16 // GPIO 16

#define TotalSteps 4096
#define StepDelay 1.00

// Printer Configuration
#define PRINTER_IP "192.168.100.28"  // Replace with your printer's IP address
#define Endpoint "/printer/objects/query?print_stats&display_status&webhooks"
#define RESUME_ENDPOINT "/printer/print/resume"
#define API_UPDATE_INTERVAL 10000  // Time in ms between API updates

// WiFi Configuration
#define WIFI_SSID "Lab"
#define WIFI_PASSWORD "jason789"
#define WIFI_RETRY_DELAY 5000 // Time in ms between connection attempts
#define WIFI_MAX_ATTEMPTS 10  // Maximum connection attempts before giving up
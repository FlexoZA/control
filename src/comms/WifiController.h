#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "../Config.h"

class WifiController {
private:
    bool isConnected;
    int connectionAttempts;
    unsigned long lastAttemptTime;
    void printWifiDetails();

public:
    WifiController();
    void begin();
    bool connect();
    bool isWifiConnected() const;
    void checkConnection();
    void disconnect();
    bool testConnection();
};
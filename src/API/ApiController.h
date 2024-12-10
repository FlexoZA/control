#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../comms/WifiController.h"
#include "../Config.h"

class ApiController {
private:
    String baseUrl;
    HTTPClient http;
    const int jsonBufferSize = 2048;
    WifiController& wifiController;
    unsigned long lastUpdateTime;
    String currentPrinterState;

public:
    ApiController(const String& printerIp, WifiController& wifi);
    void begin();
    bool getPrinterStatus();
    String getCurrentState() { return currentPrinterState; }
    bool resumePrint();
    void updateStatus();
};
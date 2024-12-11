#include "ApiController.h"

ApiController::ApiController(const String& printerIp, WifiController& wifi) 
    : wifiController(wifi), lastUpdateTime(0) {
    baseUrl = "http://" + printerIp;
}

void ApiController::begin() {
    Serial.println("ApiController initialized");
}

bool ApiController::getPrinterStatus() {
    if (!wifiController.isWifiConnected()) {
        Serial.println("Cannot get printer status: WiFi not connected");
        return false;
    }

    String endpoint = baseUrl + Endpoint;
    
    http.begin(endpoint);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        
        DynamicJsonDocument doc(jsonBufferSize);
        DeserializationError error = deserializeJson(doc, response);
        
        if (error) {
            Serial.print("JSON parsing failed: ");
            Serial.println(error.c_str());
            http.end();
            return false;
        }
        
        // Extract and print printer status
        const char* state = doc["result"]["status"]["print_stats"]["state"] | "unknown";
        currentPrinterState = String(state);  // Store the state
        float completion = doc["result"]["status"]["display_status"]["progress"] | 0.0;
        
        //Serial.println("Printer Status:");
        //Serial.print("Print State: ");
        //Serial.println(currentPrinterState);
        //Serial.print("Progress: ");
        //Serial.print(completion * 100);
        //Serial.println("%");
        
        http.end();
        return true;
    } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}

bool ApiController::resumePrint() {
    if (!wifiController.isWifiConnected()) {
        Serial.println("Cannot resume print: WiFi not connected");
        return false;
    }

    String endpoint = baseUrl + RESUME_ENDPOINT;
    
    http.begin(endpoint);
    int httpResponseCode = http.POST("");  // Empty POST request
    
    if (httpResponseCode > 0) {
        Serial.println("Print resumed successfully");
        http.end();
        return true;
    } else {
        Serial.print("Error resuming print. Code: ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}

void ApiController::updateStatus() {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= API_UPDATE_INTERVAL) {
        if (wifiController.isWifiConnected()) {
            getPrinterStatus();
        }
        lastUpdateTime = currentTime;
    }
}
#include "WifiController.h"

WifiController::WifiController() : isConnected(false), connectionAttempts(0), lastAttemptTime(0) {}

void WifiController::begin() {
    Serial.println("Initializing WiFi...");
    WiFi.mode(WIFI_STA);
    connect();
}

void WifiController::printWifiDetails() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n=== WiFi Connection Details ===");
        Serial.print("SSID: ");
        Serial.println(WiFi.SSID());
        Serial.print("Signal Strength (RSSI): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Subnet Mask: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Gateway IP: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("DNS Server: ");
        Serial.println(WiFi.dnsIP());
        Serial.print("MAC Address: ");
        Serial.println(WiFi.macAddress());
        Serial.println("============================\n");
    } else {
        Serial.println("WiFi not connected!");
        Serial.print("Status code: ");
        Serial.println(WiFi.status());
    }
}

bool WifiController::testConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected - Testing failed");
        return false;
    }

    // Test connection by pinging the gateway
    IPAddress gateway = WiFi.gatewayIP();
    Serial.print("Testing connection to gateway (");
    Serial.print(gateway);
    Serial.println(")");
    
    if (gateway[0] != 0) {
        Serial.println("Gateway reachable");
        printWifiDetails();
        return true;
    } else {
        Serial.println("Cannot reach gateway!");
        return false;
    }
}

bool WifiController::connect() {
    if (WiFi.status() == WL_CONNECTED) {
        isConnected = true;
        return true;
    }

    if (connectionAttempts >= WIFI_MAX_ATTEMPTS) {
        Serial.println("Max connection attempts reached. Please check WiFi credentials.");
        return false;
    }

    unsigned long currentTime = millis();
    if (currentTime - lastAttemptTime < WIFI_RETRY_DELAY && lastAttemptTime != 0) {
        return false;
    }

    Serial.print("Connecting to WiFi ");
    Serial.print(connectionAttempts + 1);
    Serial.print("/");
    Serial.print(WIFI_MAX_ATTEMPTS);
    Serial.print(": ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    lastAttemptTime = currentTime;
    connectionAttempts++;

    // Wait for connection with timeout
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
        delay(500);
        Serial.print(".");
        timeout++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        isConnected = true;
        Serial.println("WiFi connected successfully!");
        printWifiDetails();
        return true;
    }

    Serial.println("WiFi connection failed");
    return false;
}

bool WifiController::isWifiConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

void WifiController::checkConnection() {
    if (!isWifiConnected()) {
        connect();
    }
}

void WifiController::disconnect() {
    WiFi.disconnect();
    isConnected = false;
    Serial.println("WiFi disconnected");
}
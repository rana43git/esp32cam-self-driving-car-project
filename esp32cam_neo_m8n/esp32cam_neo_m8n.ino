#include <WiFi.h>
#include <WebServer.h>
#include <TinyGPSPlus.h>
#include "webpage.h"

// GPS and Web Server
TinyGPSPlus gps;
#define GPS_Serial Serial // Use Serial0 for GPS
WebServer server(80);

// WiFi AP credentials
const char* ssid = "ESP32CAM_GPS";
const char* password = "12345678";

// GPS Data
String getGPSData() {
    String data = "Waiting for GPS fix...";
    if (gps.location.isValid()) {
        data = "Latitude: " + String(gps.location.lat(), 6) +
               ", Longitude: " + String(gps.location.lng(), 6);
    }
    return data;
}

// Handle root webpage
void handleRoot() {
    server.send_P(200, "text/html", webpage);
}

// Handle GPS data request
void handleGPS() {
    server.send(200, "text/plain", getGPSData());
}

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200); // Debugging
    delay(1000);

    // Initialize GPS UART
    GPS_Serial.begin(9600); // Use Serial for GPS

    // Start WiFi in AP mode
    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP started");
    Serial.print("SSID: ");
    Serial.println(ssid);

    // Start Web Server
    server.on("/", handleRoot);
    server.on("/gps", handleGPS);
    server.begin();
    Serial.println("Web server started");
}

void loop() {
    // Read GPS data
    while (GPS_Serial.available()) {
        gps.encode(GPS_Serial.read());
    }

    // Handle web server
    server.handleClient();
}

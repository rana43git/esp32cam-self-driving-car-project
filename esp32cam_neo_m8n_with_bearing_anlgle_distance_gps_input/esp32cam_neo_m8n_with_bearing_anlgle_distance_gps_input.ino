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

// Haversine formula to calculate distance
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    const float R = 6371e3; // Earth's radius in meters
    float phi1 = radians(lat1);
    float phi2 = radians(lat2);
    float deltaPhi = radians(lat2 - lat1);
    float deltaLambda = radians(lon2 - lon1);

    float a = sin(deltaPhi / 2) * sin(deltaPhi / 2) +
              cos(phi1) * cos(phi2) *
              sin(deltaLambda / 2) * sin(deltaLambda / 2);
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c; // Distance in meters
}

// Bearing calculation
float calculateBearing(float lat1, float lon1, float lat2, float lon2) {
    float phi1 = radians(lat1);
    float phi2 = radians(lat2);
    float lambda1 = radians(lon1);
    float lambda2 = radians(lon2);

    float y = sin(lambda2 - lambda1) * cos(phi2);
    float x = cos(phi1) * sin(phi2) -
              sin(phi1) * cos(phi2) * cos(lambda2 - lambda1);
    return fmod(degrees(atan2(y, x)) + 360.0, 360.0); // Bearing in degrees
}

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

// Handle distance and bearing calculation
void handleCalculate() {
    if (gps.location.isValid() && server.hasArg("target")) {
        String target = server.arg("target");
        int commaIndex = target.indexOf(',');
        if (commaIndex > 0) {
            float targetLat = target.substring(0, commaIndex).toFloat();
            float targetLon = target.substring(commaIndex + 1).toFloat();

            float currentLat = gps.location.lat();
            float currentLon = gps.location.lng();

            float distance = calculateDistance(currentLat, currentLon, targetLat, targetLon);
            float bearing = calculateBearing(currentLat, currentLon, targetLat, targetLon);

            server.send(200, "text/plain", "Distance: " + String(distance / 1000.0, 2) + " km\nBearing: " + String(bearing, 2) + " degrees");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid request");
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
    server.on("/calculate", handleCalculate);
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
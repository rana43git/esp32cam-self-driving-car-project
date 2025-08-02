const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32-CAM GPS</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
        h1 { color: #333; }
        #gps { font-size: 1.5em; margin-top: 20px; }
    </style>
</head>
<body>
    <h1>ESP32-CAM GPS Data</h1>
    <div id="gps">Loading...</div>
    <script>
        async function fetchGPSData() {
            try {
                const response = await fetch('/gps');
                const data = await response.text();
                document.getElementById('gps').innerText = data;
            } catch (error) {
                document.getElementById('gps').innerText = "Error fetching GPS data!";
            }
        }
        setInterval(fetchGPSData, 1000); // Fetch data every second
        fetchGPSData();
    </script>
</body>
</html>
)rawliteral";
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32-CAM GPS</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; }
        h1 { color: #333; }
        #gps, #result { font-size: 1.5em; margin-top: 20px; }
    </style>
</head>
<body>
    <h1>ESP32-CAM GPS Data</h1>
    <div id="gps">Loading...</div>

    <h2>Calculate Distance and Bearing</h2>
    <input id="target" type="text" placeholder="Latitude,Longitude">
    <button onclick="calculate()">Calculate</button>

    <div id="result">Result will appear here</div>

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

        async function calculate() {
            const target = document.getElementById('target').value;
            if (!target) {
                document.getElementById('result').innerText = "Please enter target coordinates.";
                return;
            }

            try {
                const response = await fetch(`/calculate?target=${encodeURIComponent(target)}`);
                if (response.ok) {
                    const data = await response.text();
                    document.getElementById('result').innerText = data;
                } else {
                    document.getElementById('result').innerText = "Error calculating distance and bearing!";
                }
            } catch (error) {
                document.getElementById('result').innerText = "Error communicating with server!";
            }
        }

        setInterval(fetchGPSData, 1000); // Fetch data every second
        fetchGPSData();
    </script>
</body>
</html>
)rawliteral";

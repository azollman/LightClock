# LightClock
A programmable red-light/green-light alarm clock for ESP8266 (built on NodeMCU devkit) with configuration, time synchronization, and overrides via WiFi.

Functional highlights:
* Default connection to a WiFi network, with fallback to broadcasting its own SSID (great for travel away from your home network)
* NTP synchronization with default (but configurable) timezone offset
* Once alarm time is reached, the light stays green until the clock is reset
* Any state (red, yellow, green, off) can be selected manually from the UI


To use:
* Change the configurables based on your network and wiring to LEDs.
* Flash to the board and load the Bootstrap file from /data into SPIFFS (it just serves a reference if it's connected to an AP, but serves the CSS from SPIFFS if it's broadcasting its own)
* When it connects, either use the Serial readout or it flashes LEDs (red = 100s place, yellow = 10s, green = 1s) to give you the last octet of the IP. All LEDs on = SoftAP mode, IP=192.168.4.1
* Pull the page up in a browser (tested in Chrome & Safari mobile)
* If needed, you can change the timezone offset or manually reset the clock (if it's connected to a network without NTP access, like its own) at the bottom
* Set the alarm by entering a 24hr time, no seconds ("6:00") in the alarm box, and mash the button
* Use the color buttons for overrides. Any color button disables the alarm functionality
Humidity Monitor Source
-----------------------

Build and deployed using build.particle.io

If you paste this into a new file at build.particle.io you will need to manually add the HTU21D library (from the libraries tab).

Watch for the hardware version used. You should be using 0.2 PCB (i.e. Version 2), ensure version 1 pin assignments are commented out.

This application publishes senml messages which are automatically picked up by Tinamous.com to record the humidity/temperature levels and Status messages which show on the timeline.

Note that setting a new fan humidity trigger level using setFanOnAt method (which sets fanOnAtHumidityLevel) remotely does not persist this, it will be lost on power cycle/reset.

The application monitors the humidity level and will run the fan as needed.
* Either every n seconds (10s loop * 60, fan runs for 120s).
* When humidity > fanOnAtHumidityLevel (20%) and < 40% - fan will run for 120s or as long as the humidity level remains in this zone.
* If humidity level > 40% the fan is stopped to prevent flooding the silica gel with moisture (it is assumed to be in open air).

The Photon has a RGB LED on top, the color or this is used to indicate humidity levels
* Red - Humidity > 30%
* Yellow - Humidity > 20%
* Green - Humidity <= 20%

The fan can be remotely controlled via the Particle API (and via Tinamous) using:
* fanOn - runs the fan for 10 minutes
* fanOff - stops the fan within 10 seconds

No power saving is attempted with this version. A 10 second delay happens every loop which would be a good time to switch the WiFi off, however this makes updating and controlling the unit difficult and as it uses external power their is little need for power saving.
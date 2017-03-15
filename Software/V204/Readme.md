Humidity Monitor Source
-----------------------

Build and deployed using build.particle.io

Paste the code from Humidityv4.ino into a new file at build.particle.io you will need to manually add the HTU21D library (from the libraries tab).

This works with the V2.04 PCB

This application publishes senml messages which are automatically picked up by Tinamous.com to record the humidity/temperature levels and Status messages which show on the timeline.

The application monitors the humidity level and will run the fan as needed.
* Either every n seconds (10s loop * 60, fan runs for 20s).
* When humidity > fanOnAtHumidityLevel (20%) and < humidityWarningLevel (30%) - fan will run for 60s or as long as the humidity level remains in this zone.
* If humidity level > humidityWarningLevel (30%) the fan is stopped and the beeper sounded to prevent flooding the silica gel with moisture (it is assumed to be in open air).

The Photon has a RGB LED on top, the color or this is used to indicate humidity levels
* Red - Humidity > 30%
* Yellow - Humidity > 20%
* Green - Humidity <= 20%

The fan can be remotely controlled via the Particle API (and via Tinamous) using:
* fanOn - runs the fan for 10 minutes
* fanOff - stops the fan within 10 seconds
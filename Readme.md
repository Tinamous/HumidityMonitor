Humidity Monitor.

Use Particle Photon or Core device.

Connect HTU21 to I2C bus (D0/D1) and 3v3 and Gnd.

You may need to import the library on Particle. Use HTU21D library.

Build using build.particle.io

Core
----
If you are using a Particle (Spark) Core uncomment the Spark.sleep(1) at the end of the loop.


Photon
------
If you are using a Particle Photon uncomment System.sleep(2) - but not yet, a bug in the firmware kills the WiFi so leave it commented for now!
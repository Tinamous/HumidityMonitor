// **********************************************************************
// Filament storage humidity monitor
// **********************************************************************

// This #include statement was automatically added by the Spark IDE.
#include "HTU21D/HTU21D.h"

int ledSampling = D2; // use led for now, but this is RS232 comms LED.
//int clockSpeed = CLOCK_SPEED_400KHZ;

// Loop counter 
int loopCounter = 0;

// Temperature and humidity
HTU21D humiditySensor;

// Spark variables for tracking humidity and temperature
double humidity = 0;
double temperature = 0;

void setup() {
    pinMode(ledSampling, OUTPUT);
      
    Spark.variable("humidity", &humidity, DOUBLE);
    Spark.variable("temperature", &temperature, DOUBLE);
    
    // Take control of the spark LED and make it dimmer as it's very
    // bright by default.
    RGB.control(true);
    RGB.brightness(10);
    
    // Set blue to show initializing.
    RGB.color(0, 0, 255);
  
    // Setup I2C as master.
    // Appears bug in Particle where CLOCK_SPEED_400KHZ is undefiend.
    // Works with Core but not Photon.
    // For now default to 100KHZ which appears to work for the DTH21 anyway
    // Wire.setSpeed(CLOCK_SPEED_400KHZ);
    Wire.begin();
    
    // Initialize Temperature and humidity
    if (!humiditySensor.begin()) {
        RGB.color(255, 0, 00);
        Spark.publish("Setup", "Humidity Sensor setup failed.");
    }
    
    //delay(1000);
    Spark.publish("Setup", "Setup complete.");
    RGB.color(0, 0, 0);
    delay(2000);
}

void loop() {
    
    digitalWrite(ledSampling, HIGH);
    RGB.color(0, 0, 125);
    Spark.publish("Reading", "Reading...");
    delay(100);
    
    readHumidity();
    readTemperature();
    
    delay(100);
    RGB.color(0, 0, 0);
    digitalWrite(ledSampling, LOW);
    loopCounter = 0;
    
    delay(2000);
     
     // Do not use Sleep as it breaks Photon WiFi!
    // Can we put the Core into low power mode?
    // Photon:
    //System.sleep(2);
    // Core:
    // Spark.sleep(1);
    delay(2000);
} 

void readHumidity() {
    float h = humiditySensor.readHumidity();
    if (h < 100) {
        humidity = (double)h;
        Spark.publish("Humidity", "Measured humidity as: " + String(h));
        if (h < 20) {
            RGB.color(0, 255, 0);
        } else if (h<50) {
            RGB.color(255, 255, 0);
        }
    } else {
        Spark.publish("Error", "Error reading humidity. Reported: " + String(h));
        RGB.color(255, 0, 0);
        delay(1000);
    }
}

void readTemperature() {
    float t = humiditySensor.readTemperature();
    if (t<100) {
        temperature = (double)t;
        Spark.publish("Temperature", "Measured temperature as: " + String(temperature));
    } else {
        Spark.publish("Error", "Error reading temperature. Reported: " + String(t));
        RGB.color(255, 0, 0);
        delay(1000);
    }
}
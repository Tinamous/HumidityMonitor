// **********************************************************************
// Filament storage humidity monitor
// **********************************************************************

// This #include statement was automatically added by the Spark IDE.
#include "HTU21D/HTU21D.h"

int ledSampling = D2; 
int fanPulseIn = D4; // Max pin for Core intterupt
int fanControl  = D7; 

// Temperature and humidity
HTU21D humiditySensor;

// Spark variables for tracking humidity and temperature
double humidity = 0;
double temperature = 0;
int faultCode = 0;
int fanSpeed = 0; // percentage
int fanOverrideSpeed = 0; // User requested fan speed. Overrides auto speed.
int fanPulsesPerInterval = 0; // Non volatile int

int setFanOnAt(String level);
int fanOn(String command);
int fanOff(String command);
int reset(String command);

int fanOnAtHumidityLevel = 20;

// Count of fan pulses
volatile int fanPulseCount = 0;


void setup() {
    pinMode(ledSampling, OUTPUT);
    pinMode(fanPulseIn, INPUT_PULLUP);
    pinMode(fanControl, OUTPUT);
    digitalWrite(ledSampling, LOW);
    digitalWrite(fanControl, LOW);
    
    attachInterrupt(fanPulseIn, fanPulse, FALLING);
      
    Spark.variable("humidity", &humidity, DOUBLE);
    Spark.variable("temperature", &temperature, DOUBLE);
    Spark.variable("faultCode", &faultCode, INT);
    Spark.variable("fanSpeed", &fanSpeed, INT);
    Spark.variable("fanPulses", &fanPulsesPerInterval, INT);
    //Spark.variable("fanOnAt", &fanOnAtHumidityLevel, INT);
    
    // register the cloud function
    Spark.function("setFanOnAt", setFanOnAt);
    Spark.function("fanOn", fanOn);
    Spark.function("fanOff", fanOff);
    Spark.function("reset", reset);

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
        Spark.publish("Status", "Humidity Sensor setup failed.");
    }
    
    //delay(1000);
    Spark.publish("Status", "Setup complete.");
    RGB.color(0, 0, 0);
    delay(2000);
}

void loop() {
    
    digitalWrite(ledSampling, HIGH);
    RGB.color(0, 0, 125);
    //Spark.publish("Reading", "Reading...");
    //delay(100);
    
    // Reset the fault code before measuring.
    faultCode = 0;
    readHumidity();
    // Small delay before re-reading to see if this helps resolve the errors.
    delay(250);
    readTemperature();
    
    RGB.color(0, 0, 0);
    digitalWrite(ledSampling, LOW);
    
    // Store the fan pulses during this loop
    fanPulsesPerInterval = fanPulseCount;
    fanPulseCount = 0;
    
     // Do not use Sleep as it breaks Photon WiFi!
    // Can we put the Core into low power mode?
    // Photon:
    //System.sleep(2);
    // Core:
    // Spark.sleep(1);
    // Sleep for x seconds.
    delay(15000);
} 

void readHumidity() {
    float h = humiditySensor.readHumidity();
    if (h < 100) {
        humidity = (double)h;
        Spark.publish("Humidity", "Measured humidity as: " + String(h));
        updateControl(humidity);
    } else {
        faultCode = h;
        Spark.publish("Error", "Error reading humidity. Reported: " + String(h));
        RGB.color(255, 0, 0);
        // Force a delay to try and help I2C
        delay(3000);
        //reset("Humidity Fault");
    }
}

void readTemperature() {
    float t = humiditySensor.readTemperature();
    if (t<100) {
        temperature = (double)t;
        Spark.publish("Temperature", "Measured temperature as: " + String(temperature));
    } else {
        faultCode = t;
        Spark.publish("Error", "Error reading temperature. Reported: " + String(t));
        RGB.color(255, 0, 0);
        // Force a delay to try and help I2C
        delay(3000);
        //reset("Temperature Fault");
    }
}

void updateControl(float humidity) {
    // Use the global fanSpeed variable.
    
    int newFanOnCount = fanSpeed;
    
    if (humidity < fanOnAtHumidityLevel) {
        // Humidity is in range. Do nothing
        RGB.color(0, 255, 0);
        // Humidity is below required level
        // decrease the number of itterations the fan needs to be on for.
        newFanOnCount--; 
    } else if (humidity < 40) { // Do we need this settabke?
        // Humidity is high so put fan on, but low.
        RGB.color(255, 128, 0);
        // Run the fan for 4 itterations of the main loop (about 1 minute).
        // on top of how long it takes to get below thresold.
        newFanOnCount = 4;
        // TODO: Turn the fan on.
    } else  {
        // humidity is very high, fan on full speed.
        RGB.color(255, 0, 128);
        newFanOnCount = 12;
        //fanSpeed = 100;
    }
    
    if (newFanOnCount < 0) {
        newFanOnCount = 0;
    }
    
    setFan(newFanOnCount);
}

// Particle Method
int setFanOnAt(String level) {
    // This relies on level being a single numeric value.
    fanOnAtHumidityLevel = level.toInt();
    Spark.publish("Status", "Setting fan trigger humidity level at: " + String(fanOnAtHumidityLevel) + "%, request was : '" + level + "' ");
    return 0;
}

// Particle Method
int fanOn(String command)
{
    Spark.publish("Status", "Switching fan on to 100%.");
    fanOverrideSpeed = 100;
    // On for 10 mins (4 itterations per min)
    setFan(10 * 4);
    return fanOverrideSpeed;
}

// Particle Method
int fanOff(String command)
{
    Spark.publish("Status", "Switching fan off.");
    fanOverrideSpeed = 0;
    // On for 0 itterations
    setFan(0);
    return fanOverrideSpeed;
}

// Particle Method
int reset(String args) {
    // Photon only?
    Spark.publish("Status", "I'm going to reset in 10 seconds. Reason: " + args);
    delay(10000);
    System.reset();
    return 0;
}

// Use the number of cycles that the fan should be on for.
void setFan(int fanCount) {
    
    int newfanSpeed = fanCount; //max(speed, fanOverrideSpeed);
    
    if (newfanSpeed != fanSpeed) {
        Spark.publish("Status", "Setting fan speed to:" + String(newfanSpeed));
    }
    
    // Store in the Particle variable
    fanSpeed = newfanSpeed;
    
    // TODO: Use AnalogWrite when fan connected to it.
    if (fanSpeed > 0) {
        digitalWrite(fanControl, HIGH);
    } else {
        digitalWrite(fanControl, LOW);
    }
}

void fanPulse() {
    fanPulseCount++;
}
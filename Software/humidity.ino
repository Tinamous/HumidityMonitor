// **********************************************************************
// Filament storage humidity monitor
// **********************************************************************

// If you are pasting this into build.particle.io you will need to manually
// add the HTU21D library.

// This #include statement was automatically added by the Spark IDE.
#include "HTU21D/HTU21D.h"

// Version 1 (PlaFilamentBox & FilamentBox2)
//int fanPulseIn = D4;
//int fanControl  = D7; 

// Version 2 (board 0.2) (with the buzzer) (AbsFilamentBox & LooseFilamentBox)
int fanPulseIn = D4;
int fanControl  = D3;
int buzzer = D2;

// Temperature and humidity
HTU21D humiditySensor;

// Public exposed  variables for tracking humidity and temperature
double humidity = 0;
double temperature = 0;
int faultCode = 0;
int fanSpeed = 0; // percentage
int fanPulsesPerInterval = 0; // Non volatile int

// Public exposed methods
int setFanOnAt(String level);
int fanOn(String command);
int fanOff(String command);
int resetPhoton(String command);

// Default, run the fan when humidity > 20%
int fanOnAtHumidityLevel = 20;

// Count of fan pulses
volatile int fanPulseCount = 0;

bool highHumidity = false;
int publishCount = 0;


void setup() {
    pinMode(fanPulseIn, INPUT_PULLUP);
    pinMode(fanControl, OUTPUT);
    digitalWrite(fanControl, LOW);
    
    attachInterrupt(fanPulseIn, fanPulse, FALLING);
      
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("temperature", &temperature, DOUBLE);
    Particle.variable("faultCode", &faultCode, INT);
    Particle.variable("fanPulses", &fanPulsesPerInterval, INT);
    Particle.variable("fanOnAt", &fanOnAtHumidityLevel, INT);
    
    // register the cloud function
    Particle.function("setFanOnAt", setFanOnAt);
    Particle.function("fanOn", fanOn);
    Particle.function("fanOff", fanOff);
    Particle.function("reset", resetPhoton);

    // Take control of the spark LED and make it dimmer as it's very
    // bright by default.
    RGB.control(true);
    RGB.brightness(100);
    
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
        Particle.publish("Status", "Humidity Sensor setup failed.");
    }
    
    Particle.publish("Status", "Setup complete. V2.1.0");
    RGB.color(0, 0, 0);
    delay(1000);
}

void loop() {
    
    // Indicate blue for reading.
    RGB.color(0, 0, 255);
    
    // Reset the fault code before measuring.
    faultCode = 0;
    
    // Read measurements
    readHumidity();
    readTemperature();
    
    // Update control/output based on measurements
    updateFanControl();
    setLedForHumidityLevel();
    updateTinamous();
    
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
    delay(10000);
} 

void updateTinamous() {
    // Only publish the 
    if (humidity > fanOnAtHumidityLevel && !highHumidity) {
        highHumidity = true;
        Particle.publish("Status", "High Humidity");
    }
    
    if (humidity < fanOnAtHumidityLevel) {
        highHumidity = false;
    }
    
    // Every minute.
    if (publishCount >= 0) {
        Particle.publish("senml", "{'e': [{'n':'Humidity', 'v':" + String(humidity) + "},{'n':'temperature','v':" + String(temperature) + "}]}");
        publishCount = 0;
    }
    publishCount++;
}


void setLedForHumidityLevel() {
    // If > 30, Red - very high humidity
    // if > 20, higher than wanted
    // Otherwise Green - all is well...
    if (humidity > 30) {
        RGB.color(255, 0, 0);
        RGB.brightness(100);
    } else if (humidity > 20) {
        RGB.color(255, 255, 0);
        RGB.brightness(100);
    } else {
        RGB.color(0, 255, 0);
        RGB.brightness(50);
    }
}

void readHumidity() {
    float h = humiditySensor.readHumidity();
    if (h < 100) {
        humidity = (double)h;
    } else {
        faultCode = h;
    }
}

void readTemperature() {
    float t = humiditySensor.readTemperature();
    if (t<100) {
        temperature = (double)t;
    } else {
        faultCode = t;
    }
}

void updateFanControl() {
    // Use the global fanSpeed variable.
    
    int newFanOnCount = fanSpeed;
   
    if (humidity > 40 ) {
        // If measured humidity is above 40 then don't run the fan as the unit is most likelu in free air
        // and this will ruin the gel pack.
        newFanOnCount = 0;
        
        // The humidity will gradually drop once the unit is in a controlled environment
        // and then the fan can resume 
        Particle.publish("Status", "Humidity > 40%. Stopping fan to prevent saturating gel");
    } else if (humidity < fanOnAtHumidityLevel) {
        // Humidity is in range. Do nothing
        //RGB.color(0, 255, 0);
        // Humidity is below required level
        // decrease the number of itterations the fan needs to be on for.
        newFanOnCount--; 
    } else  {
        // humidity is  high, switch the fan on for 2 minutes
        //RGB.color(255, 0, 128);
        newFanOnCount = 12;
    }
    
    // Every good cycle causes count to be decremented. When 0 or less fan is switched off.
    // 10 mins off, 2 mins on.
    if (newFanOnCount < -60) {
        newFanOnCount = 12;
    }
    
    setFan(newFanOnCount);
}

// Particle Method
int setFanOnAt(String level) {
    // This relies on level being a single numeric value.
    fanOnAtHumidityLevel = level.toInt();
    Particle.publish("Status", "Setting fan trigger humidity level at: " + String(fanOnAtHumidityLevel) + "%, request was : '" + level + "' ");
    return 0;
}

// Particle Method
int fanOn(String command)
{
    Particle.publish("Status", "Switching fan on for 10 minuites.");
    // On for 10 mins (6 itterations per min)
    setFan(10 * 6);
    return 10*6;
}

// Particle Method
int fanOff(String command)
{
    Particle.publish("Status", "Switching fan off.");
    
    // On for 0 itterations
    setFan(0);
    return 0;
}

// Particle Method
int resetPhoton(String args) {
    // Photon only?
    Particle.publish("Status", "I'm going to reset in 10 seconds. Reason: " + args);
    delay(10000);
    System.reset();
    return 0;
}

// Use the number of cycles that the fan should be on for.
void setFan(int fanCount) {
    
    int newfanSpeed = fanCount; //max(speed, fanOverrideSpeed);
    
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
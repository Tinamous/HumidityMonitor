// This #include statement was automatically added by the Particle IDE.
#include <HTU21D.h>

// **********************************************************************
// Filament storage humidity monitor V2.04 PCB (Traffic lights version).
// **********************************************************************

int fanPulseIn = D4;
int fanControl  = D3;
int buzzer = DAC;
int highHumidityLed = A3;
int fanLed = A4;
int oshLed = A5;

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
int setWarnAt(String level);
int fanOn(String command);
int fanOff(String command);
int beep(String command);

// Default, run the fan when humidity > 20%
int fanOnAtHumidityLevel = 20;
int humidityWarningLevel = 30;

// Count of fan pulses
volatile int fanPulseCount = 0;

bool highHumidity = false;
int beepedCount =0;
int publishCount = 0;


void setup() {
    
    // First time deployment only
    // Address 0: Hardware version
    //EEPROM.write(0, 2);
    //EEPROM.write(1, fanOnAtHumidityLevel);
    //EEPROM.write(2, humidityWarningLevel);
    
    // Take control of the spark LED and make it dimmer as it's very
    // bright by default.
    RGB.control(true);
    RGB.brightness(100);
    
    // Set blue to show initializing.
    RGB.color(0, 0, 255);
    
    pinMode(fanPulseIn, INPUT_PULLUP);
    pinMode(fanControl, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(highHumidityLed, OUTPUT);
    pinMode(fanLed, OUTPUT);
    pinMode(oshLed, OUTPUT);
    
    digitalWrite(fanControl, LOW);
    digitalWrite(buzzer, LOW);
    digitalWrite(highHumidityLed, HIGH);
    digitalWrite(fanLed, HIGH);
    digitalWrite(oshLed, HIGH);
    
    // Load in the settings
    fanOnAtHumidityLevel = EEPROM.read(1);
    if (fanOnAtHumidityLevel == 0xFF) {
        fanOnAtHumidityLevel = 20;
    }
    
    humidityWarningLevel = EEPROM.read(2);
    if (humidityWarningLevel == 0xFF) {
        humidityWarningLevel = 30;
    }
    Particle.publish("Status", "Setting FanOn at: " + String(fanOnAtHumidityLevel) + "%, WarnAt: " + String(humidityWarningLevel) + "%");
    delay(1000);
    
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
        delay(2000);
    }
      
    Particle.variable("humidity", &humidity, DOUBLE);
    Particle.variable("temperature", &temperature, DOUBLE);
    Particle.variable("faultCode", &faultCode, INT);
    Particle.variable("fanPulses", &fanPulsesPerInterval, INT);
    Particle.variable("fanOnAt", &fanOnAtHumidityLevel, INT);
    Particle.variable("warnAt", &humidityWarningLevel, INT);
    
    // register the cloud function
    Particle.function("setFanOnAt", setFanOnAt);
    Particle.function("setWarnAt", setWarnAt);
    Particle.function("fanOn", fanOn);
    Particle.function("fanOff", fanOff);
    Particle.function("beep", beep);
    
    attachInterrupt(fanPulseIn, fanPulse, FALLING);
    
    doBeep(2);
    Particle.publish("Status", "Setup complete. V4.1.1");
    RGB.color(0, 0, 0);
    delay(250);
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
    beepIfNeeded();
    updateTinamous();
    
    // Store the fan pulses during this loop
    fanPulsesPerInterval = fanPulseCount;
    fanPulseCount = 0;
    
    delay(10000);
} 

void updateTinamous() {
    // Only publish the 
    if (humidity > humidityWarningLevel && !highHumidity) {
        highHumidity = true;
        Particle.publish("Status", "High Humidity. Fan will not be run at this level.");
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

    digitalWrite(highHumidityLed, LOW);
    digitalWrite(oshLed, LOW);
    
    // If > 30, Red - very high humidity
    // if > 20, higher than wanted
    // Otherwise Green - all is well...
    if (humidity > humidityWarningLevel) {
        RGB.color(255, 0, 0);
        RGB.brightness(100);
        digitalWrite(highHumidityLed, HIGH);
    } else if (humidity > 20) {
        RGB.color(255, 255, 0);
        RGB.brightness(100);
        digitalWrite(highHumidityLed, HIGH);
    } else {
        RGB.color(0, 255, 0);
        RGB.brightness(50);
        digitalWrite(oshLed, HIGH);
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
   
    if (humidity > humidityWarningLevel ) {
        // If measured humidity is above 30 (humidityWarningLevel) then
        // don't run the fan as the unit is most likely in free air
        // and this will ruin the silica gel quickly.
        newFanOnCount = 0;
        
        // The humidity will gradually drop once the unit is in a controlled environment
        // and then the fan can resume 
        Particle.publish("Status", "Humidity (" + String(humidity) + ")> " + String(humidityWarningLevel) + "%. Stopping fan to prevent saturating gel");
    } else if (humidity > fanOnAtHumidityLevel) {
        // humidity is higher than we would like. Between fanOnAt and humidityWarningLevel
        // switch the fan on for n-minutes
        newFanOnCount = 6;
    } else  {
        // Humidity is below the level required to need the fan to run
        // so do nothiung.
    }
    
    // Every good cycle causes count to be decremented. When 0 or less fan is switched off.
    // 10 mins off, 20 seconds on.
    if (newFanOnCount < -60) {
        Spark.publish("Status", "Running fan for general air refresh");
        newFanOnCount = 2;
    }
    
    setFan(newFanOnCount);
    
    // decrease the number of itterations the fan needs to be on for.
    newFanOnCount--; 
}

void beepIfNeeded() {
    // Beep a max of n-times when the humidity goes out of range
    // Once it's back in range reset the counter.
    if (humidity > humidityWarningLevel) {
        if (beepedCount<5) {
            doBeep(5);
            beepedCount++;
        } else {
            /// Silent beep beep beep to stop it being annoying
        }
    } else {
        beepedCount = 0;
    }
}

// Particle Method
int setFanOnAt(String level) {
    // This relies on level being a single numeric value.
    fanOnAtHumidityLevel = level.toInt();
    EEPROM.write(1, fanOnAtHumidityLevel);
    Particle.publish("Status", "Setting fan trigger humidity level at: " + String(fanOnAtHumidityLevel) + "%");
    return fanOnAtHumidityLevel;
}

int setWarnAt(String level) {
    humidityWarningLevel = level.toInt();
    EEPROM.write(2, humidityWarningLevel);
    Particle.publish("Status", "Setting humidity waring level at: " + String(humidityWarningLevel) + "%");
    return humidityWarningLevel;
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

int beep(String command)
{
    doBeep(command.toInt());
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
        digitalWrite(fanLed, HIGH);
    } else {
        digitalWrite(fanControl, LOW);
        digitalWrite(fanLed, LOW);
    }
}

// Beep to indicate a problem (and flash the red LED)
void doBeep(int count) {
    for(int i=0; i<count; i++) {
        digitalWrite(highHumidityLed, LOW);
        for(int j=0; j<150; j++) {
            digitalWrite(buzzer, HIGH);
            delay(1);
            digitalWrite(buzzer, LOW);
            delay(1);
        }
        delay(100);
        digitalWrite(highHumidityLed, HIGH);
        delay(100);
    }
    Particle.publish("Status", "Beep, Beep, Beep...");
}

void fanPulse() {
    fanPulseCount++;
}
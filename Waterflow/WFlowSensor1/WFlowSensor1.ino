//EC Sensor library setup
#include <Wire.h>

/********************************************************************/
// ONE WIRE DS18B20 LIBRARY AND DEFINITION SETUP
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 4 on the Arduino 
#define ONE_WIRE_BUS 4 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 

// WATER FLOW SENSORS SETUP

byte sensorInterrupt1 = 0;  // 0 = digital pin 2  FOR FIRST FLOW SENSOR
byte sensorInterrupt2 = 1;  // 1 = digital pin 3  FOR SECOND FLOW SENSOR
byte sensorPin1       = 2;
byte sensorPin2       = 3;

//SETUP OF EC SENSOR PINS
const int analogInPin = A0;  // Analog input pin that the EC sensor output is attached to
//Setup of EC value variables
int ECsensorValue = 0;        // value read from the sensor
int ECoutputValue = 0;        // value output to the PWM (analog out)

// The hall-effect flow sensor outputs approximately 4.8 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.8;

volatile byte pulseCount1;
volatile byte pulseCount2;  

float flowRate1;
float flowRate2;
unsigned int flowMilliLitres1;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres1;
unsigned long totalMilliLitres2;

unsigned long oldTime;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);

  // Start up the library for DS18B20 sensor
  sensors.begin(); 
  
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  digitalWrite(sensorPin1, HIGH);
  digitalWrite(sensorPin2, HIGH);

  pulseCount1        = 0;
  pulseCount2        = 0;
  flowRate1          = 0.0;
  flowRate2          = 0.0;
  flowMilliLitres1   = 0;
  flowMilliLitres2   = 0;
  totalMilliLitres1  = 0;
  totalMilliLitres2  = 0;
  oldTime            = 0;

  // The Hall-effect sensor is connected to pin 2 and 3 which uses interrupt 0 and 1 respectively.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt1, pulseCounter1, FALLING);
  attachInterrupt(sensorInterrupt2, pulseCounter2, FALLING);
}

/**
 * Main program loop
 */
void loop()
{
   
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt1);
    detachInterrupt(sensorInterrupt2);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate1 = ((1000.0 / (millis() - oldTime)) * pulseCount1) / calibrationFactor;
    flowRate2 = ((1000.0 / (millis() - oldTime)) * pulseCount2) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres1 += flowMilliLitres1;
    totalMilliLitres2 += flowMilliLitres2;
      
    unsigned int frac;

    // read the EC analog in value:
    ECsensorValue = analogRead(analogInPin);            
    // map it to the range of the analog out:
    ECoutputValue = map(ECsensorValue, 0, 1023, 0, 5000);
    Serial.print("ECsensor = " );
    Serial.print(ECoutputValue);
    

     // Getting Water temperature and printing to Console
    sensors.requestTemperatures();
    Serial.print("\t WTemp: "); 
    Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
    Serial.print("\t");       // Print tab space
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Qi: ");
    Serial.print(int(flowRate1));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Vi: ");        
    Serial.print(totalMilliLitres1);
    Serial.print("mL"); 
    Serial.print("\t");       // Print tab space
    
    Serial.print("Qo: ");
    Serial.print(int(flowRate2));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t"); // Print tab space

    Serial.print("Vo: ");        
    Serial.print(totalMilliLitres2);
    Serial.println("mL");
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount1 = 0;
    pulseCount2 = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt1, pulseCounter1, FALLING);
    attachInterrupt(sensorInterrupt2, pulseCounter2, FALLING);
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter1()
{
  // Increment the pulse counter
  pulseCount1++;
}

void pulseCounter2()
{
  // Increment the pulse counter
  pulseCount2++;
}

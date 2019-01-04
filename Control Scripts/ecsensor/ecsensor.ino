#include <Wire.h>

//Arduino Conductivity Sensor
//
//Plug and play conductivity sensor for Arduino  is a convenient way  to measure water salinity . 
//Analog unit receives power from Arduino's 5v power supply and provides 0-5v output to any of it's analog inputs.
//Conductivity sensor with 1 meter cable and din connector equipped with temperature compensation.
//Unit has calibration trim pot.
//Unit has a 3 wire cable attached to din connector.
//
//Specifications
//
//Power :  5vdc  with power LED
//Range : 0-5 EC or (0-5000 micro/S) 
//Accuracy: .2% error .Works perfect  from 0-4 EC, Starts losing accuracy after 4000 micro/S due to power limitations.
//Temperature compensation: Yes
//EC electrode: k=1 , submersible/ inline with 1 meter cable
//Size : 3 " x  2" ABS enclosure
//
//Example sketch 
//This example code is in the public domain.
// 
// 

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the sensor output is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the sensor
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 5000);  
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);           

  // print the results to the serial monitor:
  Serial.print("ECsensor = " );                       
  Serial.print(sensorValue);      
  Serial.print("\t output = ");      
  Serial.println(analogRead(1)* 5.00, 2);

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(500);                     
}

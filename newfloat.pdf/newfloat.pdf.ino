//IF THE READING VALUE IS 1, RESERVOIR NEEDS TO BE REFILLED.

int inPin = 2;         // the number of the input pin
int outPin = 13;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 1000;   // the debounce time, increase if the output flickers

void setup()
{
  Serial.begin(9600);
  pinMode(inPin, INPUT);
  pinMode(outPin, OUTPUT);
}

void loop(){
  reading = digitalRead(inPin);
  

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH) {
    state = HIGH;
  }  
    else{
      state = LOW;
    }

    time = millis();
    Serial.println(state);      
 }  
  //digitalWrite(outPin, state);

  //previous = reading;
//}

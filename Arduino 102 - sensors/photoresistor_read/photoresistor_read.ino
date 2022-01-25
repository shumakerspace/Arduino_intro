/*
  PHOTORESISTOR

  Read a photoresistor (light sensor) to detect "darkness" and turn on an LED when it
  is "dark" and turn back off again when it is "bright.
*/


// creating names for our pins for ease of reading the code
const int sensorPin = 0;
const int ledPin = 9;

// We'll also set up some global variables for the light level a calibration value and
//and a raw light value
int lightCal;
int lightVal;


void setup()
{
  // We'll set up the LED pin to be an output.
  pinMode(ledPin, OUTPUT);
  lightCal = analogRead(sensorPin);
  //we will take a single reading from the light sensor and store it in the lightCal
  //variable. This will give us a prelinary value to compare against in the loop
}


void loop()
{
  //Take a reading using analogRead() on sensor pin and store it in lightVal
  lightVal = analogRead(sensorPin);


  //if lightVal is less than our initial reading (lightCal) minus 50 it is dark and
  //turn pin 9 HIGH. The (-50) part of the statement sets the sensitivity. The smaller
  //the number the more sensitive the circuit will be to variances in light.
  if (lightVal < lightCal - 50)
  {
    digitalWrite(9, HIGH);
  }

  //else, it is bright, turn pin 9 LOW
  else
  {
    digitalWrite(9, LOW);
  }

}
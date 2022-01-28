/**
 * Speedometer - SHU Engineering EV Go-Kart
 * 
 * This code is meant for our EV GO-Kart
 * It uses an optocoupler to measure the speed with a hollowed out disk.
 * 
 * 
 * http://www.cesarebrizio.it/Arduino/Speedometer.html
 * http://tronixstuff.com/tutorials > chapter 37
 *   
 *   
 */
 
// include the library code:
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
 
float start, finished;
float elapsed, time;
float circMetric=0.50; // wheel circumference (in meters)
float circImperial; // using 1 kilometer = 0.621371192 miles
float speedk, speedm;    // holds calculated speed vales in metric and imperial
 
void setup()
{
  // convert metric to imperial for MPH calculations
  circImperial=circMetric*.62137; 

  // the syntax with digitalPinToInterrupt should allow portability
  //among different Arduino models - see https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  attachInterrupt(digitalPinToInterrupt(2), speedCalc, RISING); // interrupt called when sensors sends digital 2 high (every wheel rotation)
  //attachInterrupt(0, speedCalc, RISING); // interrupt called when sensors sends digital 2 high (every wheel rotation)

  //start now (it will be reset by the interrupt after calculating revolution time)
  start=millis(); 

  // Print a transitory message to the LCD.
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("SHU - SPEEDOMETER");
  delay(1000); //just to allow you to read the initialization message

  // top serial dialogue speed improves precision
  Serial.begin(9600);
}

//Function called by the interrupt attached to the sensor
// It is called once every detection of a "hole" on the speedometer wheel.
void speedCalc()
{
  if((millis()-start)>10) // 10 millisec debounce
    {
    elapsed=millis()-start;               // calculate elapsed time
    start=millis();                       // reset start
    speedk=(3600*circMetric)/elapsed;     // calculate speed in km/h
    speedm=(3600*circImperial)/elapsed;   // calculate speed in mph
    }
}
 
void loop()
{
  // The loop will be interrupted by the sensor each time the 
  // magnet passes near the sensor, in other words once per revolution

  // Top line in the 16 char, 2 lines display - speed data
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print(int(speedk));
  lcd.print(" km/h ");
  lcd.print(int(speedm));
  lcd.print(" MPH ");

  //bottom line the 16 char, 2 lines display - time data
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(int(elapsed));
  lcd.print(" ms/rev      ");

  // adjust for personal preference to minimise flicker
  delay(250); 
}

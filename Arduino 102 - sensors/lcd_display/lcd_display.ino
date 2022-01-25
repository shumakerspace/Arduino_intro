/*
 * LCD screen - distance measurer 
 */
 
 #include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const int trigPin = 3;
const int echoPin = 2;
long duration;
int distanceCm, distanceInch;
void setup() {
lcd.init(); 
lcd.backlight();
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.clear();
lcd.print("CSE125"); // Prints string "Distance" on the LCD
delay(1000);
}
void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
distanceInch = duration*0.0133/2;
lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distance: "); // Prints string "Distance" on the LCD
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print("  cm");
delay(10);
lcd.setCursor(0,1);
lcd.print("Distance: ");
lcd.print(distanceInch);
lcd.print("inch");
delay(10);
}

/**
 * 
 * Arduino coding challenges based on 8 leds and two buttons
 * 
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 14
#define WALLWIDTH 8
#define WALLHEIGHT 32
#define WALLSIZE (WALLWIDTH*WALLHEIGHT)
#define WALLBRIGHTNESS 2


Adafruit_NeoPixel strip = Adafruit_NeoPixel(256, PIN, NEO_GRB + NEO_KHZ800);


// Fill the dots one after the other with a color
void fillup(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Lights up the leds like a christmas light (alternating odds/evens)
void christmas(uint32_t c, uint8_t wait) {
  uint8_t odd_even = 0;
  while(1){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
       if( i % 2 == odd_even ){ strip.setPixelColor(i, c); }
      }
      strip.show();
      delay(wait);
      if(odd_even){odd_even=0;}else{odd_even=1;}
      strip.clear();
  }
}

// Lights up one led from left to right and then stacks it at the end until everything is on
void stacking(uint32_t c, uint8_t wait) {
  uint16_t numleds = strip.numPixels();
  uint16_t stacks[numleds];
  Serial.println(numleds);

  for(uint16_t s=1; s<=numleds; s++){
    stacks[s]=0;
  }
  stacks[numleds]=1;

  while(1){
      for(uint16_t i=1; i<=numleds; i++) {
        strip.clear();
        strip.setPixelColor(i, c);

        if(stacks[i+1]==1){stacks[i]=1;}

        for(uint16_t s=1; s<=numleds; s++){
          if(stacks[s]==1){strip.setPixelColor(s, c);}
        }
        
        strip.show();
        delay(wait);
      }
   }
}


// Draws a full width row
void row(int rownum, uint32_t c = strip.Color(255, 0, 0)){
  //strip.clear();
  // calculate starting point of the column
  int start;
  start = rownum*WALLWIDTH;
    for(int n=start; n<start+WALLWIDTH; n++){
      strip.setPixelColor(n, c);
    }
}

// Draws a full height column 
void col(int colnum){
  //strip.clear();
  // calculate starting point of the odd columns
    for(int k=0; k<=WALLHEIGHT; k+=2){
  int go,go2;
    go = (colnum+(k*WALLWIDTH));
    go2 = go-(2*colnum+1);
    strip.setPixelColor(go, strip.Color(255, 255, 255));
    if(go2>0){
      strip.setPixelColor(go2, strip.Color(255, 255, 255));
    }
  }
}

// Fills up the led matrix based on the distance from an ultrasonic sensor
void distance() {
// defines variables
uint8_t echoPin = 2; // attach pin D2 Arduino to pin Echo of HC-SR04
uint8_t trigPin = 3; //attach pin D3 Arduino to pin Trig of HC-SR04
pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
pinMode(12, OUTPUT); // POWER PIN HACK
digitalWrite(12,1); // POWER PIN HACK
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

Serial.println("Distance meter using a pixel matrix");

while(1){
  
  // MEASURING THE DISTANCE
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // DISPLAY ON THE LED MATRIX
  strip.clear();
  int numrows = map(distance, 0, 100, 0, 31);
  for(int i=0; i<=numrows; i++){
    row(i);
  }
  strip.show();  
}
}

// Binary counter over 8 bits (0 to 255)
void binary(){

while(1){
  // For each of the 8 bits let's claculate 
  for(int num=1; num<=256; num++){
    Serial.print(num);
    Serial.print(" : ");
    strip.clear();
    // for each bit, calculate if on or off
    int cur=num;
    for(int b=7; b>=0; b--){
      if(cur/pow(2,b)>=1){
        strip.setPixelColor(b, strip.Color(255, 255, 255));
        // row(b*4);
        // row(b*4+1);
        // row(b*4+2);
        // row(b*4+3);
        cur-=pow(2,b);
        Serial.print("X");
      }else{
        strip.setPixelColor(b, strip.Color(0, 0, 0));
        Serial.print("0");
      }
    }
    strip.show();
    Serial.println();
    delay(200);
  }
}
}


void flag(){
  strip.clear();

  // Calculate the reminder when you can't divide the screen in 3 parts perfectly
  int reminder = WALLHEIGHT%3;
  for(int num=0; num<WALLHEIGHT-1; num+=(WALLHEIGHT/3)){
  Serial.print("----------- ");
  Serial.println(num);
    for(int b=num; b<=num+(WALLHEIGHT/3); b++){
        switch (num) {
          case 0:
            row(b, strip.Color(0, 0, 255));
            Serial.println(b);
            break;
          case 10:
            row(b, strip.Color(255, 255, 255));
            Serial.println(b);
            break;
          case 20:
            row(b, strip.Color(255, 0, 0));
            if(reminder==1){ row(b+1, strip.Color(255, 0, 0)); num+=1;} // When you can;t divide the matrix in 3, we spread the 
            Serial.println(b);
            break;
          default:
            row(b, strip.Color(0, 255, 0));
            Serial.println(b);
            break;
        }
     }
  }
  strip.show();
  delay(10000);
}

// Dims an led
void ledDim(){
  pinMode(9, OUTPUT);

  analogWrite(9, 100); // Why is 100 not full brightness?
}

// Dims an led based on the read from a pot
void potDim(){
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);

  int potVal = analogRead(A0); // value will be 0-255 (8 bit)
  analogWrite(9, potVal); // this pin has to be a PWM pin
  delay(1); // what happens if you increase this?
}

// Dims an led based on the read from a pot + sends the % value over serial
void ledSerial(){
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);
  
  int potVal = analogRead(A0); // value will be 0-255 (8 bit)
  analogWrite(9, potVal); // this pin has to be a PWM pin
  Serial.print("Pot value:");
  int potPer = map(potVal, 0, 255, 0, 100); // transforms the value in % from the 8 bit (0-255 to 0-100)
  Serial.print(potPer);
  Serial.println("%");
  delay(1000);
}

// User inputs the % of the 
void ledSerialRead(){
  int incomingByte = 0; // for incoming serial data
  // send data only when you receive data:
  Serial.println("Please enter led brightness percentage:");

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    
    int ledBrightness = map(incomingByte, 0, 100, 0, 255); // transform this into an integer from the read and map it from percentage (0-100) to the 8bit (0-255)
    analogWrite(9, ledBrightness); // this pin has to be a PWM pin

    // say what you got:
    Serial.print("Chosen brightness: ");
    Serial.print(incomingByte, DEC);
    Serial.println("%");
  }
}

// chatbot between two arduinos 
void chatBotSend(){
  char c = ' ';

  while(1){
    c = Serial.read();
    if(c==-1){
      Serial.print(c);
      delay(500);
    } else { 
      Serial.print(c);
      delay(500);
    }
  }
}

void setup() {
  strip.begin();
  strip.setBrightness(WALLBRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  // Choose which function you want to run:

  // fillup(strip.Color(255, 0, 0), 5); 
  // christmas(strip.Color(255, 255, 255), 2000); 
  // stacking(strip.Color(255, 0, 0), 1);
  // col(3); strip.show();
  // row(3, strip.Color(255, 255, 255)); strip.show();
  // distance();
  // binary();
  // flag();
  // ledDim();
  // potDim();
  // ledSerial();
  // ledSerialRead();
  chatBotSend();
}

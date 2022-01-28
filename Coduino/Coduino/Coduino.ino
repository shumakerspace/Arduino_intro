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


void setup() {
  strip.begin();
  strip.setBrightness(1);
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // fillup(strip.Color(255, 0, 0), 5); 
  // christmas(strip.Color(255, 255, 255), 2000); 
   stacking(strip.Color(255, 0, 0), 10); 
}

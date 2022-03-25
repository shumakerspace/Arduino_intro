#include <SPI.h>
#include "LowPower.h"

#define LEFT_BUTTON 2
#define RIGHT_BUTTON 3

#define EPPROM_ADDR    100
#define EPPROM_OK      0xA5
#define WIFICHANEL     108
#define WIFIPIPE      0

unsigned long wakeUP;
unsigned long gulBtnTime;
unsigned long gucBtnShortTime1;
unsigned long gucBtnLongTime1;
unsigned long gucBtnShortTime2;
unsigned long gucBtnLongTime2;
unsigned char gucLeftLastSts;
unsigned char gucRightLastSts;
unsigned char gucLeftSts = HIGH;
unsigned char gucRightSts = HIGH;
unsigned char gucLeftLongSts;
unsigned char gucLeftLongActived;
unsigned char gucRightLongSts;
unsigned char gucRightLongActived;
unsigned long ulRetWakeUp;

void setup()
{
  fncSlaveSetup(WIFIPIPE, WIFICHANEL);
  delay(1000);
  Serial.begin(9600);
  pinMode(LEFT_BUTTON, INPUT_PULLUP); 
  pinMode(RIGHT_BUTTON, INPUT_PULLUP); 
  wakeUP = 0;
}

/* Main loop */
void loop()
{   

    if(wakeUP == 0){
      Serial.println("SLEEP");
      Serial.flush();
      detachInterrupt(digitalPinToInterrupt(LEFT_BUTTON)); 
      detachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON));        
      attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON), wakeMeUp, RISING);
      attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON), wakeMeUp, RISING);
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
      detachInterrupt(digitalPinToInterrupt(LEFT_BUTTON)); 
      detachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON));      
    }
    attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON), wakeMeUp, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON), wakeMeUp, RISING);
      
    fncCheckButton();
    
}

void wakeMeUp(){
    wakeUP = 1;
    ulRetWakeUp = millis() + 5000;
}


void fncCheckButtonInit(void){
    unsigned long ulRet;
    
    ulRet = millis();
    gulBtnTime = ulRet + 10;
    gucBtnShortTime1 = ulRet + 100;
    gucBtnLongTime1 = ulRet + 1000;

    gucLeftLastSts = digitalRead(LEFT_BUTTON);
    gucRightLastSts = digitalRead(RIGHT_BUTTON);

    gucLeftLongActived = false;
    gucRightLongActived = false;
    
}

void fncCheckButton(void){
    unsigned long ulRet;
    unsigned char ucLeftBnt, ucRightBnt;
    ulRet = millis();
    
    if(ulRet > gulBtnTime){
        gulBtnTime = ulRet + 10;
        ucLeftBnt = digitalRead(LEFT_BUTTON);
        if(gucLeftLastSts != ucLeftBnt){
            gucLeftLastSts = ucLeftBnt;
            gucBtnShortTime1 = ulRet + 50;
            gucBtnLongTime1 = ulRet + 1000;
        }
        else{
            if(ulRet > gucBtnShortTime1){
                if((gucLeftSts == LOW) && (gucLeftLastSts == HIGH)){
                     if(gucLeftLongActived != true){
                        fncStartMatch();
                     }
                }
                if(gucLeftSts != gucLeftLastSts){
                    gucLeftLongActived = false;
                }
                gucLeftSts = gucLeftLastSts;
                
            }
            if(ulRet > gucBtnLongTime1){
                if(gucLeftLastSts == LOW){
                    if(gucLeftLongActived == false){
                        fncStopMatch();    // Decrease
                        gucLeftLongActived = true;
                    }
                }
                gucLeftLongSts = gucLeftLastSts;
            }
        }
        
        
        ucRightBnt = digitalRead(RIGHT_BUTTON);
        if(gucRightLastSts != ucRightBnt){
            gucRightLastSts = ucRightBnt;
            gucBtnShortTime2 = ulRet + 100;
            gucBtnLongTime2 = ulRet + 1000;
        }
        else{
            if(ulRet > gucBtnShortTime2){
                if((gucRightSts == LOW) && (gucRightLastSts == HIGH)){
                     if(gucRightLongActived != true){
                        fncSendInc();    // Increase
                        }
                }
                if(gucRightSts != gucRightLastSts){
                    gucRightLongActived = false;
                }
                gucRightSts = gucRightLastSts;
                
            }

            if(ulRet > gucBtnLongTime2){
                if(gucRightLastSts == LOW){
                    if(gucRightLongActived == false){
                        fncSendDec();    // Decrease
                        gucRightLongActived = true;
                    }
                }
                gucRightLongSts = gucRightLastSts;
            }
        }
        
    }
    if (ulRetWakeUp < millis()) {
      wakeUP = 0;
      //Serial.println(" Sleep please");  
      
    }
}

void fncStartMatch(void){
    Serial.println("Button - Start timer");  
                     
    //memset(&gucSendDat, 0x0, sizeof(gucSendDat));
    //gucSendDat[0] = 1;
    //fncSlaveSend(gucSendDat, sizeof(gucSendDat));
}

void fncStopMatch(void){
    Serial.println("Button - reset timer");  
    
    //memset(&gucSendDat, 0x0, sizeof(gucSendDat));
    //gucSendDat[0] = 2;
    //fncSlaveSend(gucSendDat, sizeof(gucSendDat));    
}
void fncSendInc(void){
    Serial.println("Button - increase");  
                     
    //memset(&gucSendDat, 0x0, sizeof(gucSendDat));
    //gucSendDat[0] = 3;
    //fncSlaveSend(gucSendDat, sizeof(gucSendDat));
}

void fncSendDec(void){
    Serial.println("Button - decrease");  
    
    //memset(&gucSendDat, 0x0, sizeof(gucSendDat));
    //gucSendDat[0] = 3;
    //fncSlaveSend(gucSendDat, sizeof(gucSendDat));    
}

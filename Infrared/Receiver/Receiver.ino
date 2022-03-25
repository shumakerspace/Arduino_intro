#include <IRremote.h> //including infrared remote header file
int RECV_PIN = 12; // the pin where you connect the output pin of IR sensor
IRrecv irrecv(RECV_PIN);
decode_results results;
void setup()
{
Serial.begin(115200);
irrecv.enableIRIn();
pinMode(12,INPUT);
}
void loop() {

if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
{
int readResults = results.value;// Results of decoding are stored in result.value
Serial.print("Code: ");
Serial.print(readResults); //prints the value a a button press
Serial.print("  -  Bits: ");
Serial.print(results.bits); //prints the value a a button press
Serial.println(" ");
irrecv.resume(); // Restart the ISR state machine and Receive the next value
}

/*
Serial.println(analogRead(12));
delay(5);
*/
}

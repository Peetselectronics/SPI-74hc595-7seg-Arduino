// 7 Segment Display SPI code by Peet's electronics
//
//          AAAA         Q0Q0Q0
//         F    B       Q5    Q1
//         F    B       Q5    Q1
//          GGGG         Q6Q6Q6
//         E    C       Q4    Q2
//         E    C       Q4    Q2        
//          DDDD  H      Q3Q3Q3  Q7   (H and Q7 = DP)

#include <SPI.h>

//define pinS
const int latchPin = 9;
const int clockPin = 13;
const int dataPin  = 11;
const int com1  = 10;
const int com2  = 16;
const int com3  = 14;
const int com4  = 15;

//variable
byte value;
int i = 0;
long previousMillis = 0;
long interval = 500; //(milliseconds)
 

//bitstream patterns 
byte one      = 0b00000110; //LEFT 0b00110000;
byte two      = 0b01011011;
byte three    = 0b01001111;
byte four     = 0b01100110;
byte five     = 0b01101101;
byte six      = 0b01111101;
byte seven    = 0b00000111;
byte eight    = 0b01111111;
byte nine     = 0b01100111;
byte zero     = 0b00111111;
byte dot     =  0b10000000;

// setup the correct pins and initialize SPI library
void setup() {
  // setup pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  
  // setup SPI library
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();

  //setup common pins display
  pinMode(com1, OUTPUT);
  pinMode(com2, OUTPUT);
  pinMode(com3, OUTPUT);
  pinMode(com4, OUTPUT);

  digitalWrite(com1, HIGH);
  
}

void loop() {

   Refresh_Display();
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    Set_Digit(i);  
    i++;  
  }
  if(i > 10) { i = 0;}
  Refresh_Display();

}

void Set_Digit(byte digit) {
  
  if(digit == 1)  { value = one;}
  if(digit == 2)  { value = two;}  
  if(digit == 3)  { value = three;}
  if(digit == 4)  { value = four;}
  if(digit == 5)  { value = five;}
  if(digit == 6)  { value = six;}
  if(digit == 7)  { value = seven;}
  if(digit == 8)  { value = eight;}
  if(digit == 9)  { value = nine;}
  if(digit == 10) { value = dot;}
  if(digit == 0)  { value = zero;}
  
}

void Refresh_Display(){
  
    digitalWrite(com1, LOW); // com1 low active 
    digitalWrite(latchPin, LOW);
    SPI.transfer(value); // write partern 
    digitalWrite(latchPin, HIGH);
    digitalWrite(com1, HIGH); // com1 low inactive 
 
    digitalWrite(com2, LOW); // com1 low active 
    digitalWrite(latchPin, LOW);
    SPI.transfer(value); // write partern 
    digitalWrite(latchPin, HIGH);
    digitalWrite(com2, HIGH); // com1 low inactive 

    digitalWrite(com3, LOW); // com1 low active 
    digitalWrite(latchPin, LOW);
    SPI.transfer(value); // write partern 
    digitalWrite(latchPin, HIGH);
    digitalWrite(com3, HIGH); // com1 low inactive 

    digitalWrite(com4, LOW); // com1 low active 
    digitalWrite(latchPin, LOW);
    SPI.transfer(value); // write partern 
    digitalWrite(latchPin, HIGH);
    digitalWrite(com4, HIGH); // com1 low inactive 
  }


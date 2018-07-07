// 7 Segment Display SPI code by Peet's electronics
//This example use timelib.h for PaulStoffregen: https://github.com/PaulStoffregen/Time
//
//          AAAA         Q0Q0Q0
//         F    B       Q5    Q1
//         F    B       Q5    Q1
//          GGGG         Q6Q6Q6
//         E    C       Q4    Q2
//         E    C       Q4    Q2        
//          DDDD H       Q3Q3Q3 Q7   (H and Q7 = DP)

#include <SPI.h>
#include <TimeLib.h>

//timezone
int timezone = +2; //+2 hours = Amsterdam, Berlin, etc. 
long int timestamp = 1530986093 ; // 07/07/2018 @ 5:54pm (UTC)

// define pins and other variables
const int latchPin = 9;
const int clockPin = 13;
const int dataPin  = 11;
//common pin array
const int commons[4] = {10, 16, 15, 14};

//variable
byte value;
int pow2[4] = { 1, 10, 100, 1000}; // 

int time_out = 0;
long previousMillis = 0;
long interval = 10; //(milliseconds)
 

//bitstream patarnas 
byte one      = 0b00000110; //0b00110000;
byte two      = 0b01011011;
byte three    = 0b01001111;
byte four     = 0b01100110;
byte five     = 0b01101101;
byte six      = 0b01111101;
byte seven    = 0b00000111;
byte eight    = 0b01111111;
byte nine     = 0b01101111;
byte zero     = 0b00111111;
byte dot     =  0b10000000;

// setup the correct pins and initialize SPI library
void setup() {
  //set the time with the current unix timestamp, you can find the online. 
  setTime(timestamp + (timezone * 3600) ); //set the unix timestamp her. 
 
  // setup pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);
  
  // setup SPI library
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  //setup common pins display
  pinMode(commons[1], OUTPUT);
  pinMode(commons[0], OUTPUT);
  pinMode(commons[2], OUTPUT);
  pinMode(commons[3], OUTPUT);

  digitalWrite(commons[0], HIGH); // com1 low inactive 
  digitalWrite(commons[1], HIGH); // com2 low inactive 
  digitalWrite(commons[2], HIGH); // com3 low inactive 
  digitalWrite(commons[3], HIGH); // com4 low inactive  
}

void loop() {
  
  Refresh_Display_int(time_out, 2);  //(variable, dot place) can only display 0-9 and 1 decimal dot at once. 
  
  unsigned long currentMillis = millis(); //perodic interval check, so there is more time(cpu power) left for other tasks.
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    
    time_out =  (int)(hour()* 100) + minute(); //read timelib hour and minutes and count them so you get integer of the time.
    //23:21 will be 2321.
  }
}

void Refresh_Display_int(int value_in, byte dot_in){

    for (int i=0; i < 4; i++){
      digitalWrite(latchPin, LOW);
      
      if(dot_in == i){
        SPI.transfer((Set_Digit(ExtractNumbers(value_in, i))) | 0b10000000); //Set the decimal dot bit with OR logic
      }
      else
      SPI.transfer(Set_Digit(ExtractNumbers(value_in, i))); // write partern to the spi bus. 
      
      digitalWrite(latchPin, HIGH);
      digitalWrite(commons[i], LOW); 
      delay(1);
      digitalWrite(commons[i], HIGH);    
    }
  }

byte Set_Digit(byte digit) {
  
  byte value;
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
   
  return value;
}

int ExtractNumbers(int V, byte P)
{
  return V / pow2[P] % 10 ;
}

#include <SoftwareSerial.h>

SoftwareSerial bleShield(2, 3);

long previousMillis = 0; 
long interval = 1000; 
int counter = 0;
void setup()  
{
  // set the data rate for the SoftwareSerial port
  bleShield.begin(19200);          
  Serial.begin(19200);
  randomSeed(analogRead(0));  
}
void p(char *fmt, ... )
{
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}
void loop() // run over and over
{
   uint8_t temp,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8, temp9; 
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    
    int randNumber1 = random(255);
    int randNumber2 = random(255);
    int randNumber3 = random(255);
    int randNumber4 = random(255);
    
    bleShield.write(randNumber1);
    bleShield.write(randNumber2);
    bleShield.write(randNumber3);
    bleShield.write(randNumber4);    
  }
  
  if (bleShield.available()) {
    //Serial.write("yes");
    uint8_t t;
    temp =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp1 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}  
    temp2 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp3 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp4 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp5 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp6 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp7 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp8 =(uint8_t) bleShield.read();
    while(!(bleShield.available())){}
    temp9 =(uint8_t) bleShield.read();
    
    if(temp6!=0x0A && temp8==0x0A && temp9==0x0A){
     // if(counter < 6)
        p("%02X " , temp);
        p("%02X " , temp1);
        p("%02X " , temp2);
        p("%02X " , temp3);
        p("%02X " , temp4);
        p("%02X " , temp5);
     // else{
     //   counter = -1;
        int rssi = (int) temp6;
        rssi = rssi - 262;
        Serial.println();
    //    if(rssi < -90 || rssi > -30)
    //    {   
    //      Serial.println();
        
    //    }
    Serial.print("From:"); 
    Serial.println(temp7);
        Serial.println(rssi);
 //     }
   }else{
      while(!(temp6!=0x0A && temp8==0x0A && temp9==0x0A)){
        temp6 = temp7;
        temp7 = temp8;
        temp8 = temp9;
        while(!(bleShield.available())){}
        temp9 =(uint8_t) bleShield.read();
      }
    }
 //   counter++;
    
  }
}

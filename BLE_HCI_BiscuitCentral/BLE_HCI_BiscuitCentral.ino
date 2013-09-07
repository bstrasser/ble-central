#include <stdarg.h>
#include "typedef.h"
#include "biscuit_central.h"
#include "ble_hci.h"
#include "MemoryFree.h"
#include <avr/wdt.h>

#if defined(__AVR_ATmega328P__) // Arduino UNO?
#include <AltSoftSerial.h>
AltSoftSerial Serial1;
#endif
 uint16_t event;
  uint8_t *buf;
//change the address to connect to different server
uint8_t found_address[6] = {255, 247, 73, 41, 106 , 188};
//uint8_t found_address[6] = {6, 113, 103, 128, 7 , 0};

static uint8_t found_address1[1000];
uint8_t rssia[200];
int connectedd = 0;
int addcount = 0;
boolean scanning = false;
void p(char *fmt, ... )
{
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}

void ble_event_poll()
{
}

byte ble_event_available()
{
  return Serial1.available(); 
}

byte ble_event_process()
{
  uint8_t type, event_code, data_len, status1;
  
  type = Serial1.read();
  delay(35);
  event_code = Serial1.read();
  data_len = Serial1.read();
  
  buf=(uint8_t *)malloc(64*sizeof(uint8_t));

  for (int i = 0; i < data_len; i++)
    buf[i] = Serial1.read();

  event = BUILD_UINT16(buf[0], buf[1]);
  status1 = buf[2];
  p(" Event       : 0x%04X\r\n", event);
  p(" Status      : 0x%02X\r\n", status1);

  switch (event)
  {
  case 0x060D: // GAP_DeviceDiscovery
    {
      p("GAP_DeviceDiscovery\r\n");

     int rssi = (int) buf[11];
      //rssi = -93 - rssi + 256;
      rssi = rssi - 262;
      if(rssi >= -75){
        Serial.println(addcount);
        uint8_t temprssi = buf[11];
    
        rssia[addcount] = temprssi;
        p("0x%02X", rssia[addcount]);
        int temppp = addcount*6;
        memcpy(&found_address1[temppp], &buf[5], 6);
        addcount++;
      }
      for(int k=5;k < 11; k++){
        p(" %02X:%02X:%02X:%02X:%02X:%02X", buf[k], buf[k+1],buf[k+2],buf[k+3],buf[k+4],buf[k+5]);
        Serial.println();
        k = k + 5;     
      }

      Serial.println(rssi);        
    }
    break;

  case 0x051B: // ATT_HandleValueNotification
    {
      p("ATT_HandleValueNotification\r\n");

      uint8_t data[21] = {
        0      };
      uint8_t len = data_len - 8;

      if (len > 20)
        len = 20;

      memcpy(data, &buf[8], len);

      p(" -------------> Received from Biscuit peripheral: %s\r\n", data);
    }
    break;
  case 0x0605:
    {
      p("EstablishLink Event\r\n");
      p(" Status      : 0x%02X\r\n", status1);

      connectedd = 1;
    }
    break;  
  case 0x0606:
    {
      p("EstablishLink Event\r\n");
      p(" Status      : 0x%02X\r\n", status1);

      connectedd = 0;
    }
    break;  

  }  
  free(buf);
}
 
void setup()
{ 
#if defined(__AVR_ATmega328P__)
  Serial1.begin(57600);
  Serial.begin(57600);

  ble_hci_init(&Serial1);
#else
  Serial1.begin(57600);
  Serial.begin(57600);
  while (!Serial);

  ble_hci_init();
#endif

  biscuit_central_init();
}

void loop()
{
 Serial.println("main loop");
 
//   while (Serial.available())
//  {  
//    byte cmd = Serial.read();
 
  while(1){
    while (ble_event_available())
    {      
      ble_event_process();
      delay(50);
      
    }
    Serial.println("main");
    addcount = 0;
    for(int m = 0; m < 14; m++){
      p(" -> Start discovery...\r\n");
      scanning = 1;
      while (ble_event_available())
      {
        ble_event_process();
      }
      biscuit_central_start_discovery();
    
      delay(300);
    
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }
      delay(300);
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }    
      while(ble_event_available())
      {
        ble_event_process();
        delay(50);       
      }
      delay(300);
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }
      delay(300);
      while(ble_event_available())
      {
        ble_event_process();
        delay(50); 
      }
      biscuit_central_end_discovery();
    
      Serial.print(addcount);
    }
    delay(1000);
  
    while(ble_event_available())
    {
      ble_event_process();  
    }
    p(" -> Send data to the Biscuit peripheral...\r\n");
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
    int failcount = 0;
    while(connectedd==0){
      biscuit_central_connect(found_address);
       delay(2000);
      while(ble_event_available())
      {
        ble_event_process();
      }
      delay(2000);
      while(ble_event_available())
      {
        ble_event_process();
      }
      if(failcount >= 20)
        break;
        
      failcount++;  
    }
    for(int a = 0; a < addcount; a++){
      uint8 *temp1;

      char temp[10];

      temp[0] = found_address1[a*6];
      temp[1] = found_address1[a*6+1];
      temp[2] = found_address1[a*6+2];
      temp[3] = found_address1[a*6+3];
      temp[4] = found_address1[a*6+4];
      temp[5] = found_address1[a*6+5];
      temp[6] = rssia[a];
      temp[7] = 0x01;
      temp[8] = 0x0A;
      temp[9] = 0x0A;
      biscuit_central_write_bytes((uint8 *) &temp, 10);
      delay(300);
    }
    addcount = 0;
    delay(200);
    while (ble_event_available())
    {  
      ble_event_process();
      delay(50);
    }
    failcount = 0;
    while(connectedd==1){
      biscuit_central_disconnect();
      delay(2000);
      while (ble_event_available())
      {  
        ble_event_process();
      }
      delay(2000);
      while(ble_event_available())
      {
        ble_event_process();
      }
      if(failcount >= 20)
        break;
        
      failcount++;
    }

  }
//  }
}




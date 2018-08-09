
 /*
  
  Leds UDP Receiver 
     
 Description:
 * Firmware receiving RGB LED pixel data over UDP packets.

 Software:
 * FastLED Library 
 * Wifi Library 
 * WifiUdp Library
 
 Hardware:
* Adafruit HUZZAH32 – ESP32 Feather Board
* WS2812B Led Strip
* 74AHCT125 - Quad Level-Shifter (3V to 5V) 


 created 06 August 2018
 Based on Claude Heintz "sACN2Ws2811.ino"
 This code is under A Creative Commons Attribution/Share-Alike License http://creativecommons.org/licenses/by-sa/4.0/
   (2018) by Imanol Gómez

 */
 
 

#include "WifiManager.h"
#include "LedsManager.h"


LedsManager ledsManager;
WifiManager wifiManager(&ledsManager);


void setup() 
{
    Serial.begin(115200);
    delay(300);
    Serial.println("Starting Software!!!!");

    ledsManager.setup();
    wifiManager.setup();
}

void loop() 
{
    ledsManager.update();
    wifiManager.update();
 
}

///////////////////////////////////////////////////////////////////
// Class controlling the leds and its effects
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "FastLED.h"
#include "WiFiManager.h"


#define DATA_PIN_1    32
#define DATA_PIN_2    2

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 300
#define MAX_BRIGHTNESS 100
#define TEST_DELAY 300


const byte channelwidth = 3; //3 channels per pixel


class LedsManager{

  public:
    
    LedsManager() {}
    
    void setup();
    void update();

    void parseRGBReceived(unsigned char* pbuff, int count);
    void setAllBlack();
    
  private:

    void setupLeds();
    void initTest();

    CRGBArray<NUM_LEDS> leds1;
    CRGBArray<NUM_LEDS> leds2;

    
};

void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
}


void LedsManager::setupLeds()
{
   FastLED.addLeds<LED_TYPE,DATA_PIN_1,COLOR_ORDER>(leds1, NUM_LEDS);
   FastLED.addLeds<LED_TYPE,DATA_PIN_2,COLOR_ORDER>(leds2, NUM_LEDS);
   FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   FastLED.setDither( 0 );
   FastLED.clear();   
   this->setAllBlack();
   Serial.println("LedsManager::setupLeds");
}

void LedsManager::update()
{
    
}

void LedsManager::setAllBlack()
{ 
  fill_solid(leds1,NUM_LEDS, CRGB::Black);
  fill_solid(leds2,NUM_LEDS, CRGB::Black);
  FastLED.show();
 
}

void LedsManager::parseRGBReceived(unsigned char* pbuff, int count) 
{
  //DEBUG_PRINT_LN ("DMX Packet Received");
  int output_channel = pbuff[6];
  int numLeds = count/channelwidth;

//  Serial.print("LedsManager::parseRGBReceived -> outputChannel: ");
//  Serial.println(output_channel);
//
//  Serial.print("LedsManager::parseRGBReceived -> numLeds: ");
//  Serial.println(numLeds);
  
  if(numLeds > NUM_LEDS){
     numLeds = NUM_LEDS;
  }
  
  if ( output_channel == 0) 
  {
    int channel = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
    {
        leds1[i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
        channel +=channelwidth; //increase last channel number by channel width
    }
  } 

 else if( output_channel ==  1)
  {
      int channel = 0; //reset RGB channel assignment to 0 each time through loop.
      for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
      {
          leds2[i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
          channel +=channelwidth; //increase last channel number by channel width
      }
  }


  //adjust_gamma();
  
  FastLED.show(); //send data to pixels
}


void LedsManager::initTest() //runs at board boot to make sure pixels are working
{
  FastLED.setBrightness(255);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  Serial.println ("Red:");
  fill_solid(leds1,NUM_LEDS, CRGB::Red);
  fill_solid(leds2,NUM_LEDS, CRGB::Red);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Green);
  fill_solid(leds2,NUM_LEDS, CRGB::Green);
  Serial.println ("Green");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Blue);
  fill_solid(leds2,NUM_LEDS, CRGB::Blue);
  Serial.println ("Blue");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Yellow);
  fill_solid(leds2,NUM_LEDS, CRGB::Yellow);
  Serial.println ("Yellow");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Violet);
  fill_solid(leds2,NUM_LEDS, CRGB::Violet);
  Serial.println ("Violet");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::White);
  fill_solid(leds2,NUM_LEDS, CRGB::White);
  Serial.println ("White - Check Power!");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  FastLED.clear();
  Serial.println ("Rainbows!");
  fill_rainbow(leds1, NUM_LEDS, CRGB::White);
  fill_rainbow(leds2, NUM_LEDS, CRGB::White);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  Serial.println("Show Time...");
  FastLED.clear();
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    


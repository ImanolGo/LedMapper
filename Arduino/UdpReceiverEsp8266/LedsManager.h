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
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "WiFiManager.h"


#define DATA_PIN_1    12
#define DATA_PIN_2    15
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 170
#define MAX_BRIGHTNESS 100
#define TEST_DELAY 500


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

    //CRGBArray<NUM_LEDS> leds1;
    //CRGBArray<NUM_LEDS> leds2;
    CRGB leds1[NUM_LEDS];
    CRGB leds2[NUM_LEDS];
    
};

void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
    //this->initTest();
}


void LedsManager::setupLeds()
{
   FastLED.addLeds<LED_TYPE,DATA_PIN_1, COLOR_ORDER>(leds1, NUM_LEDS);
   FastLED.addLeds<LED_TYPE,DATA_PIN_2, COLOR_ORDER>(leds2, NUM_LEDS);

 
   FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   //FastLED.setDither( 0 );
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
  FastLED.setBrightness(MAX_BRIGHTNESS);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  Serial.println ("LedsManager::Red:");
  fill_solid(leds1,NUM_LEDS, CRGB::Red);
  fill_solid(leds2,NUM_LEDS, CRGB::Red);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Green);
  fill_solid(leds2,NUM_LEDS, CRGB::Green);
  Serial.println ("LedsManager::Green");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Blue);
  fill_solid(leds2,NUM_LEDS, CRGB::Blue);
  Serial.println ("vBlue");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Yellow);
  fill_solid(leds2,NUM_LEDS, CRGB::Yellow);
  Serial.println ("LedsManager::Yellow");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::Violet);
  fill_solid(leds2,NUM_LEDS, CRGB::Violet);
  Serial.println ("LedsManager::Violet");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds1,NUM_LEDS, CRGB::White);
  fill_solid(leds2,NUM_LEDS, CRGB::White);
  Serial.println ("LedsManager::White - Check Power!");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  FastLED.clear();
  Serial.println ("LedsManager::Rainbows!");
  fill_rainbow(leds1, NUM_LEDS, CRGB::White);
  fill_rainbow(leds2, NUM_LEDS, CRGB::White);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  Serial.println("LedsManager::Show Time...");
  FastLED.clear();
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    


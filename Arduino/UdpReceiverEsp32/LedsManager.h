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

#define LED_TYPE    DOTSTAR
#define COLOR_ORDER BGR
#define NUM_CHANNELS 8
#define NUM_LEDS 300
#define MAX_BRIGHTNESS 100
#define TEST_DELAY 500

constexpr int data_pins[NUM_CHANNELS] = {0,2,4,12,13,14,15,16};
constexpr int clock_pins[NUM_CHANNELS] = {17,18,19,21,22,25,26,27};


const byte channelwidth = 3; //3 channels per pixel


class LedsManager{

  public:
    
    LedsManager();
    
    void setup();
    void update();

    void parseRGBReceived(unsigned char* pbuff, int count);
    void setAllColor(CRGB color);
    
  private:

    void setupLeds();
    void initTest();

    //int data_pins[NUM_CHANNELS];
    //int clock_pins[NUM_CHANNELS];
    CRGB leds[NUM_CHANNELS][NUM_LEDS];
    
};

LedsManager::LedsManager()
{

}

void LedsManager::setup()
{
    Serial.println("LedsManager::setup");
    this->setupLeds(); 
    //this->initTest();
}


void LedsManager::setupLeds()
{

    //FastLED.addLeds<WS2812B,32, GRB>(leds[0], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[0],clock_pins[0], COLOR_ORDER>(leds[0], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[1],clock_pins[1], COLOR_ORDER>(leds[1], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[2],clock_pins[2], COLOR_ORDER>(leds[2], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[3],clock_pins[3], COLOR_ORDER>(leds[3], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[4],clock_pins[4], COLOR_ORDER>(leds[4], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[5],clock_pins[5], COLOR_ORDER>(leds[5], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[6],clock_pins[6], COLOR_ORDER>(leds[6], NUM_LEDS);
    FastLED.addLeds<LED_TYPE,data_pins[7],clock_pins[7], COLOR_ORDER>(leds[7], NUM_LEDS);

 
   //FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
   //FastLED.setDither( 0 );
   FastLED.clear();  
   this->setAllColor(CRGB::Black); 
   Serial.println("LedsManager::setupLeds");
}

void LedsManager::update()
{
    
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

   if(output_channel < 0 || output_channel>= NUM_CHANNELS){
    return;
  }
  
  if(numLeds > NUM_LEDS){
     numLeds = NUM_LEDS;
  }

 
    int channel = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < numLeds; i++) //loop to assign 3 channels to each pixel
    {
        leds[output_channel][i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
        channel +=channelwidth; //increase last channel number by channel width
    }
  
  //adjust_gamma();
  
  FastLED.show(); //send data to pixels
}

void LedsManager::setAllColor(CRGB color) 
{
  for(int i=0; i<NUM_CHANNELS; i++)
  {
     fill_solid(leds[i],NUM_LEDS, color);
  }
 
  FastLED.show();
}
void LedsManager::initTest() //runs at board boot to make sure pixels are working
{
  FastLED.setBrightness(MAX_BRIGHTNESS);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  Serial.println ("LedsManager::Red:");
  this->setAllColor(CRGB::Red);
  
  FastLED.delay(TEST_DELAY);
  
  Serial.println ("LedsManager::Green");
  this->setAllColor(CRGB::Green);
  
  FastLED.delay(TEST_DELAY);
  
  Serial.println ("LedsManager::Blue");
  this->setAllColor(CRGB::Blue);
  
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::Yellow");
  this->setAllColor(CRGB::Yellow);
  
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::Violet");
  this->setAllColor(CRGB::Violet);
 
  FastLED.delay(TEST_DELAY);

  Serial.println ("LedsManager::White  - Check Power!");
  this->setAllColor(CRGB::White);
  
  FastLED.delay(TEST_DELAY);

  Serial.println("LedsManager::Show Time...");
  FastLED.clear();
  this->setAllColor(CRGB::Black);
  //FastLED.setBrightness(MAX_BRIGHTNESS);
}

    


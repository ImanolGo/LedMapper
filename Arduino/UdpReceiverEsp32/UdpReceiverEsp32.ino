
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
 

#include "FastLED.h"
#include <WiFi.h>
#include <WiFiUdp.h>


#define DEBUG

#ifdef DEBUG
 #define DEBUG_PRINT_LN(x) Serial.println(x)
 #define DEBUG_PRINT(x) Serial.print(x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINT_LN(x)
#endif

#define DATA_PIN    32

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 144
#define MAX_BRIGHTNESS 100
CRGBArray<NUM_LEDS> leds;

#define TEST_DELAY 600

#define OUTPUT_CHANNEL 0
#define HEADER_SIZE 6
#define BUFFER_MAX 2048
#define SEND_PORT 2390


const byte channelwidth = 3; //3 channels per pixel
//byte channel; //channel number for each pixel RGB
unsigned long now = 0; //current time value
bool  is_connected = false;

char ssid[] = "TP-LINK_54E4"; //  your network SSID (name)
char pass[] = "27155332";    // your network password (use for WPA, or use as key for WEP)

//char ssid[] = "TP-Link_9528"; //  your network SSID (name)
//char pass[] = "15419699";    // your network password (use for WPA, or use as key for WEP)


IPAddress ip(192, 168, 0, 21); //  Fixed IP
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
//
//IPAddress ip(192, 168, 178, 21); //  Fixed IP
//IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
//IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

//Are we currently connected?
boolean connected = false;


WiFiUDP Udp;

unsigned char packetBuffer[BUFFER_MAX];
char ReplyBuffer[] = "acknowledged";

void setup()                    
{
  setupSerial();
  setupWifi();
  setupLeds();
  //initTest(); //test to make sure pixels are working
}

void setupSerial() {
   //Initialize serial and wait for port to open:
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  delay(100);
  DEBUG_PRINT_LN("Starting Software!!");
}

void setupWifi() {

   initializeWifi();
//  
//  // Print WiFi MAC address:
//  printMacAddress();
//
   //connectWifi();
//  printWiFiStatus();

  connectToWiFi(ssid, pass);
  
}

void setupLeds() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(UncorrectedColor);;
  FastLED.setDither( 0 );
  FastLED.setBrightness(MAX_BRIGHTNESS); 
   //FastLED.setMaxPowerInVoltsAndMilliamps (5, 2100);
  FastLED.clear();
  FastLED.show();

  pinMode(LED_BUILTIN, OUTPUT);
  
}

int value = 0;

void loop() 
{    
    int packetSize = Udp.parsePacket();
    //timer to check if receiving data
    if(packetSize)
    {
      //DEBUG_PRINT_LN("Received Data!!!");
      Udp.read(packetBuffer,BUFFER_MAX); //read UDP packet
      
      int count = checkProtocolHeaders(packetBuffer, packetSize);
      if (count) 
      {
          parseRGBReceived(packetBuffer, count); //process data function
      }
    }
}

void initializeWifi() {
   
   // udp.stop();
    //udp.flush();
    WiFi.disconnect(true);
    WiFi.config(ip, gateway, subnet);
    WiFi.setAutoReconnect(true);
}


void connectWifi() {

  int ledState = 0;
     // attempt to connect to WiFi network:
   DEBUG_PRINT("Attempting to connect to SSID: ");
   DEBUG_PRINT_LN(ssid);

//  WiFi.config(ip, gateway, subnet);
//  WiFi.setAutoReconnect(true);
//  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, pass);
         
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_BUILTIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    DEBUG_PRINT(".");
  }
 
   DEBUG_PRINT("\nConnected to SSID: ");
   DEBUG_PRINT_LN(ssid);

    DEBUG_PRINT("WiFi connected! IP address: ");
    DEBUG_PRINT_LN(WiFi.localIP());  
    //initializes the UDP state

   DEBUG_PRINT("\nStarting connection to UDP port ");
   DEBUG_PRINT_LN(PORT);
   
   //This initializes the transfer buffer
    Udp.begin(WiFi.localIP(),PORT);
    Udp.flush();
    connected = true;
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  delay(200);
  //register event handlerpin
  WiFi.onEvent(WiFiEvent);
  //WiFi.config(ip, gateway, subnet);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}


int checkProtocolHeaders(const unsigned char* messagein, int messagelength) {

  
  if ( messagein[0] == 0x10 && messagein[1] == 0x41 && messagein[2] == 0x37) { 
      // 0x41 = 'A'
      // 0x37 = '7'
      //DEBUG_PRINT("Data Size: ");
      int data_size = BtoI((byte)messagein[3],(byte)messagein[4]); // number of values plus start code
      
     // messagein[3] * 256 + messagein[4]; // number of values plus start code
      //DEBUG_PRINT_LN(data_size);
      //DEBUG_PRINT_LN(messagelength-HEADER_SIZE);

      if ( (messagelength-HEADER_SIZE) == data_size ) 
      {
        return data_size; //Return how many values are in the packet.
      }
        
    }
  return 0;
}

void parseRGBReceived(unsigned char* pbuff, int count) 
{
  //DEBUG_PRINT_LN ("DMX Packet Received");
  int output_channel = pbuff[5];
  if ( output_channel == OUTPUT_CHANNEL) 
  {
    int channel = 0; //reset RGB channel assignment to 0 each time through loop.
    for (int i = 0; i < NUM_LEDS; i++) //loop to assign 3 channels to each pixel
    {
        leds[i] = CRGB(pbuff[HEADER_SIZE + channel], pbuff[HEADER_SIZE + (channel +1)], pbuff[HEADER_SIZE + (channel +2)]);
        channel +=channelwidth; //increase last channel number by channel width
    }
  } 
  
  FastLED.show(); //send data to pixels
}


void sendAutodiscovery()
{
  if(is_connected) return;
  processUDP();
  if( millis() - autodiscovery_timmer > DISCOVERY_TIMMER)
  {
    uint8_t aa = udp_comm.beginPacket(broadcast, DISCOVERY_PORT);
    Build_autodiscoveryMessage();
    udp_comm.write((uint8_t *)udp_buffer,UDP_LEN);
    uint8_t bb = udp_comm.endPacket();
    if( !aa || !bb )
    {
      Serial.println(aa);
      Serial.println(bb);
      Serial.println("Error sending discovery"); 
    }
    else 
      Serial.println("sent!");
    autodiscovery_timmer = millis();
  }
}
void initTest() //runs at board boot to make sure pixels are working
{
  FastLED.setBrightness(100);       // set to full power
  
  FastLED.clear(); // on power up, one or more leds can be in a funky state.
  
  
  DEBUG_PRINT_LN ("Lamp Test:");
  DEBUG_PRINT_LN ("Red:");
  fill_solid(leds,NUM_LEDS, CRGB::Red);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Green);
  DEBUG_PRINT_LN ("Green");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Blue);
  DEBUG_PRINT_LN ("Blue");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Yellow);
  DEBUG_PRINT_LN ("Yellow");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::Violet);
  DEBUG_PRINT_LN ("Violet");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  fill_solid(leds,NUM_LEDS, CRGB::White);
  DEBUG_PRINT_LN ("White - Check Power!");
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  FastLED.clear();
  DEBUG_PRINT_LN ("Rainbows!");
  fill_rainbow(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
  FastLED.delay(TEST_DELAY);
  Serial.println("Show Time...");
  FastLED.clear();
  FastLED.show();
  FastLED.setBrightness(MAX_BRIGHTNESS);
}




//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          DEBUG_PRINT("WiFi connected! IP address: ");
          DEBUG_PRINT_LN(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          Udp.begin(WiFi.localIP(),PORT);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          DEBUG_PRINT_LN("WiFi lost connection");
          connected = false;
          //software_Reset();
          break;
    }
}


void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  esp_restart();  //tells the SDK to reboot, so its a more clean reboot, use this one if possible.
}  


int BtoI(byte a, byte b)
{
  return (a<<8)+b;
}


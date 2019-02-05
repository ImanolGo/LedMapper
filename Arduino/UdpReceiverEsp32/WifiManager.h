///////////////////////////////////////////////////////////////////
// Class controlling the wifi connection
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////


#define OUTPUT_CHANNEL 0
#define HEADER_SIZE 7

#pragma once
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUdp.h>
//#include <WiFiMulti.h>
#include "LedsManager.h"


#define BUFFER_MAX 2048
#define PACKET_SIZE 4
#define DISCOVERY_TIMER 3000
#define WIFI_TIMEOUT 3000              // checks WiFi every ...ms. Reset after this time, if WiFi cannot reconnect.
#define NO_DATA_TIMEOUT 10000         // sends autodiscovery if no data is coming after timeout
#define LOCAL_PORT 7000 
#define DISCOVERY_PORT 2390
#define SEND_PORT 7001 


//The udp library class
WiFiUDP UdpReceive;
WiFiUDP UdpDiscovery;

bool wifiConnected = false;

class WifiManager
{
  private:
 
  LedsManager* ledsManager;  ///< it sends the commands to the leds manager
  

  public:
    
    WifiManager(LedsManager* ledsManager);
    
    void setup();
    void update();

    static void WiFiEvent(WiFiEvent_t event);

  private:

    void initializeWifi();
    void parseUdp();
    void connectToWiFi(const char * ssid, const char * pwd);
    void connectWifi();
    void checkWifiConnection();
    void sendAutodiscovery();
    void noReceive();

    int checkProtocolHeaders(const unsigned char* messagein, int messagelength);
    int BtoI(byte a, byte b);

    String ssid;
    String pass;
    bool is_connected;
    unsigned long autodiscovery_timer;
    unsigned long no_data_timer;

    //WiFiUDP Udp;
    unsigned char packetBuffer[BUFFER_MAX];
    int receivedUdpLength;

    char PACKET_START;
    char AUTO_DISCOVERY_COMMAND;
    char PACKET_END;
  
};


WifiManager::WifiManager(LedsManager* ledsManager)
{
    this->ledsManager=ledsManager;
    
//    ssid = "TPH Operations";
//    pass = "TheFUTURE!Sno3";

//    ssid = "Don't worry, be happy!";
//    pass = "whyistheskysohigh?";

//    ssid     =  "TP-LINK_54E4";
//    pass = "27155332";

       ssid     =  "Spektrum";
    pass = "sonicvibrations19";
    
    

    wifiConnected = false;

    receivedUdpLength = 0;
    autodiscovery_timer =  millis();
    no_data_timer =  millis();
    is_connected = false;
}

void WifiManager::setup()
{
    Serial.println("WifiManager::setup");
    initializeWifi();
   // connectWifi();
}

void WifiManager::initializeWifi()
{
  
    UdpReceive.stop();
    UdpReceive.flush();
    UdpDiscovery.stop();
    UdpDiscovery.flush();
  

    Serial.println("WifiManager::connect wifi");
    connectToWiFi(ssid.c_str(), pass.c_str());
    //connectWifi();
}


void WifiManager::connectWifi() {

   wifiConnected = true;
     // attempt to connect to WiFi network:
   //Serial.print("Attempting to connect to SSID: ");
   //Serial.println(ssid);
   WiFi.begin(ssid.c_str(), pass.c_str());

   int attempts = 0;
 
    unsigned long connect_start = millis();
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("WifiManager::WIFI STATUS ");Serial.println(WiFi.status());
      Serial.print("..");
      attempts++;
      //connected = true;
      if(attempts>=15){
        wifiConnected =  false;
        break;
      }
   }

   wifiConnected = true;
   if(wifiConnected){
         Serial.print("\nWifiManager::Connected to SSID: ");
         Serial.println(ssid);
      
         Serial.println("WifiManager::IP address: ");
         Serial.println(WiFi.localIP());
        
         Serial.print("\nWifiManager::Starting connection to UDP port ");
         Serial.println(LOCAL_PORT);

         // if you get a connection, report back via serial:
         UdpReceive.begin(LOCAL_PORT);
         UdpReceive.flush();
    

   }

   else{
        Serial.print("\nUnable to connect to: ");
        Serial.println(ssid);
   }

    Serial.print("\nConnected:  ");
    Serial.println(wifiConnected);
  
    //connected = true;
}

//
//void WifiManager::connectWifi() {
//     // attempt to connect to WiFi network:
//   //Serial.print("Attempting to connect to SSID: ");
//   //Serial.println(ssid);
//   WiFi.begin(ssid.c_str(), pass.c_str());
//
//    unsigned long connect_start = millis();
//    while(WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.print(".");
//  
//      if(millis() - connect_start > WIFI_TIMEOUT) {
//        Serial.println();
//        Serial.print("Tried ");
//        Serial.print(WIFI_TIMEOUT);
//        Serial.print("ms. Resetting ESP now.");
//        ESP.reset();
//      }
//    }
// 
//   Serial.print("\nConnected to SSID: ");
//   Serial.println(ssid);
//
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//  
//    Serial.print("\nStarting connection to UDP port ");
//    Serial.println(localPort);
//    // if you get a connection, report back via serial:
//    Udp.begin(localPort);
//    Udp.flush();
//
//    connected = true;
//}



void WifiManager::update()
{
    parseUdp();
    sendAutodiscovery();
    noReceive();
}


void WifiManager::parseUdp()
{
  // if there's data available, read a packet
  int packetSize = UdpReceive.parsePacket();
  if (packetSize)
  {   
      //Serial.print("WifiManager::New Message: Size -> ");
      //Serial.println(packetSize);
      UdpReceive.read(packetBuffer,BUFFER_MAX); //read UDP packet
      int count = checkProtocolHeaders(packetBuffer, packetSize);
      //Serial.println(count);
      if (count) 
      {     
             //Serial.println(packetBuffer[5]);
            if(packetBuffer[5] == 'd'){
              no_data_timer =  millis();
              this->ledsManager->parseRGBReceived(packetBuffer, count); //process data function
            }
            else if(packetBuffer[5] == 'c'){
               is_connected = true;
               Serial.println("WifiManager::parseUdp-> Device Connected!!!");
               no_data_timer =  millis();
               // if you get a connection, report back via serial:
//               Udp.begin(LOCAL_PORT);
//               Udp.flush();
            }
           
           
              
      }
  }
}



int WifiManager::checkProtocolHeaders(const unsigned char* messagein, int messagelength) {

  
  if ( messagein[0] == 0x10 && messagein[1] == 0x41 && messagein[2] == 0x37) { 
      // 0x41 = 'A'
      // 0x37 = '7'
     //DEBUG_PRINT("Data Size: ");
      int data_size = BtoI((byte)messagein[3],(byte)messagein[4]); // number of values plus start code

//      Serial.println("WifiManager::checkProtocolHeaders: Data Bytes -> ");
//      Serial.println(messagein[3]);
//      Serial.println(messagein[4]);
      
     // messagein[3] * 256 + messagein[4]; // number of values plus start code
     //DEBUG_PRINT_LN(data_size);
      //DEBUG_PRINT_LN(messagelength-HEADER_SIZE);

//      Serial.print("WifiManager::checkProtocolHeaders: Data Size -> ");
//      Serial.println(data_size);
      if ( (messagelength-HEADER_SIZE) == data_size ) 
      {
        return data_size; //Return how many values are in the packet.
      }
        
    }
  return 0;
}

void WifiManager::connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("WifiManager::Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handlerpin
  WiFi.onEvent(WiFiEvent);
 // WiFi.config(ip, gateway, subnet);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("WifiManager::Waiting for WIFI connection...");
}


void WifiManager::checkWifiConnection(){
   if(WiFi.status() != WL_CONNECTED) {
      Serial.println("WifiManager::WiFi connection lost. Reconnecting...");
      this->connectWifi();
    } else {
      Serial.println("WifiManager::WiFi OK");
    }
}


//wifi event handler
void WifiManager::WiFiEvent(WiFiEvent_t event){
    
    IPAddress ip;
  
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WifiManager::WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          UdpReceive.begin(LOCAL_PORT);
          UdpDiscovery.begin(DISCOVERY_PORT);
          Serial.print("Listening to port: ");
          Serial.println(WiFi.localIP(), LOCAL_PORT); 
          wifiConnected = true;
          ip = WiFi.localIP();  ip[3] = 255;
 
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WifiManager::WiFi lost connection");
          wifiConnected = false;
          //software_Reset();
          break;
    }
}


void WifiManager::sendAutodiscovery()
{
  //if(is_connected || !wifiConnected) return;

  if (!wifiConnected) return;

  if(is_connected) return;

  if( millis() - autodiscovery_timer > DISCOVERY_TIMER)
  {
      IPAddress ip = WiFi.localIP();
      ip[3] = 255;

      int packetLength = 7;
      char bffr[packetLength];
      bffr[0] = 0x10;
      bffr[1] = 0x41;
      bffr[2] = 0x37;
      bffr[3] = 0;
      bffr[4] = 0;
      bffr[5] = 'c';
      bffr[6] = 0;
      
      // transmit broadcast package
   
      UdpDiscovery.beginPacket(ip, SEND_PORT);
      UdpDiscovery.write((uint8_t *)bffr,packetLength);
      UdpDiscovery.endPacket();

      Serial.println("WifiManager::Autodiscovery sent!");
      autodiscovery_timer = millis();
  }
}

void WifiManager::noReceive()
{
    if((millis() - no_data_timer) > NO_DATA_TIMEOUT) //is the time since the counter changed greater than 5 seconds?
    {
       Serial.println("WifiManager::No received Data!!");
       this->ledsManager->setAllColor(CRGB::Black);
        no_data_timer =  millis();
        //autodiscovery_timer = millis();
        is_connected = false;
    }
    
}


int WifiManager::BtoI(byte a, byte b)
{
  return (a<<8)+b;
}

/*
 *  UdpManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */




#include "UdpManager.h"
#include "AppManager.h"


UdpManager::UdpManager(): Manager()
{
    //Intentionally left empty
}

UdpManager::~UdpManager()
{
    ofLogNotice() << "UdpManager::destructor";
}


//--------------------------------------------------------------

void UdpManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    this->setupHeader();
    this->setupUdp();
    
    ofLogNotice() <<"UdpManager::initialized" ;
}


void UdpManager::setupHeader()
{
    m_header.f1 = 0x10;
    m_header.f2 = 0x41;
    m_header.f3 = 0x37;
    m_header.size = 0;
    m_header.channel = 0;
}

void UdpManager::setupUdp()
{
    int port = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    string ip = AppManager::getInstance().getSettingsManager().getIpAddress();
    //create the socket and set to send to 127.0.0.1:11999
    m_udpConnection.Create();
    m_udpConnection.Connect(ip.c_str(),port);
    m_udpConnection.SetNonBlocking(true);
    
    ofLogNotice() <<"UdpManager::setupUdp << Connecting to ip address: " << ip << ", port " << port;
    
}

void UdpManager::update()
{
    const auto & leds = AppManager::getInstance().getLedsManager().getLeds();
    //    const int length = leds.size()*3;
    //    const char* pixels[length];
    
    int ledsPerPixel = 3;
    
    string message="";
    message+= m_header.f1; message+= m_header.f2; message+= m_header.f3;
    m_header.size = ledsPerPixel*leds.size();
    unsigned char * s = (unsigned char*)& m_header.size;
    message+= s[1] ;  message+=  s[0];
    message+=m_header.channel;
    
    for(int i = 0; i< leds.size(); i++)
    {
        message+=leds[i]->getColor().r;
        message+=leds[i]->getColor().g;
        message+=leds[i]->getColor().b;
    }
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    //    message.clear();
    //    message = "";
    //    message+= m_header.f1; message+= m_header.f2; message+= m_header.f3;
    //    message+= s[1] ;  message+=  s[0];
    //    message+= (m_header.channel+1);
    //
    //    for(int i = 0; i< leds.size(); i++)
    //    {
    //        message+=leds[i]->getColor().r;
    //        message+=leds[i]->getColor().g;
    //        message+=leds[i]->getColor().b;
    //
    //    }
    //
    //     m_udpConnection.Send(message.c_str(),message.length());
    
}

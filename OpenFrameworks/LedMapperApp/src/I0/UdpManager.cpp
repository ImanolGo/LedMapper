/*
 *  UdpManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */




#include "UdpManager.h"
#include "AppManager.h"
#include "ofxMyIP.h"

const int UdpManager::UDP_MESSAGE_LENGHT = 100;

UdpManager::UdpManager(): Manager(), m_connected(false), m_ledsPerChannel(100)
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
    
    this->setupHeaders();
    this->setupIP();
    //this->setupTimer();
    this->sendAutodiscovery();
    this->setupUdpConnection();
    
   
    
    ofLogNotice() <<"UdpManager::initialized" ;
}


void UdpManager::setupHeaders()
{
    m_dataHeader.f1 = 0x10;
    m_dataHeader.f2 = 0x41;
    m_dataHeader.f3 = 0x37;
    m_dataHeader.size = 0;
    m_dataHeader.command = 'd';
    m_dataHeader.channel = 0;
    
    m_connectHeader.f1 = 0x10;
    m_connectHeader.f2 = 0x41;
    m_connectHeader.f3 = 0x37;
    m_connectHeader.size = 1;
    m_connectHeader.command = 'c';
    m_connectHeader.channel = 0;
}

void UdpManager::setupUdpConnection()
{
    int portReceive = AppManager::getInstance().getSettingsManager().getUdpPortReceive();
    ofLogNotice() <<"UdpManager::setupUdpReceiver -> listening for udp messages on port  " << portReceive;
    
    //m_udpConnection.SetEnableBroadcast(true);
    m_udpConnection.Create(); //create the socket
    m_udpConnection.Bind(portReceive); //and bind to port
    m_udpConnection.SetNonBlocking(true);
    
    
    string ip = AppManager::getInstance().getSettingsManager().getIpAddress();
    int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
    
    m_udpConnection.Connect(m_broadcast.c_str(),portSend);
    //m_udpConnection.Connect(ip.c_str(),portSend);
    m_udpConnection.SetEnableBroadcast(true);
    
    ofLogNotice() <<"UdpManager::setupUdpReceiver -> sending to IP " << m_broadcast <<" to port " << portSend;
    
    m_udpConnection.SetNonBlocking(true);
    
}

void UdpManager::createConnection(string& ip, int send)
{
    ofLogNotice() <<"UdpManager::createConnection -> sending to IP " << ip.c_str() <<" to port " << send;
    
    m_udpConnection.Connect(ip.c_str(),send);
    m_udpConnection.SetNonBlocking(true);
    m_connected = true;
}

void UdpManager::setupTimer()
{
    m_timer.setup( 60000 );
    
    m_timer.start( false ) ;
    ofAddListener( m_timer.TIMER_COMPLETE , this, &UdpManager::timerCompleteHandler ) ;
}

void UdpManager::setupIP()
{
    ofxMyIP myip;
    myip.setup();
    m_ip = myip.getIpAddress();



    ofLogNotice() <<"UdpManager::setupIP -> IP address: " << m_ip;

    m_broadcast = "";
    auto stringSplit = ofSplitString(m_ip, ".");

    for(int i=0; i<stringSplit.size(); i++){

        if(i<stringSplit.size()-1){
            m_broadcast += stringSplit[i];
            m_broadcast += ".";
        }

    }

    m_broadcast+="255";
    ofLogNotice() <<"UdpManager::setupIP -> Broadcast IP address: " << m_broadcast;

}
void UdpManager::update()
{
    this->updateReveivePackage();
    this->updatePixels();
   // m_timer.update();
    
}

void UdpManager::updatePixels()
{
    if(!m_connected){
        return;
    }
    
    bool newFrame = AppManager::getInstance().getLedsManager().isNewFrame();
    
    if(!newFrame ){
        return;
    }
    
    const auto & leds = AppManager::getInstance().getLedsManager().getLeds();
    //    const int length = leds.size()*3;
    //    const char* pixels[length];
    
    
   // ofLogNotice() <<"UdpManager::updatePixels -> New Frame " << leds.size();
    
    int ledsPerPixel = 3;
    int numChannels = leds.size()/m_ledsPerChannel + 1;
    
    //ofLogNotice() <<"UdpManager::updatePixels -> numChannels " << numChannels;
    
    for(int channel=0; channel<numChannels; channel++){
        int startIndex  = channel*m_ledsPerChannel;
        int endIndex  = (channel+1)*m_ledsPerChannel;
        
        if(endIndex>leds.size()){
            endIndex = leds.size();
        }
        
        string message="";
        message+= m_dataHeader.f1; message+= m_dataHeader.f2; message+= m_dataHeader.f3;
        int size = endIndex - startIndex;
        m_dataHeader.size = ledsPerPixel*size;
        unsigned char * s = (unsigned char*)& m_dataHeader.size;
        message+= s[1] ;  message+=  s[0];
        message+=m_dataHeader.command;
        message+=channel;
        
        for(int i=startIndex; i<endIndex; i++){
            if(i>=leds.size()){
                break;
            }
            message+=leds[i]->getColor().r;
            message+=leds[i]->getColor().g;
            message+=leds[i]->getColor().b;
      
        }
        
        m_udpConnection.Send(message.c_str(),message.length());
    }
    
   
    
}

void UdpManager::updateReveivePackage()
{
    char udpMessage[UDP_MESSAGE_LENGHT];
    m_udpConnection.Receive(udpMessage,UDP_MESSAGE_LENGHT);
    string message=udpMessage;
    
    if(message!="")
    {
        ofLogNotice() <<"UdpManager::updateReveivePackage -> SIZE " << message.size();
        ofLogNotice() <<"UdpManager::updateReveivePackage -> message " << message;
        
        this->parseMessage(udpMessage, UDP_MESSAGE_LENGHT);
    }
}

bool UdpManager::isMessage(char * buffer, int size)
{
    if(buffer[0] != m_connectHeader.f1  && buffer[1] != m_connectHeader.f2  && buffer[2] != m_connectHeader.f3 ){
        ofLogNotice() <<"UdpManager::isMessage -> FALSE ";
        return false;
    }
    
    
    ofLogNotice() <<"UdpManager::isMessage -> TRUE ";
    return true;
}

void UdpManager::parseMessage(char * buffer, int size)
{
    if(isMessage(buffer, size))
    {
        if(buffer[5] == m_connectHeader.command)
        {
            ofLogNotice() <<"UdpManager::isMessage -> Received Connect Command: " << m_connectHeader.command;
            string ip; int port;
            m_udpConnection.GetRemoteAddr(ip, port);
            int portSend = AppManager::getInstance().getSettingsManager().getUdpPortSend();
            this->createConnection(ip, portSend );
            this->sendConnected();
            m_timer.stop();
        }
        
    }
      
}

void UdpManager::timerCompleteHandler( int &args )
{
    m_timer.start(false);
    //  cout<<"TIMER COMPLETED"<<endl;
    this->sendAutodiscovery();
}

void UdpManager::sendConnected()
{
    string message="";
    message+= m_connectHeader.f1; message+= m_connectHeader.f2; message+= m_connectHeader.f3;
    unsigned char * s = (unsigned char*)& m_connectHeader.size;
    message+= s[1] ;  message+=  s[0];
    message+=m_connectHeader.command;
    message+=m_connectHeader.channel;
    message+='c';
    
    m_udpConnection.Send(message.c_str(),message.length());
    
    ofLogNotice() <<"UdpManager::sendConnected -> Send Connected ";
}
void UdpManager::sendAutodiscovery()
{
//    string message="";
//    
//    message+= START_COMMAND;
//    message+= AUTODISCOVERY_COMMAND;
//    message+= END_COMMAND;
//    
//    m_udpConnection.Send(message.c_str(),message.length());
//    
//    ofLogNotice() <<"UdpManager::sendAutodiscovery << " << message;
}

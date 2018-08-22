/*
 *  UdpManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "Manager.h"
#include "ofxSimpleTimer.h"


//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *    \brief class for managing the udp connection
 *    \details It writes and send udp data
 */


class UdpManager: public Manager
{
    static const int UDP_MESSAGE_LENGHT; ///Defines the Udp"s message length
    
    struct udp_header {
        Byte f1;
        Byte f2;
        Byte f3;
        short size;
        Byte command;
        Byte  channel;
    };
    
public:
    //! Constructor
    UdpManager();
    
    //! Destructor
    virtual ~UdpManager();
    
    //! setups the udp manager
    void setup();
    
    //! updates the udp manager
    void update();
    
    void timerCompleteHandler( int &args ) ;
    
    void setLedsPerChannel(int& value) {m_ledsPerChannel=value;}
    
    
private:
    
    void setupUdpConnection();
    
    void setupHeaders();
    
    void setupTimer();
    
    void setupIP();
    
    void updateReveivePackage();
    
    bool isMessage(char * buffer, int size);
    
    void parseMessage(char * buffer, int size);
    
    void receivedIp(char _id);
    
    void receivedHeartbeat(char _id, char val1, char val2);
    
    void createConnection(string& ip, int port);
    
    void sendAutodiscovery();
    
    void sendConnected();
    
    void sendDiscovered();
    
    void updatePixels();
    
    
    
private:
    
    ofxUDPManager m_udpConnection;
    udp_header    m_dataHeader;
    udp_header    m_connectHeader;
    ofxSimpleTimer         m_timer;
    string                 m_broadcast;
    string                 m_ip;
    bool                   m_connected;
    int                    m_ledsPerChannel;
    
    
};


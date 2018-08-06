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

//========================== class UdpManager =======================================
//==============================================================================
/** \class UdpManager UdpManager.h
 *    \brief class for managing the udp connection
 *    \details It writes and send udp data
 */


class UdpManager: public Manager
{
    
    struct udp_header {
        Byte f1;
        Byte f2;
        Byte f3;
        short size;
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
    
    
private:
    
    void setupUdp();
    
    void setupHeader();
    
private:
    
    ofxUDPManager m_udpConnection;
    udp_header    m_header;
    
    
};


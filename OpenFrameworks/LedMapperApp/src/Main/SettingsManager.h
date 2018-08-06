/*
 *  SettingsManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */


#pragma once

#include "Manager.h"


//========================== class SettingsManager ==============================
//============================================================================
/** \class SettingsManager SettingsManager.h
 *    \brief Class managing the whole settings of the application
 *    \details it reads from an xml settings file and provides access to the information
 */

typedef             map<string,string>               ResourcesPathMap;       ///< defines a map of path attached to the resources name

class SettingsManager: public Manager
{

public:
    
    static const string APPLICATION_SETTINGS_FILE_NAME;
    
    //! Destructor
    ~SettingsManager();
    
    //! Constructor
    SettingsManager();
    
    //! Compares two transition objects
    void setup();
    
    const ResourcesPathMap& getTextureResourcesPath() const {return m_texturesPath;}
    
    const ResourcesPathMap& getVideoResourcesPath() const {return m_videoResourcesPath;}
    
    const ofColor& getColor(const string& colorName);
    
    float getAppWidth() const {return m_appWidth;}
    
    float getAppHeight() const {return m_appHeight;}
    
    int getUdpPortSend() const {return m_portUdpSend;}
    
    int getUdpPortReceive() const {return m_portUdpReceive;}
    
    string getIpAddress() const {return m_ipAddress;}

    
private:
    
    //! Loads the settings file
    bool loadSettingsFile();
    
    //! Loads all the settings
    void loadAllSettings();
    
    //! Sets all the debug properties
    void setDebugProperties();
    
    //! Sets all the window properties
    void setWindowProperties();
    
    //! Sets all the network properties
    void setNetworkProperties();
    
    //! Loads all the app colors
    void loadColors();
    
    //! Loads all the textures settings
    void loadTextureSettings();
    
    //! Loads all the video  settings
    void loadVideoSettings();

    
private:
    
    typedef             map< string, ofColor >    ColorMap;               ///< Defines a map of colors attached to a name
    
    
    ofXml                    m_xml;                  ///< instance of the xml parser
    ResourcesPathMap        m_texturesPath;         ///< stores the texture paths
    ResourcesPathMap        m_videoResourcesPath;   ///< stores the video paths
    ColorMap                m_colors;               ///< stores all the application's colors
    float                   m_appWidth;             ///< stores the applications width
    float                   m_appHeight;            ///< stores the applications height
    int                     m_portUdpSend;          ///< stores the UDP port used for the OSC Sending communications
    int                     m_portUdpReceive;       ///< stores the UDP port used for the UDP Receiving communications
    string                  m_ipAddress;            ///< stores the Ip Address used for the Network communications
    ofColor                 m_blankColor;           ///<return blank color

};



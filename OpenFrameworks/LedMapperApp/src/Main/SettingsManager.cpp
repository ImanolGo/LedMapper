/*
 *  SettingsManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "ofMain.h"

#include "SettingsManager.h"


const string SettingsManager::APPLICATION_SETTINGS_FILE_NAME = "xmls/ApplicationSettings.xml";


SettingsManager::SettingsManager(): Manager(), m_appHeight(0.0), m_appWidth(0.0)
{
    //Intentionally left empty
}


SettingsManager::~SettingsManager()
{
    ofLogNotice() <<"SettingsManager::Destructor" ;
}


void SettingsManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"SettingsManager::initialized" ;
    
    Manager::setup();
    
    if(this->loadSettingsFile()){
        this->loadAllSettings();
    }
}

void SettingsManager::loadAllSettings()
{
    this->setWindowProperties();
    this->setDebugProperties();
    this->loadTextureSettings();
    this->loadVideoSettings();
    this->loadColors();
}

bool SettingsManager::loadSettingsFile()
{
    
    if(!m_xml.load(APPLICATION_SETTINGS_FILE_NAME)){
        ofLogNotice() <<"SettingsManager::loadSettingsFile-> unable to load file: " << APPLICATION_SETTINGS_FILE_NAME ;
        return false;
    }
    
    ofLogNotice() <<"SettingsManager::loadSettingsFile->  successfully loaded " << APPLICATION_SETTINGS_FILE_NAME ;
    return true;
}

void SettingsManager::setDebugProperties()
{
    
    //m_xml.setTo("//");
    
    string path = "//of_settings/debug";
    auto xml = m_xml.findFirst(path);
    if(xml) {
        typedef   std::map<string, string>   AttributesMap;
      //  AttributesMap attributes = m_xml.getAttributes();
        
        bool showCursor = xml.getAttribute("showCursor").getBoolValue();
        
        if(showCursor){
            ofShowCursor();
        }
        else{
            ofHideCursor();
        }
        
        
        
        bool setVerbose = xml.getAttribute("setVerbose").getBoolValue();
        if(setVerbose){
            ofSetLogLevel(OF_LOG_VERBOSE);
        }
        else{
            ofSetLogLevel(OF_LOG_NOTICE);
        }
        
        
        ofLogNotice() <<"SettingsManager::setDebugProperties->  successfully loaded the OF general settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setOFProperties->  path not found: " << path ;
}

void SettingsManager::setWindowProperties()
{
   // m_xml.setTo("//");
   
    string path = "//of_settings/window";
    auto xml = m_xml.findFirst(path);
    if(xml)
    {
        //m_xml.setTo(windowPath);
       // typedef   std::map<string, string>   AttributesMap;
       // AttributesMap attributes = m_xml.getAttributes();
        string title = xml.getAttribute("title").getValue();
        m_appWidth = xml.getAttribute("width").getIntValue();
        m_appHeight = xml.getAttribute("height").getIntValue();
       
        
        int x = xml.getAttribute("x").getIntValue();
        int y = xml.getAttribute("y").getIntValue();
        bool fullscreen = xml.getAttribute("fullscreen").getBoolValue();
        
        ofSetFullscreen(fullscreen);
        ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
        if(!fullscreen){
            ofSetWindowPosition(x,y);
        }
        ofSetWindowTitle(title);
        
        ofLogNotice() <<"SettingsManager::setWindowProperties->  successfully loaded the window settings" ;
        ofLogNotice() <<"SettingsManager::setWindowProperties->  title = "<< title<<", width = " << m_appWidth <<", height = "
        <<m_appHeight <<", x = "<<x<<", y = "<<y;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::setWindowProperties->  path not found: " << path ;
}


void SettingsManager::loadTextureSettings()
{
    string path = "//textures/texture";
    auto texturesXml = m_xml.find(path);
    if(!texturesXml.empty()) {
        
        for(auto & textureXml: texturesXml)
        {
            string path =  textureXml.getAttribute("path").getValue();
            string name =  textureXml.getAttribute("name").getValue();
            
            m_texturesPath[name] = path;
            
        
            ofLogNotice() <<"SettingsManager::loadTextureSettings->  texture = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadTextureSettings->  successfully loaded the resource settings" ;
        return;
    }
    
    
    ofLogNotice() <<"SettingsManager::loadTextureSettings->  path not found: " << path ;
}

const ofColor& SettingsManager::getColor(const string& colorName)
{
    if(m_colors.find(colorName)!= m_colors.end()){
        return m_colors[colorName];
    }
    
    return m_blankColor;
}



void SettingsManager::loadVideoSettings()
{
    string path = "//videos/video";
    auto videosXml = m_xml.find(path);
    if(!videosXml.empty()) {
        
        for(auto & videoXml: videosXml)
        {
            string path =  videoXml.getAttribute("path").getValue();
            string name =  videoXml.getAttribute("name").getValue();
            
            m_videoResourcesPath[name] = path;
            
            
            ofLogNotice() <<"SettingsManager::loadVideoSettings->  video = " << name
            <<", path = "<< path;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadVideoSettings->  successfully loaded the video settings" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadVideoSettings->  path not found: " << path ;
}

void SettingsManager::loadColors()
{
    m_blankColor = ofColor(0,0,0,0);
    
    string path = "//colors/color";
    auto colorsXml = m_xml.find(path);
    if(!colorsXml.empty()) {
        
        for(auto & colorXml: colorsXml)
        {
            int r = colorXml.getAttribute("r").getIntValue();
            int g = colorXml.getAttribute("g").getIntValue();
            int b = colorXml.getAttribute("b").getIntValue();
            int a = colorXml.getAttribute("a").getIntValue();
            string name =  colorXml.getAttribute("name").getValue();;
            
            m_colors[name] = ofColor(r,g,b,a);
            
            ofLogNotice() <<"SettingsManager::loadColors->  color = " << name <<", r = " << r
            <<", g = "<< g << ", b = " << b << ", a = " << a ;
        }
        
        
        ofLogNotice() <<"SettingsManager::loadColors->  successfully loaded the applications colors" ;
        return;
    }
    
    ofLogNotice() <<"SettingsManager::loadColors->  path not found: " << path ;
}







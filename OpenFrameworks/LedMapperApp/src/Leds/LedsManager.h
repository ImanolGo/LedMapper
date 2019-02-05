/*
 *  LedsManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#pragma once

#include "Manager.h"
#include "Led.h"

//========================== class LedsManager ==============================
//============================================================================
/** \class LedsManager LedsManager.h
 *	\brief Class managing the LEDs
 *	\details It controls the postion and color of the LEDs
 */


class LedsManager: public Manager
{
    public:
    
        typedef vector<int> IntVector;
        typedef vector< ofPtr<Led> > LedVector;

    
    public:

        //! Constructor
        LedsManager();

        //! Destructor
        ~LedsManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Led Manager
        void update();
    
        //! Draw the Led Manager
        void draw();
    
        const LedVector& getLeds() const {return m_leds;}
    
        void setPixels(ofPixelsRef pixels);
    
        void setLedColors(ofPixelsRef pixels);
    
        const ofPoint getMin() const {return m_minPos;}
    
        const ofPoint getMax() const {return m_maxPos;}
    
        bool loadLeds();
    
        bool load(string& path);
    
        bool saveLeds(string& path);
    
        void loadTest();
    
        bool isNewFrame() const {return m_isNewFrame;}
    
        void setSize(float& value);
    
        void setLedBrightness(float& value){m_ledsBrightness = value;}
    
        void setLaserBrightness(float& value){m_laserBrightness = value;}
    
    
    private:
    
    
        void setupLeds();
        void setupShader();
    
        void readLedsPosition();
    
        void createLedPositions();
    
        void sortLeds();
    
        void normalizeLeds();
    
        bool getIs3D();
    
        void  centreLeds();
    
        void readLedsPositionFromGroup(const string& groupName, int& id, int numberOfSections);
    
        void readLasersPositionFromGroup(const string& groupName, int& id, vector<int>& sections);
    
        bool parseLedLine(string& line, ofPoint& position);
    
        void createLed(const ofPoint& position, int& id);
    
        void removeCharsFromString( string &str, char* charsToRemove );
    
        bool isValidFile(const string& path);
    
        void setPixelColor(ofPixelsRef pixels, int index);
    
    

    private:
    
        LedVector          m_leds;
        ofPoint            m_minPos;
        ofPoint            m_maxPos;
        bool               m_isNewFrame;
        string             m_ledsFilePath;
        bool               m_is3D;
        float              m_ledsBrightness;
        float              m_laserBrightness;
    
    
        ofVbo m_vbo;
        ofShader m_shader;
        ofTexture m_texture;
    
        vector <ofVec3f> m_points;
        vector <ofVec3f> m_sizes;
        vector <ofFloatColor> m_colors;
        int         m_offset;
    
    
};




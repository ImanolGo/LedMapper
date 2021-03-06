/*
 *  VideoManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */


#pragma once

#include "Manager.h"
#include "ofxPSLevels.h"
#include "ofxBlur.h"
#include "ofxFastFboReader.h"

//========================== class VideoManager ==============================
//============================================================================
/** \class VideoManager VideoManager.h
 *	\brief Class managing the video player visuals
 *	\details it creates and updates the video player
 */


class VideoManager: public Manager
{
    
    static const string VIDEO_PATH;
    
public:
    
    //! Constructor
    VideoManager();
    
    //! Destructor
    ~VideoManager();
    
    //! Setup the Video Manager
    void setup();
    
    //! Update the Video Manager
    void update();
    
    //! Draw the Video Manager
    void draw();
    
    void loadVideo();
    
    void load(string& path);
    
    void exportVideo();
    
    void loadTest();
    
    void setContrast(float& value) {m_levels.contrast = value;}
    
    void setSaturation(float& value) {m_levels.saturation = value;}
    
    void setBrightness(float& value) {m_levels.brightness = value;}
    
    void setGamma(float& value) {m_levels.gamma = value;}
    
    void setMinInput(float& value) {m_levels.minInput = value;}
    
    void setMaxInput(float& value) {m_levels.maxInput = value;}
    
    void setMinOutput(float& value) {m_levels.minOutput = value;}
    
    void setMaxOutput(float& value) {m_levels.maxOutput = value;}
    
    void setBlurScale(float& value);
    
    void setSpeed(float& value);
    
    void stopExporting();
    
    void play();
    
    void pause();
    
    bool  isExporting() {return m_exportMode;}
    

private:
    
    void setupFbo();
    
    void setupShaders(float width,float height);
    
    void setupBlur(float width, float height);
    
    void setupLevels(float width, float height);
    
    void updateFbos();
    
    void updateVideo();
    
    void drawFbo();
    
    void drawVideo();

	void drawExporting();
    
    bool isVideo(const string& path);
    
private:
    
    
    ofVideoPlayer 	    m_videoPlayer;
    ofxPSLevels         m_levels;
    ofxBlur             m_blur;
    ofxFastFboReader    m_reader;
  
    ofFbo           m_fbo;
    ofFbo           m_exportFbo;
    ofColor         m_color;
    bool            m_exportMode;
    int             m_frameNumber;
    float           m_blurScale;
    

};





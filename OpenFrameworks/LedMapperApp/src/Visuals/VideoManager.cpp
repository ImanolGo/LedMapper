/*
 *  VideoManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "ofMain.h"

#include "VideoManager.h"
#include "AppManager.h"


VideoManager::VideoManager(): Manager(), m_exportMode(false), m_frameNumber(-1)
{
    //Intentionally left empty
}


VideoManager::~VideoManager()
{
    ofLogNotice() <<"VideoManager::Destructor" ;
}


void VideoManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->setupFbo();
    this->setupShaders();
    
    ofLogNotice() <<"VideoManager::initialized" ;
    
}



void VideoManager::setupFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    m_fbo.allocate(width,height);
    m_fbo.begin(); ofClear(0); m_fbo.end();
    
    m_exportFbo.allocate(width,height);
    m_exportFbo.begin(); ofClear(0); m_exportFbo.end();
}

void VideoManager::setupShaders()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    this->setupLevels(width, height);
}

void VideoManager::setupLevels(float width, float height)
{
    m_levels.setup(width,height);
}



void VideoManager::load(string& path)
{
    if(m_videoPlayer.load(path))
    {
        m_exportFbo.allocate(m_videoPlayer.getWidth(), m_videoPlayer.getHeight());
        m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
        m_videoPlayer.play();
        
        this->setupLevels(m_videoPlayer.getWidth(), m_videoPlayer.getHeight());
        
        AppManager::getInstance().getGuiManager().setVideoPath(path);
        
        ofLogNotice() <<"VideoManager::load -> successfully loaded: " << path;
        
    }
    else{
        ofLogNotice() <<"VideoManager::load -> unable to load: " << path;
    }
}




void VideoManager::updateVideo()
{
    if (!m_videoPlayer.isLoaded()) {
        return;
    }
    
    m_videoPlayer.update();
    
    if(m_videoPlayer.isFrameNew())
    {
        int frame = m_videoPlayer.getCurrentFrame();
        //ofLogNotice()<< "VideoManager::updateVideo:  New Frame!!" << frame;
        // ofLogNotice()<< "VideoManager::updateVideo:  Old Frame!!" << m_frameNumber;
        //ofLogNotice()<< "VideoManager::updateVideo:  Total Frames!!" <<  m_videoPlayer.getTotalNumFrames();
        
        if(m_exportMode && frame < m_frameNumber){
            ofLogNotice()<< "VideoManager::updateVideo:  Movie DONE!!";
            this->stopExporting();
        }
        
        if(m_frameNumber != frame)
        {
            m_frameNumber = frame;
            ofPixels pixels;
            m_fbo.readToPixels(pixels);
            AppManager::getInstance().getLedsManager().setPixels(pixels);
            //ofLogNotice()<< "VideoManager::newFrame: ";
        }
        
//        if(m_exportMode){
//            ofLogNotice()<< "VideoManager::updateVideo:  Next Frame!!" << m_frameNumber+1;
//            m_videoPlayer.setFrame(m_frameNumber+1);
//        }
        
//        if(m_exportMode && m_videoPlayer.getIsMovieDone()){
//            ofLogNotice()<< "VideoManager::updateVideo:  Movie DONE!!";
//            this->stopExporting();
//        }
        
//        if(m_exportMode && m_frameNumber>= m_videoPlayer.getTotalNumFrames()){
//            ofLogNotice()<< "VideoManager::updateVideo:  Movie DONE!!";
//            this->stopExporting();
//        }
        
        
    }
    
    
}

void VideoManager::update()
{
    this->updateVideo();
    this->updateFbos();
    
}


void VideoManager::updateFbos()
{
    if (m_videoPlayer.isLoaded()) {
        m_exportFbo.begin();
            ofClear(0);
            this->drawVideo();
        m_exportFbo.end();
    }
    
    m_fbo.begin();
        ofClear(0);
        m_exportFbo.draw(0,0,m_fbo.getWidth(), m_fbo.getHeight());
    m_fbo.end();
    
}

void VideoManager::drawVideo()
{
    m_levels.begin();
    m_videoPlayer.draw(0, 0);
    m_levels.end();
    
    m_levels.draw();
}



void VideoManager::draw()
{
    this->drawFbo();
}

void VideoManager::drawFbo()
{
    m_fbo.draw(0,0);
    
}

void VideoManager::loadVideo()
{
    //Open the Open File Dialog
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a mov or mp4");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess){
        
        ofLogVerbose("User selected a file");
        
        ofLogNotice() <<"VideoManager::loadVideo -> ser selected a file";
        ofLogNotice() <<"VideoManager::processOpenFileSelection -> name: " <<   openFileResult.getName();
        
        ofLogNotice() <<"VideoManager::processOpenFileSelection -> path: " <<   openFileResult.getPath();
        
        string path = openFileResult.getPath();
        //We have a file, check it and process it
        if(this->isVideo(path)){
            this->load(path);
        }
        
    }else {
        
         ofLogNotice() <<"VideoManager::loadVideo -> User hit cancel";
    }
}


bool VideoManager::isVideo(const string& path)
{
    
    ofFile file(path);
    
    if (file.exists())
    {
        string fileExtension = ofToUpper(file.getExtension());
        
        //We only want videos
        if (fileExtension == "MOV" || fileExtension == "MP4")
        {
            return true;
        }
        else{
            ofLogNotice() <<"VideoManager::processOpenFileSelection -> file is not a video ";
            return false;
        }
    }
    else{
        ofLogNotice() <<"VideoManager::processOpenFileSelection -> file doesn't exist ";
        return false;
    }
    
    return false;
    
}

void VideoManager::loadTest()
{
    string name = "TEST_VIDEO";
    auto& videoPaths = AppManager::getInstance().getSettingsManager().getVideoResourcesPath();
    
    if(videoPaths.find(name)!=videoPaths.end())
    {
//        ofDisableDataPath();
//        string path = videoPaths.at(name);
//        path = "../../../" + path; //To make it realtive to PrimaveraSoundProto.app
//        ofDirectory dir(path);
//        this->loadVideo(path);
//        ofEnableDataPath();
        
        string path = videoPaths.at(name);
         this->load(path);
        
    }
    else{
        ofLogNotice() <<"VideoManager::loadTest -> cannot find video: " << name;
    }
}


void VideoManager::exportVideo()
{
    m_exportMode = AppManager::getInstance().getImageManager().startExporting();
    
    if(m_exportMode){
        m_videoPlayer.setFrame(0);
        m_frameNumber = -1;
        //m_videoPlayer.setLoopState(OF_LOOP_NONE);
        m_videoPlayer.play();
        ofLogNotice() <<"VideoManager::exportVideo -> Starting Exportation! ";
    }
    else{
        m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
        m_videoPlayer.play();
    }
    
}

void VideoManager::stopExporting()
{
    ofLogNotice() <<"VideoManager::exportVideo -> Stop Exportation! ";
    
    m_exportMode = false;
    m_frameNumber = -1;
    m_videoPlayer.setFrame(0);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    m_videoPlayer.play();

     AppManager::getInstance().getImageManager().stopExporting();
}

void VideoManager::play()
{
    if(!m_exportMode){
        m_videoPlayer.play();
    }
    
}

void VideoManager::pause()
{
    if(!m_exportMode){
        m_videoPlayer.setPaused(true);
    }
}




/*
 *  ImageManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "ofMain.h"

#include <time.h>

#include "ImageManager.h"
#include "AppManager.h"


ImageManager::ImageManager(): Manager(), m_isRecording(false), m_mirror(false)
{
	//Intentionally left empty
}


ImageManager::~ImageManager()
{
    ofLogNotice() <<"ImageManager::Destructor" ;
}


void ImageManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    
    m_image.clear();
    m_colors.clear();
    
    ofLogNotice() <<"ImageManager::initialized" ;
    
}


void ImageManager::update()
{
    if (m_isRecording) {
         this->updateColorPixels();
    }
}

void ImageManager::draw()
{
    
}

void ImageManager::updateColorPixels()
{
    bool newFrame = AppManager::getInstance().getLedsManager().isNewFrame();
    
    if(!newFrame ){
        return;
    }
    
    auto leds = AppManager::getInstance().getLedsManager().getLeds();
   // ofLogNotice() <<"ImageManager::updateColorPixels" ;
    for (auto& led: leds){
        m_colors.push_back(led->getColor());
    }
    
}




void ImageManager::record(bool value)
{
    if (m_isRecording && !value) {
        ofLogNotice() <<"ImageManager::record: Stop Recording!!" ;
        this->saveImage();
    }
    else{
        ofLogNotice() <<"ImageManager::record: Start Recording!!" ;
        m_image.clear();
        m_colors.clear();
    }
    
     m_isRecording = value;
}


void ImageManager::saveImage()
{
    
    if(m_mirror){
        this->saveImageMirror();
    }
    else{
        this->saveImageSample();
    }
    

    m_image.clear();
    m_colors.clear();
}



void ImageManager::saveImageMirror()
{
    if(m_exportPath.empty()){
        ofLogNotice() <<"ImageManager::saveImageMirror ->  export path is empty";
        return;
    }
    
    auto leds = AppManager::getInstance().getLedsManager().getLeds();
    

    int width = leds.size();
    int height = 0;
    
    if(width>0){
        height = 2*m_colors.size()/width;
    }
    
    ofLogNotice() <<"ImageManager::saveImageMirror ->  width = " << width;
    ofLogNotice() <<"ImageManager::saveImageMirror ->  height = " << height;
    
    m_image.clear();
    m_image.allocate(width, height, OF_IMAGE_COLOR);
    
    
    ofPixelsRef pixels = m_image.getPixelsRef();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int n = x + y*width;
            
            if ( y >= height/2) {
                int y_ = height - y - 1;
                n = x + y_* width;
            }
            
            
            pixels.setColor(x, y, m_colors[n]);
        }
    }
    
    m_image.update(); // uploads the new pixels to the gfx card
    
    //string fileName = "images/saved/image_vase_" + getDateTime() +  ".bmp";
    m_image.save(m_exportPath);
    
    ofLogNotice() <<"ImageManager::saveImageMirror ->  Saved " << m_exportPath;
    
}


void ImageManager::saveImageSample()
{
    
    auto leds = AppManager::getInstance().getLedsManager().getLeds();
    
    int width = leds.size();
    int height = 0;
    
    if(width>0){
        height = m_colors.size()/width;
    }
    
    ofLogNotice() <<"ImageManager::saveImageSample ->  width = " << width;
    ofLogNotice() <<"ImageManager::saveImageSample ->  height = " << height;
    
    
    m_image.clear();
    m_image.allocate(width, height, OF_IMAGE_COLOR);
    
    ofPixelsRef pixels = m_image.getPixelsRef();
    
    /*
    In a bottom-up DIB, the image buffer starts with the bottom row of pixels, followed by the next row up,
    and so forth. The top row of the image is the last row in the buffer.
    Therefore, the first byte in memory is the bottom-left pixel of the image.
    */
    
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int y_ = height - y - 1;
            int n = x + y_*width;
            pixels.setColor(x, y, m_colors[n]);
        }
    }
    
    m_image.update(); // uploads the new pixels to the gfx card
    
    m_image.save(m_exportPath);
    
    ofLogNotice() <<"ImageManager::saveImageSample ->  Saved " << m_exportPath;
    
}

string ImageManager::getDateTime()
{
    char buffer[80];
    string fmt="%d-%m-%Y-%H-%M-%S";
    time_t rawtime;
    time ( &rawtime );
    
    struct tm *timeinfo = localtime(&rawtime);
    strftime(buffer,80,fmt.c_str(),timeinfo);
    return buffer;
}


bool ImageManager::startExporting()
{
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".bmp", "Export your file");
    
    if (saveFileResult.bSuccess){
        //m_exportPath = saveFileResult.filePath;
        m_exportPath = ofSplitString(saveFileResult.filePath, ".bmp")[0] + ".bmp";
        ofLogNotice() <<"ImageManager::startExporting ->  Path: " << m_exportPath;
        this->record(true);
        return true;
    }
    
    return false;
}

void ImageManager::stopExporting()
{
   this->record(false);
}



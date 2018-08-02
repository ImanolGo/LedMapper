/*
 *  LayoutManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */




#include "ofMain.h"

#include "AppManager.h"
#include "SettingsManager.h"
#include "ViewManager.h"


#include "LayoutManager.h"

const int LayoutManager::MARGIN = 20;
const int LayoutManager::FRAME_MARGIN = 2;

const string LayoutManager::LAYOUT_FONT =  "fonts/open-sans/OpenSans-Semibold.ttf";
const string LayoutManager::LAYOUT_FONT_LIGHT =  "fonts/open-sans/OpenSans-Light.ttf";

LayoutManager::LayoutManager(): Manager(), m_drawMode(0)
{
	//Intentionally left empty
}


LayoutManager::~LayoutManager()
{
    ofLogNotice() <<"LayoutManager::Destructor";
}


void LayoutManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"LayoutManager::initialized";

	Manager::setup();

  
    this->setupFbos();
    this->setupWindowFrames();
    
    this->createTextVisuals();
    this->createImageVisuals();
}


void LayoutManager::setupFbos()
{
    int margin = MARGIN;
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    ofPtr<ofFbo> fbo = ofPtr<ofFbo>(new ofFbo());
    fbo->allocate(width, height, GL_RGBA);
    fbo->begin(); ofClear(0);  fbo->end();
    m_fbos["Video"] = fbo;
    
    fbo = ofPtr<ofFbo>(new ofFbo());
    fbo->allocate(width, height, GL_RGBA);
    fbo->begin(); ofClear(0);  fbo->end();
    m_fbos["Leds"] = fbo;
    
}


void LayoutManager::setupWindowFrames()
{
    ofColor color = AppManager::getInstance().getSettingsManager().getColor("FrameRectangle");
    
    for (auto& fbo : m_fbos)
    {
        ofPtr<ofRectangle>  rect = ofPtr<ofRectangle> (new ofRectangle());
        m_windowRects[fbo.first] = rect;
        
        ofPtr<RectangleVisual>  rectVisual = ofPtr<RectangleVisual> (new RectangleVisual());
        m_windowFrames[fbo.first] = rectVisual;
        m_windowFrames[fbo.first]->setColor(color);
    }
    
    this->resetWindowRects();
    this->resetWindowFrames();
}


void LayoutManager::resetWindowRects()
{
    
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    float ratio = width/ height;
    float frame_width = ofGetWidth() - AppManager::getInstance().getGuiManager().getWidth() - 2*MARGIN;
    float frame_height= ofGetWindowHeight() - 2*MARGIN;
    
    
    int i = 0;
    for (auto& rect : m_windowRects)
    {
        rect.second->height = frame_width/m_windowRects.size() - 2*MARGIN;
        rect.second->width = rect.second->height * ratio;
        
        if(rect.second->width > frame_width  - 4*MARGIN){
            rect.second->width = frame_width  - 4*MARGIN;
            rect.second->height = rect.second->width/ratio;
        }
        
        rect.second->x = AppManager::getInstance().getGuiManager().getWidth()  + 4*MARGIN;
        rect.second->y = i*rect.second->height + 2*i*MARGIN  + 2*MARGIN;;
        i++;
    }
}


void LayoutManager::resetWindowFrames()
{
    for (auto& rect : m_windowRects)
    {
        m_windowFrames[rect.first]->setPosition(ofPoint(rect.second->x - FRAME_MARGIN, rect.second->y - FRAME_MARGIN));
        m_windowFrames[rect.first]->setWidth(rect.second->width + 2*FRAME_MARGIN);
        m_windowFrames[rect.first]->setHeight(rect.second->height+ 2*FRAME_MARGIN);
    }
}


void LayoutManager::update()
{
    if(!m_initialized)
        return;
    
    this->updateFbos();
}

void LayoutManager::updateFbos()
{
    this->updateVideoFbo();
    this->updateLedsFbo();
}

void LayoutManager::updateVideoFbo()
{
    string name = "Video";
    this->begin(name);
    AppManager::getInstance().getVideoManager().draw();
    this->end(name);
}



void LayoutManager::updateLedsFbo()
{
    float width = AppManager::getInstance().getSettingsManager().getAppWidth();
    float height  = AppManager::getInstance().getSettingsManager().getAppHeight();
    
    string name = "Leds";
    this->begin(name);
    ofClear(0);
    AppManager::getInstance().getModelManager().draw();
    this->end(name);
}

void LayoutManager::createTextVisuals()
{
    float size = 14;
    
    for (auto& rect : m_windowRects)
    {
        float w = rect.second->width;
        float h = size;
        float x =  rect.second->x + w*0.5;
        float y =  rect.second->y - h - MARGIN;
        ofPoint pos = ofPoint(x, y);
        string text = rect.first;
        string fontName = LAYOUT_FONT;
        
        auto textVisual = ofPtr<TextVisual>(new TextVisual(pos,w,h,true));
        textVisual->setText(text, fontName, size, ofColor::white);
        m_textVisuals[rect.first] = textVisual;
    }
    
}


void LayoutManager::resetWindowTitles()
{
    for (auto& rect : m_windowRects)
    {
        float x =  rect.second->x + rect.second->width*0.5;
        float y =  rect.second->y -  m_textVisuals[rect.first]->getHeight()*0.5 - MARGIN*0.5;
        ofPoint pos = ofPoint(x, y);
        m_textVisuals[rect.first]->setPosition(pos);
    }
}



void LayoutManager::createImageVisuals()
{
    //this->createBackground();
}

void LayoutManager::createBackground()
{
    
}

void LayoutManager::addVisuals()
{
    int depthLevel = -1;

    for(TextMap::iterator it = m_textVisuals.begin(); it!= m_textVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
    
    for(ImageMap::iterator it = m_imageVisuals.begin(); it!= m_imageVisuals.end(); ++it){
        AppManager::getInstance().getViewManager().addOverlay(it->second,depthLevel);
    }
}

void LayoutManager::onFullScreenChange(bool value)
{
    if(value){
        ofSetWindowShape(ofGetScreenWidth(),ofGetScreenHeight());
    }
    else{
        
        float width = 4*MARGIN + 2*AppManager::getInstance().getGuiManager().getWidth();
        float height = AppManager::getInstance().getGuiManager().getHeight() + 2*MARGIN;
        ofSetWindowShape(width,height);
    }
}


void LayoutManager::draw()
{
    if(!m_initialized)
        return;
    
    switch (m_drawMode)
    {
        case DRAW_NORMAL:  this->drawNormal(); break;
        case DRAW_VIDEO:  this->drawVideo(); break;
        case DRAW_LEDS:  this->drawLeds(); break;
        default: this->drawNormal(); break;
    }
   
}

void LayoutManager::drawVideo()
{	
    m_fbos["Video"]->draw(0,0, ofGetWidth(), ofGetHeight());
}

void LayoutManager::drawLeds()
{
    m_fbos["Leds"]->draw(0,0, ofGetWidth(), ofGetHeight());
}

void LayoutManager::drawNormal()
{
    this->drawFbos();
    this->drawText();
}


void LayoutManager::drawText()
{
    for(auto textVisual: m_textVisuals){
        textVisual.second->draw();
    }
}

void LayoutManager::drawFbos()
{
    for (auto fbo : m_fbos)
    {
        m_windowFrames[fbo.first]->draw();
        //fbo.second->draw(m_windowRects[fbo.first]->x, m_windowRects[fbo.first]->y, m_windowRects[fbo.first]->width, m_windowRects[fbo.first]->height);
        //fbo.second->draw(0,0);
        m_fbos[fbo.first]->draw(m_windowRects[fbo.first]->x, m_windowRects[fbo.first]->y, m_windowRects[fbo.first]->width, m_windowRects[fbo.first]->height);
    }
}


void LayoutManager::windowResized(int w, int h)
{
    if(!m_initialized){
        return;
    }
    
    this->resetWindowRects();
    this->resetWindowFrames();
    this->resetWindowTitles();
}

void LayoutManager::begin(string& name)
{
    if(m_fbos.find(name) == m_fbos.end()){
        return;
    }
    
    m_fbos[name]->begin();
    ofClear(0);
    
}

void LayoutManager::end(string& name)
{
    if(m_fbos.find(name) == m_fbos.end()){
        return;
    }
    
    m_fbos[name]->end();
}

void LayoutManager::toggleDrawMode()
{
    if(m_drawMode == 0)
    {
        m_drawMode = 1;
    }
    else{
        m_drawMode = 0;
    }
    
}



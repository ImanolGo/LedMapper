/*
 *  ViewManager.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "BasicVisual.h"
#include "VisualEffectsManager.h"
#include "VisualEffects.h"
#include "ViewManager.h"

#include "AppManager.h"

#include "ofMain.h"


ViewManager::ViewManager(): Manager(), m_showDebugInfo(true)
{
	// intentionally left empty
}

ViewManager::~ViewManager()
{
    ofLogNotice() <<"ViewManager::destructor";
}


void ViewManager::setup()
{
    if(m_initialized)
        return;

    Manager::setup();

    //this->setupGL();
    //this->setup3D();
    this->setupTextVisuals();

    ofLogNotice() << "ViewManager::initialized ";
}

void ViewManager::setupGL()
{
    
    ofSetVerticalSync(true);
    ofEnableLighting();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();
    
    // ofPlane uses texture coordinates from 0-1, so you can load whatever
    // sized images you want and still use them to texture your box
    // but we have to explicitly normalize our tex coords here
    ofEnableNormalizedTexCoords();
}

void ViewManager::setup3D()
{
    m_light.setPosition(0,0,100);
    m_light.setAmbientColor(ofFloatColor(1.0f, 1.0f,1.0f, 1.0f));
    m_light.setDiffuseColor(ofFloatColor(1.0f, 1.0f,1.0f, 1.0f));
    m_light.setSpecularColor(ofFloatColor(1.0f, 1.0f,1.0f, 1.0f));
    m_cam.setAutoDistance(true);
}

void ViewManager::setupTextVisuals()
{
    ofVec3f position(42,16);
    float fontSize = 16;
    float width = 256;
    float height = fontSize;
    string text = "";
    string fontName ="fonts/helvetica-neue-medium.ttf";
    ofColor textColor = ofColor::white;

    //m_frameRateText = ofPtr<TextVisual> (new TextVisual(position,width,height));
    //m_frameRateText->setText(text,fontName,fontSize,textColor);
}

void ViewManager::update()
{
    if(m_showDebugInfo){
        //string text = "fps: " + ofToString(ofGetFrameRate(), 2);
        //m_frameRateText->setText(text);
	}
}



void ViewManager::draw()
{
    
    this->drawBackround();
    this->drawOverlays();
    this->drawVisuals();
    
//    ofPushStyle();
//    ofSetColor(ofColor::white);
//    string text = "fps: " + ofToString(ofGetFrameRate(), 2);
//    ofDrawBitmapString(text, 20, 20);
//    ofPopStyle();
}

void ViewManager::drawBackround()
{
    //ofClear ( 0 );
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    //ofColor centerColor = AppManager::getInstance().getSettingsManager()->getColor("CenterBackgroundColor");
    //ofColor edgeColor = AppManager::getInstance().getSettingsManager()->getColor("EdgeBackgroundColor");
    //ofBackgroundGradient(centerColor,edgeColor,OF_GRADIENT_CIRCULAR);
}

void ViewManager::drawVisuals()
{
    ofEnableAlphaBlending();
    
    m_cam.begin();
    m_light.enable();
    ofEnableLighting();
    glEnable(GL_DEPTH_TEST);
  
    
    for(VisualList::iterator it = m_visuals.begin(); it != m_visuals.end(); it++) {
        ofPushMatrix();
        (*it)->draw();
        ofPopMatrix();
    }
    
    glDisable(GL_DEPTH_TEST);
    ofDisableLighting();
    m_light.disable();
    m_cam.end();
    ofDisableAlphaBlending();
}

void ViewManager::drawOverlays()
{
    ofEnableAlphaBlending();
    for(OverlayList::reverse_iterator it = m_overlays.rbegin(); it != m_overlays.rend(); it++)
    {
		ofPushMatrix();
		it->second->draw();
		ofPopMatrix();
	}
    ofDisableAlphaBlending();

}

void ViewManager::addOverlay(ofPtr<BasicVisual> visual,int zOrder)
{
    if(!visual){
        return;
    }

    if(isOverlayAlreadyAdded(visual)){
        return;
    }
    
	m_overlays.push_back(Overlay(zOrder,visual));
	m_overlays.sort();
}


void ViewManager::removeOverlay(ofPtr<BasicVisual> visual)
{
    if(!visual){
        return;
    }

	for(OverlayList::iterator it = m_overlays.begin(); it != m_overlays.end();) {
		if(it->second == visual) {
            AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(it->second);
			it = m_overlays.erase(it);
		}
		else {
			++it;
		}
	}
}

void ViewManager::addVisual(ofPtr<BasicVisual> visual)
{
    if(!visual){
        return;
    }
    
    if(isVisualAlreadyAdded(visual)){
        return;
    }

    
    m_visuals.push_back(visual);
}

void ViewManager::removeVisual(ofPtr<BasicVisual> visual)
{
    for(VisualList::iterator it = m_visuals.begin(); it != m_visuals.end();) {
        if(*it== visual) {
            AppManager::getInstance().getVisualEffectsManager().removeAllVisualEffects(*it);
            it = m_visuals.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool ViewManager::isOverlayAlreadyAdded(ofPtr<BasicVisual> visual)
{
    if(!visual)
        return true;

	for(OverlayList::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it) {
		if(it->second == visual) {
           return true;
		}
	}

	return false;
}

bool ViewManager::isVisualAlreadyAdded(ofPtr<BasicVisual> visual)
{
    if(!visual)
        return true;
    
    for(VisualList::iterator it = m_visuals.begin(); it != m_visuals.end(); ++it) {
        if(*it == visual) {
            return true;
        }
    }
    
    return false;
}







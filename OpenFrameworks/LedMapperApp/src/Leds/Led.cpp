/*
 *  Led.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "Led.h"
#include "AppManager.h"


const int Led::SIZE = 4;

Led::Led(const ofPoint& position, int id): BasicVisual(position, SIZE, SIZE), m_id(id)
{
    //Intentionaly left empty
}

Led::~Led()
{
    //Intentionaly left empty
}

void Led::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(m_position);
    ofRotateX(m_rotation.x);
    ofRotateY(m_rotation.y);
    ofRotateZ(m_rotation.z);
    ofScale(m_scale.x, m_scale.y, m_scale.z);
    
    ofSetColor(m_color);
    ofFill();
    
        ofDrawSphere(0, 0, m_width);
    
    ofPopStyle();
    ofPopMatrix();
}


void Led::normalize(const ofRectangle& boundingBox)
{
    m_position.x = (m_position.x - boundingBox.getX()) / boundingBox.getWidth();
    m_position.y = (m_position.y - boundingBox.getY()) / boundingBox.getHeight();
}


void Led::setPixelColor(ofPixelsRef pixels)
{
    float minX = AppManager::getInstance().getLedsManager().getMin().x;
    float maxX = AppManager::getInstance().getLedsManager().getMax().x;
    
    float minY = AppManager::getInstance().getLedsManager().getMin().z;
    float maxY = AppManager::getInstance().getLedsManager().getMax().z;
    
    float minZ = AppManager::getInstance().getLedsManager().getMin().y;
    float maxZ = AppManager::getInstance().getLedsManager().getMax().y;
    
    float treshold = minZ + (maxZ - minZ)*0.5;
    
    
    ofVec2f pixelPos;
    if(m_position.y >= treshold ){
        pixelPos.x = ofMap(m_position.x, minX, maxX, 0, (pixels.getWidth()-1)*0.5);
        pixelPos.y = ofMap(m_position.z, maxY, minY, 0,  pixels.getHeight()-1);
    }
    else{
        pixelPos.x = ofMap(m_position.x, minX, maxX, pixels.getWidth()-1, (pixels.getWidth()-1)*0.5);
        pixelPos.y = ofMap(m_position.z, maxY, minY, 0,  pixels.getHeight()-1);
    }
    
    
    //ofLogNotice() <<  m_position.x ; ofLogNotice() <<  m_position.y;
    //ofLogNotice() <<  pixelPos.x ; ofLogNotice() <<  pixelPos.y;
   //ofLogNotice() << pixelPos.x;
    
    m_color = pixels.getColor((int)pixelPos.x, (int)pixelPos.y);
}


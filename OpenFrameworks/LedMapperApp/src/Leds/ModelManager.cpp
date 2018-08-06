/*
 *  ModelManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */


#include "ofMain.h"

#include "ModelManager.h"
#include "AppManager.h"


ModelManager::ModelManager(): Manager()
{
	//Intentionally left empty
}


ModelManager::~ModelManager()
{
    ofLogNotice() <<"ModelManager::Destructor" ;
}



void ModelManager::setup()
{
	if(m_initialized)
		return;

    ofLogNotice() <<"ModelManager::initialized" ;

	Manager::setup();
    
    this->resetCamera();
}

void ModelManager::resetCamera()
{
    m_camera.reset();
    m_camera.setDistance(120);
    //m_camera.enableInertia();
    //m_camera.setAutoDistance(true);
}


void ModelManager::update()
{
    
}


void ModelManager::draw()
{
    ofBackgroundGradient( ofColor(100), ofColor(15), OF_GRADIENT_CIRCULAR );
    
    ofEnableDepthTest();
    m_camera.begin();
    this->drawLeds();
    m_camera.end();
    ofDisableDepthTest();
    
}

void ModelManager::drawLeds()
{
    float max = 100;
    auto ledVector = AppManager::getInstance().getLedsManager().getLeds();
    for (auto led: ledVector) {
        auto pos = led->getPosition();

        ofPushMatrix();
            ofTranslate(pos*max);
            led->draw();
        ofPopMatrix();
    }
}

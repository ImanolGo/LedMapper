/*
 *  LightManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */

#include "LightManager.h"


const float LightManager::NEAR_DISTANCE = 1050;
const float LightManager::FAR_DISTANCE = 10500;

LightManager::LightManager():Manager()
{
    //Intentionally left empty
}

LightManager::~LightManager()
{
    ofLogNotice() << "LightManager::destructor";
}

//--------------------------------------------------------------
void LightManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"LightManager::initialized";
    
     // light the scene to show off why normals are important
    this->enable();
}


void LightManager::update()
{
   //Here comes some light animations
}

void LightManager::enable()
{
    ofEnableLighting();
    for(LightMap::iterator it = m_lights.begin(); it!=m_lights.end(); ++it){
        it->second->enable();
    }
}

void LightManager::disable()
{
    ofDisableLighting();
    for(LightMap::iterator it = m_lights.begin(); it!=m_lights.end(); ++it){
        it->second->disable();
    }
}


void LightManager::draw()
{
    ofPushMatrix();

        for(LightMap::iterator it = m_lights.begin(); it!=m_lights.end(); ++it){
            it->second->draw();
        }

    ofPopMatrix();
}







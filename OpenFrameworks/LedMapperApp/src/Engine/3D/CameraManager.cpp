/*
 *  CameraManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */



#include "CameraAnimationsManager.h"
#include "CameraAnimations.h"

#include "CameraManager.h"

const float CameraManager::GLOBAL_VIEW_DISTANCE = 1750;
const float CameraManager::DISTANCE_TO_CUBE_SPHERE = 250;

CameraManager::CameraManager(): Manager()
{
    //Intentionally left empty
}

CameraManager::~CameraManager()
{
    //Intentionally left empty
}


//--------------------------------------------------------------
void CameraManager::setup()
{
    if(m_initialized)
        return;
    
    ofLogNotice() <<"CameraManager::initialized";
    
    Manager::setup();
    
    this->setupCamera();
}

void CameraManager::setupCamera()
{
    m_camera = ofPtr<ofEasyCam> (new ofEasyCam());
    m_cameraAnimationsManager = ofPtr<CameraAnimationsManager> (new CameraAnimationsManager());
}

void CameraManager::update()
{
    m_cameraAnimationsManager->update();
}

void CameraManager::begin()
{
    m_camera->begin();
}

void CameraManager::end()
{
    m_camera->end();
}



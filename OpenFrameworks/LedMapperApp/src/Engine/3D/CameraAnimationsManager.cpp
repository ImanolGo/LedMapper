/*
 *  CameraAnimationsManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */


#include "CameraAnimationsManager.h"
#include "CameraAnimations.h"
#include "ofMain.h"

CameraAnimationsManager::CameraAnimationsManager()
{
	// intentionally left empty
}


CameraAnimationsManager::~CameraAnimationsManager()
{
    ofLogNotice() << "CameraAnimationsManager::destructor";
}



void CameraAnimationsManager::update()
{
    for(CameraAnimationVector::iterator it = m_cameraAnimations.begin(); it != m_cameraAnimations.end();)
    {
        (*it)->update();
        
        if((*it)->isFinished()) {
            it = m_cameraAnimations.erase(it);
        }
        else{
            ++it;
        }
    }
}

void CameraAnimationsManager::addCameraAnimation(ofPtr<CameraAnimation> cameraAnimation)
{
    for(CameraAnimationVector::iterator it = m_cameraAnimations.begin(); it != m_cameraAnimations.end();it++) {
        if(*it == cameraAnimation) {
            return;
        }
    }
    m_cameraAnimations.push_back(cameraAnimation);
}

void CameraAnimationsManager::removeCameraAnimation(ofPtr<CameraAnimation> cameraAnimation)
{
    if(!cameraAnimation){
        return;
    }
    
    for(CameraAnimationVector::iterator it = m_cameraAnimations.begin(); it != m_cameraAnimations.end();) {
        if(*it == cameraAnimation) {
            it = m_cameraAnimations.erase(it);
        }
        else
        {
            ++it;
        }
    }
}



void CameraAnimationsManager::removeAllCameraAnimations(ofPtr<ofEasyCam>camera)
{
    for(CameraAnimationVector::iterator it = m_cameraAnimations.begin(); it != m_cameraAnimations.end();) {
        if((*it)->getCamera() == camera) {
            it = m_cameraAnimations.erase(it);
        }
        else {
            ++it;
        }		
    }
}















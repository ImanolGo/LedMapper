/*
 *  CameraAnimationManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */

#pragma once


#include "Manager.h"
#include "CameraAnimations.h"
#include <ofMain.h>

//====================== class CameraAnimationsManager =======================
//============================================================================
/** \class CameraAnimationsManager CameraAnimationsManager.h
 *	\brief Manages arbitrary camera animations
 */
//==============================================================================

class CameraAnimationsManager: public Manager
{
public:
    
    //! Constructor
    CameraAnimationsManager();
    
    //! Destructor
    ~CameraAnimationsManager();
    
    //! setups the Camera
    virtual void setup(){}
    
    //! Updates all the visual effects
	virtual void update();
    
	//! Adds a certain camera animation
	void addCameraAnimation(ofPtr<CameraAnimation> cameraAnimation);
    
	//! Removes a certain camera animation from the list
	void removeCameraAnimation(ofPtr<CameraAnimation> cameraAnimation);
    
	//! Removes all camera animations for the specified camera
	void removeAllCameraAnimations(ofPtr<ofEasyCam> camera);
    
    //==========================================================================
    
    
private:
    
    
    typedef std::vector<ofPtr<CameraAnimation> >  CameraAnimationVector;
	CameraAnimationVector	m_cameraAnimations; ///< Vector of all camera animations
    
};

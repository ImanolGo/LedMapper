/*
 *  CameraManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */

#pragma once

#include "CameraAnimationsManager.h"
#include "Manager.h"
#include "ofMain.h"


//========================== class CameraManager =======================================
//==============================================================================
/** \class CameraManager CameraManager.h
 *	\brief class for managing the camera and its animations
 *	\details It uses all the ofEasyCam functionalities
 */


class CameraManager: public Manager
{

    static const float GLOBAL_VIEW_DISTANCE;     ///< Defines the camera distance at the global view perspective
    static const float DISTANCE_TO_CUBE_SPHERE;  ///< Defines the camera distance to the cube sphere

public:

    //! Constructor
    CameraManager();

    //! Destructor
    virtual ~CameraManager();

    //! setups the camera manager
    void setup();

    //! update the animations
    void update();

    //! Start of camera view. Everything that is in here will be affected by the camera view
    void begin();

    //! End of camera view.
    void end();

    //! get camera animation manager
    ofPtr<CameraAnimationsManager> getCameraAnimationsManager() {return m_cameraAnimationsManager;}

protected:

    //! setups the camera
    void setupCamera();



protected:

    ofPtr<ofEasyCam>  m_camera; // add mouse controls for camera movement
    ofPtr<CameraAnimationsManager>  m_cameraAnimationsManager; //class in charge of the camera animations

};


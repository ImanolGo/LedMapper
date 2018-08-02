/*
 *  ModelManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#pragma once

#include "Manager.h"


//========================== class ModelManager ==============================
//============================================================================
/** \class ModelManager ModelManager.h
 *	\brief Class managing the model visuals
 *	\details it creates and updates the 3D model visuals
 */


class ModelManager: public Manager
{
    public:

        //! Constructor
        ModelManager();

        //! Destructor
        ~ModelManager();

        //! Setup the Vase Manager
        void setup();

        //! Update the Vase Manager
        void update();

        //! Draw the Vase Manager
        void draw();


    private:

        //! Draw all Leds
        void drawLeds();

    private:
    
        ofEasyCam m_cam; // add mouse controls for camera movement
    
        ofFbo     m_fbo;

    

};

/*
 *  LightManager.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */

#include "ofMain.h"
#include "Manager.h"

//========================== class LightManager =======================================
//==============================================================================
/** \class LightManager LightManager.h
 *	\brief class for controlling the light in the 3D virtual world
 *	\details ...
 */


class LightManager: public Manager
{

    static const float NEAR_DISTANCE;   ///< Defines the distance to the object as being near
    static const float FAR_DISTANCE;    ///< Defines the distance to the object as being far

    public:
        //! Constructor
        LightManager();

        //! Destructor
        virtual ~LightManager();

        //! setups the light
        virtual void setup();

        //! updates the class
        virtual void update();

        //! draws the light point
        virtual void draw();

        //! enables light
        void enable();

        //! disables light light
        void disable();

    protected:

        typedef std::map<string, ofPtr<ofLight> > LightMap; ///< Map of Light points with a string as key

        LightMap m_lights;   ///< Map of light sources of the 3D scene

 };


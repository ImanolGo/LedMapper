/*
 *  ImageVisual3D.h
 *
 *  Created by Imanol Gómez on 17/02/15.
 *
 */

#pragma once


#include "ImageVisual.h"


//=========================== class ImageVisual3D ==============================
//============================================================================
/** \class ImageVisual3D ImageVisual3D.h
 *	\brief Represents a 3D image visual
 *	\details The class uses the OF class ofImage to draw the 2D image in a 3D world
 */

class ImageVisual3D: public ImageVisual
{

public:

    //! Constructor
    ImageVisual3D(const ofVec3f& pos, const string& resourceName);

    //! Destructor
    virtual ~ImageVisual3D();

	//! Draws the text visual
	virtual void draw();


protected:

    ofPtr<ofPlanePrimitive>   m_plane;           ///< shared pointer to the ofPlanePrimitive class


};

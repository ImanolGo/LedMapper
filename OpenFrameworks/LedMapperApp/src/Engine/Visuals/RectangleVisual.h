/*
 *  RectangleVisual.h
 *
 *  Created by Imanol Gomez on 01/10/15.
 *
 */

#pragma once

#include "BasicVisual.h"


//=========================== class RectangleVisual ==============================
//============================================================================
/** \class RectangleVisual RectangleVisual.h
 *	\brief Represents an rectangle visual
 *	\details The class uses the OF class ofRect to draw a 2D rectangle
 */

class RectangleVisual: public BasicVisual
{

public:

    //! Constructor
    RectangleVisual();
    
    //! Constructor
    RectangleVisual(const ofVec3f& pos, float width, float height, bool centred = false);

    //! Destructor
    virtual ~RectangleVisual();

	//! Draws the image visual
	virtual void draw();
    
protected:
    
    bool     m_centred;          ///< defines if the visual should be centred or not

};



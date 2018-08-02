/*
 *  ImageVisual.h
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */

#pragma once

#include "BasicVisual.h"


//=========================== class ImageVisual ==============================
//============================================================================
/** \class ImageVisual Visual.h
 *	\brief Represents an image visual
 *	\details The class uses the OF class ofTexture to draw a 2D image
 */

class ImageVisual: public BasicVisual
{

public:

    //! Constructor
    ImageVisual();
    
    //! Constructor
    ImageVisual(const ofVec3f& pos, const string& resourceName, bool centred = false);

    //! Destructor
    virtual ~ImageVisual();

	//! Draws the image visual
	virtual void draw();

    //! Sets the texture to be binded to the quad
    virtual bool setResource(const string& resourceName);
    
    //! Sets the texture to be binded to the quad
    virtual void setCentred(bool centred) {m_centred = centred;}

    //! Set the width
    virtual void setWidth(float width, bool keepRatio=false);

    //! Set the height
    virtual void setHeight(float height, bool keepRatio=false);

    //! Get the original width
    float getOriginalWidth() const {return m_originalWidth;}

    //! Get the original height
    float getOriginalHeight() const {return m_originalHeight;}
    
    ofTexture & getTexture();


protected:

    ofPtr<ofTexture>    m_texture;          ///< ofPtr to the texture
    ofVec3f             m_resizer;          ///< it deales with the resize of the picture
    bool                m_centred;          ///< defines if the visual should be centred or not
    float               m_originalWidth;    ///< stores the original width of the image in pixels
    float               m_originalHeight;   ///< stores the original height of the image in pixels
};



/*
 *  VideoVisual.h
 *
 *  Created by Imanol Gomez on 27/08/15.
 *
 */

#pragma once

#include "BasicVisual.h"


//=========================== class VideoVisual ==============================
//============================================================================
/** \class VideoVisual Visual.h
 *	\brief Represents a video visual
 *	\details The class uses the OF class ofVideoPlayer to draw a video
 */

class VideoVisual: public BasicVisual
{

public:

    //! Constructor
    VideoVisual();
    
    //! Constructor
    VideoVisual(const ofVec3f& pos, const string& resourceName, bool centred = false);

    //! Destructor
    virtual ~VideoVisual();
    
    //! Update the video visual
    virtual void update();

	//! Draws the video visual
	virtual void draw();

    //! Sets the video to be loaded
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
    
    void setLoopState(ofLoopType state) {m_video.setLoopState(state);}
    
    void play() {m_video.play();}
    
    void stop() {m_video.stop();}
    
    void setSpeed(float speed) {m_video.setSpeed(speed);}
    
    void setFrame(int frame) {m_video.setFrame(frame);}


protected:

    ofVideoPlayer       m_video;            ///< video player
    bool                m_centred;          ///< defines if the visual should be centred or not
    float               m_originalWidth;    ///< stores the original width of the image in pixels
    float               m_originalHeight;   ///< stores the original height of the image in pixels
};



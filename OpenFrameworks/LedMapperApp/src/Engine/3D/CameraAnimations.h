/*
 *  CameraAnimation.h
 *
 *  Created by Imanol Gomez on 16/02/15.
 *
 */

#pragma once

#include "ofMain.h"
#include "VisualEffects.h"



//==============================================================================
/** \class CameraAnimation CameraAnimations.h
 *	\brief Base class for all the camera animations.
 *	\details Provides basic member attributes which are in common for all
 *           camera animations.
 */
//==============================================================================

class CameraAnimation
{
public:

	//! Constructor
	CameraAnimation(ofPtr<ofEasyCam> camera, EasingFunction function = LINEAR, EasingType type = EASE_IN);

	//! Destructor
	virtual ~CameraAnimation(){}

	//! Updates the elapsed time and visual effect parameters
	virtual void update() = 0;

	//! Starts the visual effect
	virtual void start(double startTime = 0.0);

	//! Stops the visual effect
	virtual void stop();
    
    //! Finishes the visual effect
    virtual void finish();

	//! Returns the camera this animation is associated with
	ofPtr<ofEasyCam> getCamera() { return m_camera; }

    //! Returns the current state of the animation
	bool isActive() const { return m_isActive; }
    
    //! Returns if the animation is finished
    bool isFinished() const { return m_isFinished; }


protected:

	//! Returns the update value from the selected function.
	//! t: current time, from: start value, to: end value, duration: the total animation time
	double function(double t, double from, double to, double duration) const;

protected:

	ofPtr<ofEasyCam>          m_camera;				///< defines a camera reference
	bool					m_isActive;				///< determines whether an animation is currently updated or not
    bool					m_isFinished;           ///< determines if the camera animation has finished
	double					m_animationTime;		///< duration of the animation in ms
	double					m_elapsedTime;			///< elapsed time since the last update
	EasingFunction			m_function;				///< saves what kind of easing function do you want to use
	EasingType				m_type;					///< saves what type of easing you want
	double					m_startTime;			///< start time of the animation
	double					m_elaspedTimeToStart;	///< elapsed time before the start of the animation

};


//==============================================================================
/** \class BoomAnimation CameraAnimations.h
 *	\brief Boom animation class for visuals.
 *	\details Provides camera up/down lifting animations
 */
//==============================================================================

class BoomAnimation: public CameraAnimation
{
public:

	//! Constructor
	BoomAnimation(ofPtr<ofEasyCam> camera, EasingFunction function = LINEAR, EasingType type = EASE_IN);

	//! updates the fade visual if active
	virtual void update();


    //==============================================================================

    //! Sets the final amount of boom and the duration of the animation
	virtual void setParameters(float endAmount, double animationTime);

protected:

    float	m_currentAmount;    ///< current amount value
    float	m_lastAmount;    	///< last amount value
	float	m_endAmount;        ///< end amount value

};

//==============================================================================
/** \class TruckAnimation CameraAnimations.h
 *	\brief Truck animation class for visuals.
 *	\details Provides camera left/right moving animations
 */
//==============================================================================

class TruckAnimation: public CameraAnimation
{
public:

	//! Constructor
	TruckAnimation(ofPtr<ofEasyCam>, EasingFunction function = LINEAR, EasingType type = EASE_IN);

	//! updates the fade visual if active
	virtual void update();


    //==============================================================================

    //! Sets the final amount of boom and the duration of the animation
	virtual void setParameters(float endAmount, double animationTime);

protected:

    float	m_currentAmount;    ///< current amount value
    float	m_lastAmount;    	///< last amount value
	float	m_endAmount;        ///< end amount value

};


//==============================================================================
/** \class DollyAnimation CameraAnimations.h
 *	\brief Dolly animation class for visuals.
 *	\details Provides camera backwards/forwards moving animations
 */
//==============================================================================

class DollyAnimation: public CameraAnimation
{
public:

	//! Constructor
	DollyAnimation(ofPtr<ofEasyCam> camera, EasingFunction function = LINEAR, EasingType type = EASE_IN);


	//! updates the fade visual if active
	virtual void update();


    //==============================================================================

    //! Sets the final amount of boom and the duration of the animation
	virtual void setParameters(float endAmount, double animationTime);

protected:

    float	m_currentAmount;    ///< current amount value
    float	m_lastAmount;    	///< last amount value
	float	m_endAmount;        ///< end amount value

};


//==============================================================================
/** \class TargetAnimation CameraAnimations.h
 *	\brief Target animation class for visuals.
 *	\details Provides animations to change the point at which the camera
 *			 is targeting
 */
//==============================================================================

class TargetAnimation: public CameraAnimation
{
public:

	//! Constructor
	TargetAnimation(ofPtr<ofEasyCam> camera, EasingFunction function = LINEAR, EasingType type = EASE_IN);


	//! updates the fade visual if active
	virtual void update();


    //==============================================================================

    //! Sets the final looking at position and the duration of the animation
	virtual void setParameters(const ofVec3f& endPosition, double animationTime);


protected:

    ofVec3f	m_position;         ///< current looking at position
	ofVec3f	m_startPosition;	///< start looking at position
	ofVec3f	m_endPosition;      ///< end looking at position
};


//==============================================================================
/** \class MovingAnimation CameraAnimations.h
 *	\brief Moving animation class for visuals.
 *	\details Provides animations for the changing camera position
 */
//==============================================================================

class MovingAnimation: public CameraAnimation
{
public:

	//! Constructor
	MovingAnimation(ofPtr<ofEasyCam> camera, EasingFunction function = LINEAR, EasingType type = EASE_IN);


	//! updates the fade visual if active
	virtual void update();


    //==============================================================================

    //! Sets the final looking at position and the duration of the animation
	virtual void setParameters(const ofVec3f& endPosition, double animationTime);

protected:

    ofVec3f	m_position;         ///< current moving position
	ofVec3f	m_startPosition;	///< start moving position
	ofVec3f	m_endPosition;      ///< end moving position
};

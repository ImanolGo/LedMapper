/*
 *  CameraAnimation.cpp
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */

#include <math.h>

#include "ofMain.h"

#include "AppManager.h"
#include "CameraAnimationsManager.h"
#include "CameraManager.h"

#include "CameraAnimations.h"

//==============================================================================
//========================= CAMERA ANIMATIONS ==================================
//==============================================================================


CameraAnimation::CameraAnimation(ofPtr<ofEasyCam> camera, EasingFunction function, EasingType type): m_camera(camera), m_function(function), m_isActive(false), m_animationTime(1000.0), m_elapsedTime(0.0),	m_elaspedTimeToStart(0.0), m_type(type), m_isFinished(false)
{
	// intentionally left empty
}


void CameraAnimation::start(double startTime)
{
    m_elapsedTime = 0.0;
	m_elaspedTimeToStart = 0.0;
	m_isActive = true;
	m_startTime = startTime;
    m_isFinished = false;
}


void CameraAnimation::stop()
{
	m_isActive = false;
}

void CameraAnimation::finish()
{
	m_isFinished = true;
}

double CameraAnimation::function(double t, double from, double to, double duration) const
{
	double c = to - from;
	switch (m_function)
	{
		case LINEAR:
			return  c*(t / duration) + from;
			break;

		case EXPONENTIAL:
			if (m_type== EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return c/2 * std::pow( 2, 10 * (t - 1) ) + from;
				t--;
				return c/2 * ( -std::pow( 2, -10 * t) + 2 ) + from;

			}
			else if (m_type==EASE_OUT)
			{
				return c * ( -std::pow( 2, -10 * t/duration ) + 1 ) + from;
			}
			else
			{
				return c * std::pow( 2, 10 * (t/duration - 1) ) + from;

			}
			break;

		case CIRCULAR:
			if (m_type==EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return -c/2 * (std::sqrt(1 - t*t) - 1) + from;
				t -= 2;
				return c/2 * (std::sqrt(1 - t*t) + 1) + from;
			}
			else if (m_type==EASE_OUT)
			{
				t /= duration;
				t--;
				return c * std::sqrt(1 - t*t) + from;
			}
			else
			{
				t /= duration;
				return -c * (std::sqrt(1 - t*t) - 1) + from;
			}
			break;

		case QUADRATIC:
			if (m_type==EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return c/2*t*t + from;
				t--;
				return -c/2 * (t*(t-2) - 1) + from;

			}
			else if (m_type==EASE_OUT)
			{
				t /= duration;
				return -c * t*(t-2) + from;

			}
			else
			{
				t/=duration;
				return c*t*t + from;
			}
			break;

		case CUBIC:
			if (m_type==EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return c/2*t*t*t + from;
				t -= 2;
				return c/2*(t*t*t + 2) + from;

			}
			else if (m_type==EASE_OUT)
			{
				t /= duration;
				t--;
				return c*(t*t*t + 1) + from;
			}
			else
			{
				t /= duration;
				return c*t*t*t + from;
			}
			break;

		case QUARTIC:
			if (m_type==EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return c/2*t*t*t*t + from;
				t -= 2;
				return -c/2 * (t*t*t*t - 2) + from;

			}
			else if (m_type==EASE_OUT)
			{
				t /= duration;
				t--;
				return -c * (t*t*t*t - 1) + from;

			}
			else
			{
				t /= duration;
				return c*t*t*t*t + from;
			}
			break;

		case QUINTIC:
			if (m_type==EASE_IN_OUT)
			{
				t /= duration/2;
				if (t < 1) return c/2*t*t*t*t*t + from;
				t -= 2;
				return c/2*(t*t*t*t*t + 2) + from;

			}
			else if (m_type==EASE_OUT)
			{
				t /= duration;
				t--;
				return c*(t*t*t*t*t + 1) + from;

			}
			else
			{
				t /= duration;
				return c*t*t*t*t*t + from;
			}
			break;

		case SINUSOIDAL:
			if (m_type==EASE_IN_OUT)
			{
				return -c/2 * (std::cos(PI*t/duration) - 1) + from;
			}
			else if (m_type==EASE_OUT)
			{
				return c * std::sin(t/duration * (PI/2)) + from;

			}
			else
			{
				return -c * std::cos(t/duration * (PI/2)) + c + from;
			}
			break;

		default:
        {
            double percent = c*(t/duration) + from;
            return  c*(t/duration) + from; //linear is the default
        }

	}
}


//==============================================================================
//=============================== BOOM ANIMATION ===============================
//==============================================================================

BoomAnimation::BoomAnimation(ofPtr<ofEasyCam> camera, EasingFunction function,EasingType type):
	CameraAnimation(camera,function,type),  m_currentAmount(0.0), m_lastAmount(0.0),m_endAmount(0.0)
{
	// intentionally left empty
}

void BoomAnimation::setParameters(float endAmount, double animationTime)
{
	m_elapsedTime = 0.0;
	m_endAmount = endAmount;
	m_lastAmount = m_currentAmount = 0.0;
	m_animationTime = animationTime;
}


void BoomAnimation::update()
{
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
		this->finish();
		return;
	}

	m_currentAmount = this->function(m_elapsedTime,0,m_endAmount,m_animationTime);
	float increaseAmount = m_currentAmount-m_lastAmount;
	m_camera->boom(increaseAmount);
	m_lastAmount = m_currentAmount;
}

//==============================================================================
//=============================== DOLLY ANIMATION ==============================
//==============================================================================

DollyAnimation::DollyAnimation(ofPtr<ofEasyCam> camera, EasingFunction function, EasingType type):
	CameraAnimation(camera,function,type),  m_currentAmount(0.0), m_lastAmount(0.0),m_endAmount(0.0)
{
	// intentionally left empty
}

void DollyAnimation::setParameters(float endAmount, double animationTime)
{
	m_elapsedTime = 0.0;
	m_endAmount = endAmount;
	m_lastAmount = m_currentAmount = 0.0;
	m_animationTime = animationTime;
}


void DollyAnimation::update()
{
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
		this->finish();
		return;
	}

	m_currentAmount = this->function(m_elapsedTime,0,m_endAmount,m_animationTime);
	float increaseAmount = m_currentAmount-m_lastAmount;
	m_camera->dolly(increaseAmount);
	m_lastAmount = m_currentAmount;
}

//==============================================================================
//=============================== TRUCK ANIMATION ==============================
//==============================================================================

TruckAnimation::TruckAnimation(ofPtr<ofEasyCam>  camera, EasingFunction function, EasingType type):
	CameraAnimation(camera,function,type),  m_currentAmount(0.0), m_lastAmount(0.0),m_endAmount(0.0)
{
	// intentionally left empty
}

void TruckAnimation::setParameters(float endAmount, double animationTime)
{
	m_elapsedTime = 0.0;
	m_endAmount = endAmount;
	m_lastAmount = m_currentAmount = 0.0;
	m_animationTime = animationTime;
}


void TruckAnimation::update()
{
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
		this->finish();
		return;
	}

	m_currentAmount = this->function(m_elapsedTime,0,m_endAmount,m_animationTime);
	float increaseAmount = m_currentAmount-m_lastAmount;
	m_camera->truck(increaseAmount);
	m_lastAmount = m_currentAmount;
}

//==============================================================================
//=============================== TARGET ANIMATION ==============================
//==============================================================================

TargetAnimation::TargetAnimation(ofPtr<ofEasyCam>  camera, EasingFunction function, EasingType type):
	CameraAnimation(camera,function,type)
{
	// intentionally left empty
}

void TargetAnimation::setParameters(const ofVec3f& endPosition, double animationTime)
{
	m_elapsedTime = 0.0;
	m_endPosition = endPosition;
	m_startPosition = m_camera->getTarget().getPosition();
	m_position = m_startPosition;
	m_animationTime = animationTime;
}


void TargetAnimation::update()
{
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
		this->finish();
		return;
	}

	float x = this->function(m_elapsedTime,m_startPosition.x,m_endPosition.x,m_animationTime);
	float y = this->function(m_elapsedTime,m_startPosition.y,m_endPosition.y,m_animationTime);
	float z = this->function(m_elapsedTime,m_startPosition.z,m_endPosition.z,m_animationTime);

	m_position.set(x, y, z);
	m_camera->setTarget(m_position);
}


//==============================================================================
//=============================== MOVING ANIMATION ==============================
//==============================================================================

MovingAnimation::MovingAnimation(ofPtr<ofEasyCam> camera, EasingFunction function, EasingType type):
	CameraAnimation(camera,function,type)
{
	// intentionally left empty
}

void MovingAnimation::setParameters(const ofVec3f& endPosition, double animationTime)
{
	m_elapsedTime = 0.0;
	m_endPosition = endPosition;
	m_startPosition = m_camera->getPosition();
	m_position = m_startPosition;
	m_animationTime = animationTime;
}


void MovingAnimation::update()
{
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
		this->finish();
		return;
	}

	float x = this->function(m_elapsedTime,m_startPosition.x,m_endPosition.x,m_animationTime);
	float y = this->function(m_elapsedTime,m_startPosition.y,m_endPosition.y,m_animationTime);
	float z = this->function(m_elapsedTime,m_startPosition.z,m_endPosition.z,m_animationTime);

	m_position.set(x, y, z);
	m_camera->setPosition(m_position);
}



/*
 *  VisualEffects.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */

#include <math.h>

#include "AppManager.h"
#include "VisualEffectsManager.h"
#include "VisualEffects.h"
#include "BasicVisual.h"

//==============================================================================
//========================= VISUAL EFFECT ==================================
//==============================================================================


VisualEffect::VisualEffect(ofPtr<BasicVisual> visual, EasingFunction function, EasingType type): m_visual(visual), m_function(function), m_isActive(false), m_animationTime(1000.0), m_elapsedTime(0.0),	m_elaspedTimeToStart(0.0), m_type(type), m_isFinished(true)
{
	// intentionally left empty
}


void VisualEffect::start(double startTime)
{
    m_elapsedTime = 0.0;
	m_elaspedTimeToStart = 0.0;
	m_isActive = true;
    m_isFinished = false;
	m_startTime = startTime;
}


void VisualEffect::stop()
{
	m_isActive = false;
}

void VisualEffect::finish()
{
    m_isFinished = true;
}

double VisualEffect::function(double t, double from, double to, double duration) const
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
//================================  FADE VISUAL ================================
//==============================================================================

FadeVisual::FadeVisual(ofPtr<BasicVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type),
    m_alpha(1.0), m_startAlpha(0.0),m_endAlpha(1.0)
{
	m_name = "FadeVisual";
}



void FadeVisual::setParameters(double startAlpha,double endAlpha, double animationTime)
{
	m_elapsedTime = 0.0;
	m_startAlpha = startAlpha;
	m_endAlpha = endAlpha;
	m_animationTime = animationTime;
}

void FadeVisual::setParameters(double endAlpha, double animationTime)
{
	m_elapsedTime = 0.0;
	m_startAlpha = m_visual->getAlpha();
	m_endAlpha = endAlpha;
	m_animationTime = animationTime;
}


void FadeVisual::update()
{
    if(!m_isActive){
        return;
    }
    
	double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
        m_visual->setAlpha(m_endAlpha);
		this->finish();
		return;
	}

	m_alpha = this->function(m_elapsedTime,m_startAlpha,m_endAlpha,m_animationTime);
	m_visual->setAlpha(m_alpha);

}

//==============================================================================
//================================  SCALE VISUAL ================================
//==============================================================================

ScaleVisual::ScaleVisual(ofPtr<BasicVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type),
m_startScale(0.0,0.0),m_endScale(1.0,1.0),m_scale(1.0,1.0)
{
	m_name = "ScaleVisual";
}



void ScaleVisual::setParameters(const ofVec3f& endScale, double animationTime)
{
	m_elapsedTime = 0.0;
	m_startScale = m_visual->getScale();
	m_endScale = endScale;
	m_animationTime = animationTime;
}

void ScaleVisual::setParameters(const ofVec3f& startScale,const ofVec3f& endScale, double animationTime)
{

    m_elapsedTime = 0.0;
	m_startScale = startScale;
	m_endScale = endScale;
	m_animationTime = animationTime;
}


void ScaleVisual::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
        m_visual->setScale(m_endScale);
		this->finish();
		return;
	}

    m_scale.x = this->function(m_elapsedTime,m_startScale.x,m_endScale.x,m_animationTime);
	m_scale.y = this->function(m_elapsedTime,m_startScale.y,m_endScale.y,m_animationTime);
    m_scale.z = this->function(m_elapsedTime,m_startScale.z,m_endScale.z,m_animationTime);
	m_visual->setScale(m_scale);
}

//==============================================================================
//================================  MOVE VISUAL ================================
//==============================================================================

MoveVisual::MoveVisual(ofPtr<BasicVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type),
m_startPos(0.0,0.0),m_endPos(1.0,1.0),m_pos(1.0,1.0)
{
	m_name = "MoveVisual";
}



void MoveVisual::setParameters(const ofVec3f& endPos, double animationTime)
{
	m_elapsedTime = 0.0;
	m_startPos = m_visual->getPosition();
	m_endPos = endPos;
	m_animationTime = animationTime;
}

void MoveVisual::setParameters(const ofVec3f& startPos,const ofVec3f& endPos, double animationTime)
{
    m_elapsedTime = 0.0;
	m_startPos = startPos;
	m_endPos = endPos;
	m_animationTime = animationTime;
}


void MoveVisual::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
        m_visual->setPosition(m_endPos);
		this->finish();
		return;
	}

    m_pos.x = this->function(m_elapsedTime,m_startPos.x,m_endPos.x,m_animationTime);
	m_pos.y = this->function(m_elapsedTime,m_startPos.y,m_endPos.y,m_animationTime);
	m_visual->setPosition(m_pos);
}



//==============================================================================
//================================  COLOR EFFECT ================================
//==============================================================================

ColorEffect::ColorEffect(ofPtr<BasicVisual> visual, EasingFunction function, EasingType type): VisualEffect(visual,function,type),
m_startColor(0,0,0),m_endColor(0,0,0),m_color(0,0,0)
{
	m_name = "ColorEffect";
}



void ColorEffect::setParameters(const ofColor& endColor, double animationTime)
{
	m_elapsedTime = 0.0;
	m_startColor = m_visual->getColor();
	m_endColor = endColor;
	m_animationTime = animationTime;
}

void ColorEffect::setParameters(const ofColor& startColor,const ofColor& endColor, double animationTime)
{
    m_elapsedTime = 0.0;
	m_startColor = startColor;
	m_endColor = endColor;
	m_animationTime = animationTime;
}


void ColorEffect::update()
{
    if(!m_isActive){
        return;
    }
    
    double dt = ofGetLastFrameTime();

    if(m_elaspedTimeToStart < m_startTime) {
        m_elaspedTimeToStart += dt;
		return;
	}

	m_elapsedTime = m_elapsedTime + dt;

	if(m_elapsedTime >= m_animationTime) {
        m_visual->setColor(m_endColor);
		this->finish();
		return;
	}

    
    
    float amount = this->function(m_elapsedTime,0.0,1.0,m_animationTime);
    m_color = m_startColor.lerp( m_endColor , amount );
    m_visual->setColor(m_color);
}

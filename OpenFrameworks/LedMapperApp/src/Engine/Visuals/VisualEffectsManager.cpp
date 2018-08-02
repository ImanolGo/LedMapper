/*
 *  VisualEffectsManager.cpp
 *
 *  Created by Imanol GÑmez on 16/02/15.
 *
 */

#include "VisualEffectsManager.h"

VisualEffectsManager::VisualEffectsManager(): Manager()
{
    // intentionally left empty
}


VisualEffectsManager::~VisualEffectsManager()
{
    ofLogNotice() <<"VisualEffectsManager::destructor";
}


void VisualEffectsManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"VisualEffectsManager::initialized";
}

void VisualEffectsManager::update()
{
    for(VisualEffectVector::iterator it = m_visualEffects.begin(); it != m_visualEffects.end();)
    {
        (*it)->update();
        
        
        if((*it)->isFinished()) {
            it = m_visualEffects.erase(it);
        }
        else{
            ++it;
        }
    }
}

void VisualEffectsManager::addVisualEffect(ofPtr<VisualEffect> visualEffect)
{
    if(!visualEffect)
        return;
    
    for(VisualEffectVector::iterator it = m_visualEffects.begin(); it != m_visualEffects.end();it++) {
        if(*it == visualEffect) {
            return;
        }
    }
    m_visualEffects.push_back(visualEffect);
}

void VisualEffectsManager::removeVisualEffect(ofPtr<VisualEffect> visualEffect)
{
    if(!visualEffect)
        return;
    
    for(VisualEffectVector::iterator it = m_visualEffects.begin(); it != m_visualEffects.end();)
    {
        if(*it == visualEffect) {
            it = m_visualEffects.erase(it);
        }
        else{
            ++it;
        }
    }
}

void VisualEffectsManager::removeAllVisualEffects(ofPtr<BasicVisual> visual)
{
    for(VisualEffectVector::iterator it = m_visualEffects.begin(); it != m_visualEffects.end();) {
        if((*it)->getVisual() == visual) {
            it = m_visualEffects.erase(it);
        }
        else {
            ++it;
        }
    }
}

void VisualEffectsManager::removeVisualEffects(ofPtr<BasicVisual> visual, const string& visualEffectName)
{
    for(VisualEffectVector::iterator it = m_visualEffects.begin(); it != m_visualEffects.end();) {
        if((*it)->getVisual() == visual && (*it)->getName() == visualEffectName) {
            it = m_visualEffects.erase(it);
        }
        else {
            ++it;
        }
    }
}

void VisualEffectsManager::createFadeEffect(ofPtr<BasicVisual> visual, double startAlpha,double endAlpha, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<FadeVisual> fadeVisual = ofPtr<FadeVisual>(new FadeVisual(visual,settings.function, settings.type));
    fadeVisual->setParameters(startAlpha,endAlpha,settings.animationTime);
    fadeVisual->start(settings.startAnimation);
    this->addVisualEffect(fadeVisual);
}

void VisualEffectsManager::createFadeEffect(ofPtr<BasicVisual> visual, double endAlpha, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<FadeVisual> fadeVisual = ofPtr<FadeVisual>(new FadeVisual(visual,settings.function, settings.type));
    fadeVisual->setParameters(endAlpha,settings.animationTime);
    fadeVisual->start(settings.startAnimation);
    this->addVisualEffect(fadeVisual);
}

void VisualEffectsManager::createScaleEffect(ofPtr<BasicVisual> visual, const ofVec2f& startScale,const ofVec2f& endScale, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<ScaleVisual> scaleVisual = ofPtr<ScaleVisual>(new ScaleVisual(visual,settings.function, settings.type));
    scaleVisual->setParameters(startScale,endScale,settings.animationTime);
    scaleVisual->start(settings.startAnimation);
    this->addVisualEffect(scaleVisual);
}

void VisualEffectsManager::createScaleEffect(ofPtr<BasicVisual> visual, const ofVec2f& endScale, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<ScaleVisual> scaleVisual = ofPtr<ScaleVisual>(new ScaleVisual(visual, settings.function, settings.type));
    scaleVisual->setParameters(endScale,settings.animationTime);
    scaleVisual->start(settings.startAnimation);
    this->addVisualEffect(scaleVisual);
}

void VisualEffectsManager::popUpAnimation(ofPtr<BasicVisual> visual, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofVec3f startScale(0,0,0);
    ofVec3f endScale(1,1,1);
    
    ofPtr<ScaleVisual> scaleVisual = ofPtr<ScaleVisual>(new ScaleVisual(visual, settings.function, EASE_IN));
    scaleVisual->setParameters(startScale,endScale*1.1,settings.animationTime*0.5);
    scaleVisual->start(settings.startAnimation);
    this->addVisualEffect(scaleVisual);
    
    scaleVisual = ofPtr<ScaleVisual>(new ScaleVisual(visual, settings.function, EASE_OUT));
    scaleVisual->setParameters(endScale*1.1,endScale,settings.animationTime*0.5);
    scaleVisual->start(settings.startAnimation+settings.animationTime*0.5);
    this->addVisualEffect(scaleVisual);
}

void VisualEffectsManager::createMoveEffect(ofPtr<BasicVisual> visual, const ofVec3f& startPos,const ofVec3f& endPos, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<MoveVisual> moveVisual = ofPtr<MoveVisual>(new MoveVisual(visual, settings.function, settings.type));
    moveVisual->setParameters(startPos,endPos,settings.animationTime);
    moveVisual->start(settings.startAnimation);
    this->addVisualEffect(moveVisual);
}

void VisualEffectsManager::createMoveEffect(ofPtr<BasicVisual> visual, const ofVec3f& endPos, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<MoveVisual> moveVisual = ofPtr<MoveVisual>(new MoveVisual(visual, settings.function, settings.type));
    moveVisual->setParameters(endPos,settings.animationTime);
    moveVisual->start(settings.startAnimation);
    this->addVisualEffect(moveVisual);
}


void VisualEffectsManager::createColorEffect(ofPtr<BasicVisual> visual,const ofColor& startColor,const ofColor& endColor, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<ColorEffect> colorEffect = ofPtr<ColorEffect>(new ColorEffect(visual, settings.function, settings.type));
    colorEffect->setParameters(startColor,endColor,settings.animationTime);
    colorEffect->start(settings.startAnimation);
    this->addVisualEffect(colorEffect);
}

void VisualEffectsManager::createColorEffect(ofPtr<BasicVisual> visual, const ofColor& endColor, EffectSettings& settings)
{
    if(!visual)
        return;
    
    ofPtr<ColorEffect> colorEffect = ofPtr<ColorEffect>(new ColorEffect(visual, settings.function, settings.type));
    colorEffect->setParameters(endColor,settings.animationTime);
    colorEffect->start(settings.startAnimation);
    this->addVisualEffect(colorEffect);
}








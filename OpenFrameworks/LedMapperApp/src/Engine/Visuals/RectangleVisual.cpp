/*
 *  RectangleVisual.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "AppManager.h"
#include "ResourceManager.h"

#include "RectangleVisual.h"


RectangleVisual::RectangleVisual(): BasicVisual(), m_centred(false)
{
    //Intentionally left empty
}


RectangleVisual::RectangleVisual(const ofVec3f& pos, float width, float height, bool centred):
    BasicVisual(pos, width, height),  m_centred(centred)
{
    //Intentionally left empty
}

RectangleVisual::~RectangleVisual()
{
    //Intentionally left empty
}


void RectangleVisual::draw()
{

    ofPushMatrix();
    ofPushStyle();

        ofTranslate(m_position);

        ofScale(m_scale.x, m_scale.y);

        if(m_centred){
            ofTranslate(-m_width*0.5,-m_height*0.5);
        }

        ofRotateX(m_rotation.x);
        ofRotateY(m_rotation.y);


        ofSetColor(m_color);
    
        ofRect(0, 0, m_width, m_height);

    ofPopStyle();
    ofPopMatrix();
}



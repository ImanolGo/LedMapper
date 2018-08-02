/*
 *  VideoVisual.cpp
 *
 *  Created by Imanol Gomez on 27/08/15.
 *
 */


#include "AppManager.h"
#include "ResourceManager.h"

#include "VideoVisual.h"


VideoVisual::VideoVisual(): BasicVisual(), m_centred(false)
{
    //Intentionally left empty
}


VideoVisual::VideoVisual(const ofVec3f& pos, const string& resourceName,bool centred):
    BasicVisual(pos, 0, 0), m_centred(centred)
{
    this->setResource(resourceName);
}

VideoVisual::~VideoVisual()
{
    //Intentionally left empty
}

bool VideoVisual::setResource(const string& resourceName)
{
    if( m_video.loadMovie(resourceName)){
        m_width = m_originalWidth = m_video.getWidth();
        m_height = m_originalHeight = m_video.getWidth();
        return true;
    }

    return false;
}

void VideoVisual::update()
{
    m_video.update();
}

void VideoVisual::draw()
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
        m_video.draw(0,0, m_width, m_height);

    ofPopStyle();
    ofPopMatrix();
}

void VideoVisual::setWidth(float width, bool keepRatio)
{
    m_width = width;
    if(keepRatio){
        float ratio = m_originalWidth/m_originalHeight;
        m_height = m_width/ratio;
    }
}

void VideoVisual::setHeight(float height, bool keepRatio)
{
    m_height = height;
    if(keepRatio){
        float ratio = m_originalWidth/m_originalHeight;
        m_width = m_height*ratio;
    }
}

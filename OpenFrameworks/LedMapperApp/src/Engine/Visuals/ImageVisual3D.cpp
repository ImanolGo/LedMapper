/*
 *  ImageVisual3D.cpp
 *
 *  Created by Imanol Gómez on 17/02/15.
 *
 */

#include "ImageVisual3D.h"

ImageVisual3D::ImageVisual3D(const ofVec3f& pos, const string& resourceName):
    ImageVisual(pos, resourceName, true)
{
    m_plane = ofPtr<ofPlanePrimitive>(new ofPlanePrimitive());
}

ImageVisual3D::~ImageVisual3D()
{
    //Intentionally left empty
}


void ImageVisual3D::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
        ofTranslate(m_position);
        ofRotateX(m_rotation.x);
        ofRotateY(m_rotation.y);
        ofRotateZ(m_rotation.z);
        ofScale(m_scale.x, m_scale.y, m_scale.z);

        m_plane->set(m_width, m_height);

        m_texture->bind();
            ofFill();
            ofSetColor(m_color);
            m_plane->draw();
        m_texture->unbind();

    ofPopStyle();
    ofPopMatrix();
}

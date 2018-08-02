/*
 *  TextVisual.cpp
 *
 *  Created by Imanol G—mez on 16/02/15.
 *
 */


#include "TextVisual.h"

TextVisual::TextVisual(ofVec3f pos, float width, float height, bool centred): BasicVisual(pos, width, height),m_fontSize(0), m_centred(centred), m_drawBB(false),m_lineHeight(1.0)
{
    //Intentionally left empty
}

TextVisual::TextVisual(): BasicVisual(),m_fontSize(0), m_centred(false), m_drawBB(false), m_lineHeight(1.0)
{
    //Intentionally left empty
}

TextVisual::~TextVisual()
{
    //Intentionally left empty
}

void TextVisual::setText(const std::string& text, const std::string& fontName, float fontSize, ofColor color)
{
    //    m_text = text;
    //    m_fontSize = fontSize;
    //
    //    m_font.setup(fontName);
    //
    //    if(m_centred){
    //        m_font.setTextBlockAlignment(ofxFontStash::OF_TEXT_ALIGN_CENTER);
    //    }
    //
    //    //m_box =  m_font.getBBox(m_text,m_fontSize,m_position.x,m_position.y);
    //    m_box = m_font.drawMultiLineColumn(m_text,m_fontSize,m_position.x,m_position.y,m_width, false);
    //    m_color = color;
    //    m_translation = m_position - m_box.getPosition();
    //
    //    if(m_centred){
    //       m_translation.y -= m_box.getHeight()*0.5;
    //    }
    //
    
    m_text = text;
    m_fontSize = fontSize;
    m_fontName = fontName;
    
    m_font.setup(fontName,m_fontSize);
    m_font.setLineHeight(m_lineHeight);
    
    if(m_centred){
        m_font.setTextBlockAlignment(EngineFont::OF_TEXT_ALIGN_CENTER);
    }
    
    m_box =  m_font.drawMultiLineColumn(m_text,m_position.x,m_position.y,m_width);
    
    m_color = color;
    m_translation = m_position - m_box.getPosition();
    
    if(m_centred){
        m_translation.y -= m_box.getHeight()*0.5;
    }
    
}

void TextVisual::setText(const std::string& text)
{
    //    m_text = text;
    //
    //    m_box = m_font.drawMultiLineColumn(m_text,m_fontSize,m_position.x,m_position.y,m_width, false);
    //    //m_box =  m_font.getBBox(m_text,m_fontSize,m_position.x,m_position.y);
    //
    //    m_translation = m_position - m_box.getPosition();
    //    if(m_centred){
    //        m_translation.y -= m_box.getHeight()*0.5;
    //    }
    
    m_text = text;
    
    m_box =  m_font.drawMultiLineColumn(m_text,m_position.x,m_position.y,m_width);
    
    m_translation = m_position - m_box.getPosition();
    if(m_centred){
        m_translation.y -= m_box.getHeight()*0.5;
    }
    
}

void  TextVisual::setWidth(float width)
{
    m_width = width;
    this->setText(m_text);
}

void  TextVisual::setFontSize(int value)
{
    m_fontSize = value;
    m_font.setup(m_fontName,m_fontSize);
    m_font.setLineHeight(m_lineHeight);
    this->setText(m_text);
}

void  TextVisual::setFontName(const string& name)
{
    m_fontName = name;
    m_font.setup(m_fontName,m_fontSize);
    m_font.setLineHeight(m_lineHeight);
    this->setText(m_text);
}

void  TextVisual::setLineHeight(float lineHeight)
{
    m_lineHeight= lineHeight;
    m_font.setLineHeight(m_lineHeight);
    this->setText(m_text);
}

void TextVisual::draw()
{
    ofPushMatrix();
    ofPushStyle();
    //ofEnableAlphaBlending();
    
    ofScale(m_scale.x, m_scale.y);
    
    //ofSetColor(ofColor(255,255,10,200));
    //ofCircle(m_position, 3);
    //ofSetColor(ofColor(255,10,10,100));
    //if(m_centred){
    //ofRect(m_position.x - m_box.width*0.5, m_position.y - m_box.height*0.5, m_box.width, m_box.height);
    //}
    //else{
    //ofRect(m_position.x, m_position.y, m_box.width, m_box.height);
    //}
    
    
    ofSetColor(m_color);
    
    ofPushMatrix();
    ofTranslate(m_position.x + m_box.height*0.5, m_position.y + m_box.height*0.5, 0);//move pivot to centre
    ofRotateX(m_rotation.x);
    ofRotateY(m_rotation.y);
    ofRotateZ(m_rotation.z);//rotate from centre
    //ofRotate(m_rotation.x, 0, 0, 1);//rotate from centre
    ofPushMatrix();
    ofTranslate( - m_position.x -m_box.height*0.5, -m_position.y-m_box.height*0.5, 0);//move back by the centre offset
    
    if(m_drawBB){
        ofNoFill();
        if(m_centred){
            ofDrawRectangle(m_position.x - m_box.width*0.5, m_position.y - m_box.height*0.5, m_box.width, m_box.height);
            
        }
        else{
            ofDrawRectangle(m_position.x, m_position.y, m_box.width, m_box.height);
        }
    }
    
    ofTranslate(m_translation.x, m_translation.y);
    m_font.drawMultiLineColumn(m_text,m_position.x,m_position.y,m_width);
    ofPopMatrix();
    ofPopMatrix();
    
    //ofTranslate(m_translation.x, m_translation.y);
    
    
    
    //m_font.drawMultiLineColumn(m_text,m_fontSize, m_position.x,m_position.y,m_width);
    //m_font.drawMultiLineColumn(m_text,m_position.x,m_position.y,m_width);
    
    
    //ofDisableAlphaBlending();
    
    ofPopStyle();
    ofPopMatrix();   // recall the pushed style
}


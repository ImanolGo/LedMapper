/*
 *  EngineFont.h
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */


#include <iostream>
#include "EngineFont.h"

/* *********************************************************************** */

EngineFont::EngineFont():m_lineHeight(1.0),m_batchDrawing(false),m_textBlockAlignment(OF_TEXT_ALIGN_LEFT)
{
	//Intentionally left empty
}

EngineFont::~EngineFont()
{
	//Intentionally left empty
}

bool EngineFont::setup( string fontFile, float fontSize, float lineHeightPercent)
{

    //m_trueTypeFont.setEncoding(OF_ENCODING_UTF8);

	m_lineHeight = lineHeightPercent;
    m_fontSize = fontSize;
    m_textSuite.init(fontFile,fontSize);
    return true;

//    bool bAntiAliased=true;
//    bool bFullCharacterSet=true;
//    bool makeContours=false;
//    float simplifyAmt=0.3;
//    int dpi=0.0;
//
//    if(m_trueTypeFont.load(fontFile,fontSize,bAntiAliased,bFullCharacterSet,makeContours,simplifyAmt,dpi)){
//        //ofLogNotice() << "EngineFont::setup-> font loaded " << fontFile << " with size " << fontSize;
//        return true;
//    }
//    else{
//         ofLogNotice() << "EngineFont::setup-> Can't load font " << fontFile << "!!";
//        return false;
//    }
//
//    return false;
}


void EngineFont::draw( string text, float x, float y)
{
    m_textSuite.setText(text);
    m_textSuite.draw(x,y);
}

void EngineFont::drawMultiLine( string text, float x, float y)
{
    stringstream ss(text);
    string s;
    int line = 0;
    float yy = y;

    while ( getline(ss, s, '\n') ) {
        //cout << s << endl;
        yy = y + m_fontSize * m_lineHeight* FONT_HEIGHT_MULT * line;
        m_textSuite.setText(s.c_str());
        m_textSuite.draw(x,yy);
        line ++;
    }
}

ofRectangle EngineFont::drawMultiLineColumn( string text, float x, float y, float columnWidth)
{

    ofRectangle totalArea = ofRectangle(x,y,0,0);
    m_textSuite.wrapTextX(columnWidth);
    
    
    if(m_textBlockAlignment == OF_TEXT_ALIGN_LEFT)
    {
        m_textSuite.draw(x,y);
    }
    
    else if(m_textBlockAlignment == OF_TEXT_ALIGN_CENTER)
    {
        m_textSuite.drawCenter(x,y);
    }
    else if(m_textBlockAlignment == OF_TEXT_ALIGN_RIGHT)
    {
        m_textSuite.drawRight(x,y);
    }
    
    

    totalArea.width = m_textSuite.getWidth();  totalArea.height = m_textSuite.getHeight();

    return totalArea;
}


void EngineFont::setLineHeight(float percent){
	m_lineHeight = percent;
    m_textSuite.setLineHeight(m_lineHeight);
}

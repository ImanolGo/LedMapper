/*
 *  EngineFont.cpp
 *
 *  Created by Imanol Gomez on 23/02/15.
 *
 */


#pragma once

#define FONT_HEIGHT_MULT  0.8

#include "ofMain.h"
#include "ofxTextSuite.h"


class EngineFont{

	public:

        enum TextBlockAlignment { OF_TEXT_ALIGN_LEFT, OF_TEXT_ALIGN_RIGHT, OF_TEXT_ALIGN_CENTER };

		EngineFont();
		~EngineFont();

		//call this to set your font file (.ttf, etc)
		bool setup( string fontFile, float fontSize, float lineHeightPercent = 1.0f);

		//will draw text in one line, ignoring "\n"'s
		void draw( string text, float x, float y);

		//text with "\n" will produce line breaks
		void drawMultiLine( string text, float x, float y);

		//fits text in a column of a certain width
		//if you only want to find out the bbox size, send in dontDraw=true
		ofRectangle drawMultiLineColumn( string text, float x, float y, float columnWidth );

        // works only when using drauMultilineColumn for now
		void setTextBlockAlignment(TextBlockAlignment textBlockAlignment){ m_textBlockAlignment = textBlockAlignment; }

		void setLineHeight(float percent);

    private:

        //fill in a string
		//string walkAndFill(ofUTF8Ptr being, ofUTF8Ptr & iter, ofUTF8Ptr end);

	private:

        ofxTextBlock		m_textSuite;
		TextBlockAlignment  m_textBlockAlignment;

		float				m_lineHeight; // as percent, 1.0 would be normal
		bool				m_batchDrawing;
        float               m_fontSize;

};

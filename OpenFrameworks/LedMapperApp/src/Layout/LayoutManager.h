/*
 *  LayoutManager.h
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */




#pragma once

#include "Manager.h"
#include "TextVisual.h"
#include "ImageVisual.h"
#include "RectangleVisual.h"



//========================== class LayoutManager ==============================
//============================================================================
/** \class LayoutManager LayoutManager.h
 *	\brief Class managing the layout of the application
 *	\details It creates an places al the text and elements regarding the layout
 */

enum ScreenMode{
    DRAW_NORMAL = 0,
    DRAW_VIDEO,
    DRAW_LEDS
};

class LayoutManager: public Manager
{
    
public:
    
    static const int MARGIN;
    static const int FRAME_MARGIN;
    static const string LAYOUT_FONT;
    static const string LAYOUT_FONT_LIGHT;

    //! Constructor
    LayoutManager();

    //! Destructor
    ~LayoutManager();

    //! Set-up the layout
    void setup();
    
    //! Update the layout
    void update();
    
    //! Draw the layout
    void draw();
    
    void onFullScreenChange(bool value);
    
    void windowResized(int w, int h);
    
    ofPtr<ofRectangle> getWindowRect(string& name) {return m_windowRects[name];}
    
    void begin(string& name);
    
    void end(string& name);
    
    void toggleDrawMode(int mode) {m_drawMode = mode;}
    
    void toggleDrawMode();
    
    int getDrawMode() {return m_drawMode;}
    

private:


    //! Create the text visuals
    void createTextVisuals();

    //! Create the image visuals
    void createImageVisuals();

    //! Create the background Image
    void createBackground();


    //! Add all visuals as overlays
    void addVisuals();

    void setupFbos();
    
    void resetWindowRects();
    
    void resetWindowFrames();
    
    void resetWindowTitles();
    
    void setupWindowFrames();
    
    void updateFbos();
    
    void updateVideoFbo();
    
    void updateLedsFbo();
    
    void drawFbos();
    
    void drawRectangles();
    
    void drawText();
    
    void drawLeds();
    
    void drawNormal();
    
    void drawVideo();

    
private:


    typedef  map<string, ofPtr<TextVisual> >      TextMap;            ///< defines a map of Text attached to an identifier
    typedef  map<string, ofPtr<ImageVisual>  >    ImageMap;           ///< defines a map of ImageVisual Map attached to an identifier
    
    typedef  map<string, ofPtr<ofFbo>  >    FboMap;           ///< defines a map of Fbos attached to an identifier
    typedef  map<string, ofPtr<ofRectangle>  >    FboRectangles;           ///< defines a map of ofRectangle attached to an identifier
    typedef  map<string, ofPtr<RectangleVisual>  >    FboFrames;           ///< defines a map of RectangleVisual attached to an identifier
  
    TextMap             m_textVisuals;             ///< map storing the text visuals attached to a name
    ImageMap            m_imageVisuals;            ///< map storing the image visuals attached to a name

    FboRectangles       m_windowRects;
    FboFrames           m_windowFrames;
    FboMap              m_fbos;
    
    int                 m_drawMode;

};

//==========================================================================



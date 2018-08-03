//
//  ofxPSLevels.h
//  ofxPSLevelsExample
//
//  Created by Chris Mullany on 21/05/2013.
//  me@chrismullany.com
//

#pragma once
#include "ofMain.h"

class ofxPSLevels {
public:
    ofxPSLevels();
    
    void setup();
    void setup(int w, int h);
    void begin();
    void end();
    void draw();
    
    void resize(int w, int h);
    
    // shader
    ofShader shader;
    
    // shader params
    float contrast;
    float saturation;
    float brightness;
    float gamma;
    float minInput;
    float maxInput;
    float minOutput;
    float maxOutput;
    
private:
    ofFbo fbo;
    int width, height;
    bool success;
    
};

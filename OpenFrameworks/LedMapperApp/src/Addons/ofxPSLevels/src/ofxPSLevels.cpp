//
//  ofxPSLevels.cpp
//  ofxPSLevelsExample
//
//  Created by Chris Mullany on 21/05/2013.
//  me@chrismullany.com
//

#include "ofxPSLevels.h"

//////////////////////////////////////////////////////////////////////////////////
// public
//////////////////////////////////////////////////////////////////////////////////

ofxPSLevels::ofxPSLevels() {
    // defaults
    contrast = 1.0;
    saturation = 1.0;
    brightness = 1.0;
    gamma = 1.0;
    minInput = 0.0;
    maxInput = 1.0;
    minOutput = 0.0;
    maxOutput = 1.0;
}

void ofxPSLevels::setup() {
    setup(ofGetWidth(), ofGetHeight());
}

void ofxPSLevels::setup(int w, int h){
    width = w;
    height = h;
    // allocate and clear fbo
    fbo.allocate(width, height);
    begin(); ofClear(0); end();
    // a shader handles all level adjustment
    success = shader.load("shaders/ps/levels.vert", "shaders/ps/levels.frag");
}

void ofxPSLevels::draw() {
    if (!success) {
        ofLogError() << "Shader didn't load correctly. Cannot draw in ofxPSLevels::draw()";
        return;
    }
    glPushMatrix();
    shader.begin();
    glActiveTexture(GL_TEXTURE0);
    fbo.getTextureReference().bind();
    shader.setUniform1i("tex0", int(GLuint(0)));
    // csb params
    shader.setUniform1f("contrast", contrast);
    shader.setUniform1f("saturation", saturation);
    shader.setUniform1f("brightness", brightness);
    // levels params
    shader.setUniform1f("gamma", gamma);
    shader.setUniform1f("minInput", minInput);
    shader.setUniform1f("maxInput", maxInput);
    shader.setUniform1f("minOutput", minOutput);
    shader.setUniform1f("maxOutput", maxOutput);
    // draw quad
    glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0f, height); glVertex3f(0, height, 0);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f); glVertex3f(0, 0, 0);
    glMultiTexCoord2f(GL_TEXTURE0, width, 0.0f); glVertex3f(width, 0, 0);
    glMultiTexCoord2f(GL_TEXTURE0, width, height); glVertex3f(width, height, 0);
    glEnd();
    // end shader
    shader.end();
    fbo.getTextureReference().unbind();
    glPopMatrix();
}

void ofxPSLevels::begin() {
    fbo.begin();
}

void ofxPSLevels::end() {
    fbo.end();
}

void ofxPSLevels::resize(int w, int h) {
    ofLogWarning() << "ofxPSLevels::resize not implemented";
}

//////////////////////////////////////////////////////////////////////////////////
// private
//////////////////////////////////////////////////////////////////////////////////


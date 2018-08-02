//
//  ofxPSLevels.cpp
//  ofxPSLevelsExample
//
//  Created by Chris Mullany on 21/05/2013.
//  me@chrismullany.com
//

#include "ofxPSLevels.h"

#define STRINGIFY(A) #A

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
    
    ofShaderSettings settings;
    settings.shaderSources[GL_VERTEX_SHADER] = R"(
    void main() {
        gl_TexCoord[0] = gl_MultiTexCoord0;
        gl_Position = ftransform();
    }
)";
    
    settings.shaderSources[GL_FRAGMENT_SHADER] = R"(
    uniform sampler2DRect inputTexture;
    
    uniform float contrast;
    uniform float saturation;
    uniform float brightness;
    
    uniform float gamma;
    uniform float minInput;
    uniform float maxInput;
    uniform float minOutput;
    uniform float maxOutput;
    
    
    /*
     ** Gamma correction
     ** Details: http://blog.mouaif.org/2009/01/22/photoshop-gamma-correction-shader/
     */
#define GammaCorrection(color, gamma)                               vec3( pow(color.r, 1.0 / gamma), pow(color.g, 1.0 / gamma), pow(color.b, 1.0 / gamma) )
    /*
     ** Levels control (input (+gamma), output)
     ** Details: http://blog.mouaif.org/2009/01/28/levels-control-shader/
     */
#define LevelsControlInputRange(color, minInput, maxInput)              min(max(color - vec3(minInput), vec3(0.0)) / (vec3(maxInput) - vec3(minInput)), vec3(1.0))
#define LevelsControlInput(color, minInput, gamma, maxInput)                GammaCorrection(LevelsControlInputRange(color, minInput, maxInput), gamma)
#define LevelsControlOutputRange(color, minOutput, maxOutput)           mix(vec3(minOutput), vec3(maxOutput), color)
#define LevelsControl(color, minInput, gamma, maxInput, minOutput, maxOutput)   LevelsControlOutputRange(LevelsControlInput(color, minInput, gamma, maxInput), minOutput, maxOutput)
    
    /*
     ** Contrast, saturation, brightness
     ** Code of this function is from TGM's shader pack
     ** http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=21057
     */
    // For all settings: 1.0 = 100% 0.5=50% 1.5 = 150%
    vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
    {
        // Increase or decrease theese values to adjust r, g and b color channels seperately
        const float AvgLumR = 0.5;
        const float AvgLumG = 0.5;
        const float AvgLumB = 0.5;
        
        const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
        
        vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
        vec3 brtColor = color * brt;
        vec3 intensity = vec3(dot(brtColor, LumCoeff));
        vec3 satColor = mix(intensity, brtColor, sat);
        vec3 conColor = mix(AvgLumin, satColor, con);
        return conColor;
    }
    
    
    void main ()
    {
        // Sample the input pixel
        vec4 colorIn = texture2DRect (inputTexture, gl_TexCoord [ 0 ].xy);
        vec4 colorOut;
        // csb first
        colorOut.rgb = ContrastSaturationBrightness(colorIn.rgb, brightness, saturation, contrast);
        // levels
        colorOut.rgb = LevelsControl(colorOut.rgb, minInput, gamma, maxInput, minOutput, maxOutput);
        colorOut.a = 1.0;
        // Save the result
        gl_FragColor = colorOut;
    }
    )";
    success = shader.setup(settings);
    
    
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


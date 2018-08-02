/*
 *  LedMapperApp.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "AppManager.h"

#include "LedMapperApp.h"

//--------------------------------------------------------------
void LedMapperApp::setup(){
    AppManager::getInstance().setup();
}

//--------------------------------------------------------------
void LedMapperApp::update(){
    AppManager::getInstance().update();
}

//--------------------------------------------------------------
void LedMapperApp::draw(){
    AppManager::getInstance().draw();
}

void LedMapperApp::exit()
{
    ofLogNotice() <<"LedMapperApp::exit";

}

//--------------------------------------------------------------
void LedMapperApp::keyPressed(int key){

}

//--------------------------------------------------------------
void LedMapperApp::keyReleased(int key){

}

//--------------------------------------------------------------
void LedMapperApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void LedMapperApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void LedMapperApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void LedMapperApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void LedMapperApp::windowResized(int w, int h){
    AppManager::getInstance().getLayoutManager().windowResized(w,h);


}

//--------------------------------------------------------------
void LedMapperApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void LedMapperApp::dragEvent(ofDragInfo dragInfo){

}

/*
 *  KeyboardManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */


#include "KeyboardManager.h"
#include "AppManager.h"


KeyboardManager::KeyboardManager(): Manager()
{
    //Intentionally left empty
}

KeyboardManager::~KeyboardManager()
{
   ofLogNotice() << "KeyboardManager::destructor";
}


//--------------------------------------------------------------

void KeyboardManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    ofLogNotice() <<"KeyboardManager::initialized" ;
    this->addListeners();
}

void KeyboardManager::addListeners()
{
    ofRegisterKeyEvents(this); // this will enable the InteractiveVisual class to listen to the key events.
}

void KeyboardManager::keyPressed(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogVerbose() <<"KeyboardManager::keyPressed-> " + ofToString(key);
    
  
    if(key == 'g' || key == 'G') {
        AppManager::getInstance().getGuiManager().toggleGui();
    }
    
    if(key == 'f' || key == 'F') {
        AppManager::getInstance().getLayoutManager().toggleDrawMode(1);
    }
    
    
    if(key == 'p' || key == 'P') {
        AppManager::getInstance().getLayoutManager().toggleDrawMode(2);
    }
    
    if(key == 'n' || key == 'N') {
        AppManager::getInstance().getLayoutManager().toggleDrawMode(0);
    }
    
    if(key == ' ') {
        AppManager::getInstance().getLayoutManager().toggleDrawMode();
        if(AppManager::getInstance().getLayoutManager().getDrawMode() == 0){
             AppManager::getInstance().getGuiManager().showGui(true);
        }
        else{
            AppManager::getInstance().getGuiManager().showGui(false);
        }
    }
    
}

void KeyboardManager::keyReleased(ofKeyEventArgs &e)
{
    int key = e.key;
    
    //ofLogVerbose() <<"KeyboardManager::keyReleased-> " + ofToString(key);
    
}











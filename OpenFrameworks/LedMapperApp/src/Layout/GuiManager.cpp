/*
 *  GuiManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */


#include "ofMain.h"

#include "AppManager.h"

#include "GuiManager.h"
#include "GuiTheme.h"


const string GuiManager::GUI_SETTINGS_FILE_NAME = "xmls/GuiSettings.xml";
const string GuiManager::GUI_SETTINGS_NAME = "GUI";
//const int GuiManager::GUI_WIDTH = 350;


GuiManager::GuiManager(): Manager(), m_showGui(true)
{
    //Intentionally left empty
}


GuiManager::~GuiManager()
{
    //this->saveGuiValues();
    ofLogNotice() <<"GuiManager::Destructor";
}


void GuiManager::setup()
{
    if(m_initialized)
        return;
    
    Manager::setup();
    
    
    this->setupGuiParameters();
    this->setupFileGui();
    this->setupLevelsGui();
    this->setupLedsGui();

    this->setupGuiEvents();
    this->loadGuiValues();

    
    ofLogNotice() <<"GuiManager::initialized";
    
}

void GuiManager::setupGuiParameters()
{
    
    ofxDatGuiLog::quiet();
    
    //m_gui.setPosition(ofxDatGuiAnchor::TOP_LEFT);
    m_gui.setPosition(0,0);
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath(ofToDataPath("fonts/"));
    //m_gui.setAssetPath("../Resources/data/fonts/");
    m_gui.setTheme(new GuiTheme());
    
    
    int margin =  LayoutManager::MARGIN;
    m_gui.setAutoDraw(false);
    auto pos = m_gui.getPosition();
    m_gui.setPosition(pos.x + margin, pos.y + margin);
    m_gui.addHeader(GUI_SETTINGS_NAME, false);
    
    m_gui.addFRM(0.1);
    
    m_gui.addBreak();
}

void GuiManager::setupFileGui()
{
    auto videoManager = &AppManager::getInstance().getVideoManager();
    auto ledsManager = &AppManager::getInstance().getLedsManager();
    
    
    m_modelPath.set("ModelPath", " ");
    m_modelPath.addListener(ledsManager, &LedsManager::load);
    m_parameters.add(m_modelPath);
    
    m_videoPath.set("VideoPath", " ");
    m_videoPath.addListener(videoManager, &VideoManager::load);
    m_parameters.add(m_videoPath);
    
    ofxDatGuiFolder* folder = m_gui.addFolder("FILE", ofColor::pink);
    folder->addButton("Import Project");
    folder->addButton("Save Project");
    folder->addButton("Save Project As");
    folder->addButton("Load Model");
    folder->addButton("Load Video");
    folder->addButton("Load Test");
    folder->addButton("Export");
    folder->expand();
    
}


void GuiManager::setupLevelsGui()
{
    auto videoManager = &AppManager::getInstance().getVideoManager();
   
    m_contrast.set("Contrast", 1.0, 0.0, 2.0);
    m_contrast.addListener(videoManager, &VideoManager::setContrast);
    m_parameters.add(m_contrast);
    
    m_saturation.set("Saturation", 1.0, 0.0, 2.0);
    m_saturation.addListener(videoManager, &VideoManager::setSaturation);
    m_parameters.add(m_saturation);
    
    m_brightness.set("Brightness", 1.0, 0.0, 2.0);
    m_brightness.addListener(videoManager, &VideoManager::setBrightness);
    m_parameters.add(m_brightness);
    
    m_gamma.set("Gamma", 1.0, 0.0, 2.0);
    m_gamma.addListener(videoManager, &VideoManager::setGamma);
    m_parameters.add(m_gamma);
    
    m_blur.set("Blur", 0.0, 0.0, 10.0);
    m_blur.addListener(videoManager, &VideoManager::setBlurScale);
    m_parameters.add(m_blur);
    
    m_minInput.set("MinInput", 0.0, 0.0, 1.0);
    m_minInput.addListener(videoManager, &VideoManager::setMinInput);
    m_parameters.add(m_minInput);
    
    m_maxInput.set("MaxInput", 1.0, 0.0, 1.0);
    m_maxInput.addListener(videoManager, &VideoManager::setMaxInput);
    m_parameters.add(m_maxInput);
    
    m_minOutput.set("MinOutput", 0.0, 0.0, 1.0);
    m_minOutput.addListener(videoManager, &VideoManager::setMinOutput);
    m_parameters.add(m_minOutput);
    
    m_maxOutput.set("MaxOutput", 1.0, 0.0, 1.0);
    m_maxOutput.addListener(videoManager, &VideoManager::setMaxOutput);
    m_parameters.add(m_maxOutput);
    
    m_videoSpeed.set("Speed", 1.0, 0.0, 2.0);
    m_videoSpeed.addListener(videoManager, &VideoManager::setSpeed);
    m_parameters.add(m_videoSpeed);
    
    
    
    ofxDatGuiFolder* folder = m_gui.addFolder("GENERAL", ofColor::purple);
    folder->addSlider(m_videoSpeed);
    folder->addSlider(m_contrast);
    folder->addSlider(m_saturation);
    folder->addSlider(m_brightness);
    folder->addSlider(m_gamma);
    folder->addSlider(m_blur);
    folder->addSlider(m_minInput);
    folder->addSlider(m_maxInput);
    folder->addSlider(m_minOutput);
    folder->addSlider(m_maxOutput);
    folder->expand();

    m_gui.addBreak();
}

void GuiManager::setupLedsGui()
{
    auto ledsManager = &AppManager::getInstance().getLedsManager();
    auto udpManager = &AppManager::getInstance().getUdpManager();
    
    m_ledsBrightness.set("Leds Bright.", 1.0, 0.0, 1.0);
    m_ledsBrightness.addListener(ledsManager, &LedsManager::setLedBrightness);
    m_parameters.add(m_ledsBrightness);
    
    m_laserBrightness.set("Laser Bright.", 0.25, 0.0, 1.0);
    m_laserBrightness.addListener(ledsManager, &LedsManager::setLaserBrightness);
    m_parameters.add(m_laserBrightness);

    m_ledsSize.set("Size", 1.0, 0.0, 5.0);
    m_ledsSize.addListener(ledsManager, &LedsManager::setSize);
    m_parameters.add(m_ledsSize);
    
    m_ledsPerChannel.set("LedsPerChannel", 100, 1, 300);
    m_ledsPerChannel.addListener(udpManager, &UdpManager::setLedsPerChannel);
    m_parameters.add(m_ledsPerChannel);
    
    
    ofxDatGuiFolder* folder = m_gui.addFolder("LEDS", ofColor::yellow);
    folder->addButton("Reset View");
    folder->addSlider(m_ledsBrightness);
    folder->addSlider(m_laserBrightness);
    folder->addSlider(m_ledsSize);
    folder->addSlider(m_ledsPerChannel);
    folder->expand();
    
    m_gui.addBreak();
}


void GuiManager::update()
{
    m_gui.update();
}


void GuiManager::draw()
{
    if(!m_showGui)
        return;
    
    //this->drawRectangle();
    this->drawGui();
}


void GuiManager::drawGui()
{
    ofEnableAlphaBlending();
    m_gui.draw();
    ofDisableAlphaBlending();
}

void GuiManager::setupGuiEvents()
{
    //m_gui.onDropdownEvent(this, &GuiManager::onDropdownEvent);
    //m_gui.onColorPickerEvent(this, &GuiManager::onColorPickerEvent);
    m_gui.onButtonEvent(this, &GuiManager::onButtonEvent);
    //m_gui.onToggleEvent(this, &GuiManager::onToggleEvent);
    //m_gui.onMatrixEvent(this, &GuiManager::onMatrixEvent);
}


void GuiManager::saveGuiValues(string path)
{
    ofXml xml;
    //ofSerialize(xml, m_parameters);
    xml.serialize(m_parameters);
    
    if(path.empty()){
        xml.save(GUI_SETTINGS_FILE_NAME);
    }
    else{
        xml.save(path);
    }
    
    
}

void GuiManager::loadGuiValues(string path)
{
    ofXml xml;
    if(path.empty()){
         xml.load(GUI_SETTINGS_FILE_NAME);
    }
    else{
         xml.load(path);
    }
    
    xml.deserialize(m_parameters);
    //ofDeserialize(xml, m_parameters);
}



void GuiManager::toggleGui()
{
    m_showGui = !m_showGui;
}

void GuiManager::drawRectangle()
{
    int margin =  LayoutManager::MARGIN;
    ofPushStyle();
    ofSetColor(15);
    ofDrawRectangle( m_gui.getPosition().x - margin, 0, m_gui.getWidth() + 2*margin, ofGetHeight());
    ofPopStyle();
}



void GuiManager::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getName() << " Selected" << endl;
    
    
}

void GuiManager::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getName() << " Selected" << endl;
    
    if(e.target->getName() == "Load Model")
    {
        AppManager::getInstance().getLedsManager().loadLeds();
    }
    else if(e.target->getName() == "Load Video")
    {
        AppManager::getInstance().getVideoManager().loadVideo();
    }
    
    else if(e.target->getName() == "Export")
    {
        AppManager::getInstance().getVideoManager().exportVideo();
    }
    
    else if(e.target->getName() == "Load Test")
    {
        AppManager::getInstance().getVideoManager().loadTest();
        AppManager::getInstance().getLedsManager().loadTest();
    }
    
    else if(e.target->getName() == "Import Project")
    {
        this->importProject();
    }
    
    else if(e.target->getName() == "Save Project")
    {
        this->exportProject();
    }
    
    else if(e.target->getName() == "Save Project As")
    {
        this->exportProjectAs();
    }
    else if(e.target->getName() == "Reset View")
    {
        AppManager::getInstance().getModelManager().resetCamera();
    }
}


void GuiManager::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getName() << " Selected" << endl;
    
}

void GuiManager::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
    cout << "onMatrixEvent " << e.child << " : " << e.enabled << endl;
}


void GuiManager::importProject()
{
    //Open the Open File Dialog
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a .mori file");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess)
    {
        ofLogNotice() <<"GuiManager::importProject -> name: " <<   openFileResult.getName();
        ofLogNotice() <<"GuiManager::importProject -> path: " <<   openFileResult.getPath();
        
        string path = openFileResult.getPath();
        //We have a file, check it and process it
        
        if(this->isValidFile(path)){
            this->loadGuiValues(path);
        }
        
    }else {
        
        ofLogNotice() <<"GuiManager::importProject -> User hit cancel";
    }
}

bool GuiManager::exportProjectAs()
{
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".mori", "Export your .mori file");
    
    if (saveFileResult.bSuccess){
        //m_exportPath = saveFileResult.filePath;
        string path  = ofSplitString(saveFileResult.filePath, ".mori")[0] + ".mori";
        ofLogNotice() <<"GuiManager::exportProject ->  Path: " << path;
        this->saveGuiValues(path);
        m_projectPath = path;
        return true;
    }
    
    return false;
}

void GuiManager::exportProject()
{
    if(m_projectPath.empty()){
        this->exportProjectAs();
    }
    else{
        this->saveGuiValues(m_projectPath);
    }
}



bool GuiManager::isValidFile(const string& path)
{
    
    ofFile file(path);
    
    if (file.exists())
    {
        string fileExtension = ofToUpper(file.getExtension());
        
        //We only want videos
        if (fileExtension == "MORI")
        {
            return true;
        }
        else{
            ofLogNotice() <<"GuiManager::isValidFile -> file is not a mori file ";
            return false;
        }
    }
    else{
        ofLogNotice() <<"GuiManager::isValidFile -> file doesn't exist ";
        return false;
    }
    
    return false;
    
}


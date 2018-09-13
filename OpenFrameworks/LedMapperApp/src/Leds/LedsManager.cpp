/*
 *  LedsManager.cpp
 *  LedMapperApp
 *
 *  Created by Imanol Gomez on 01/08/18.
 *
 */



#include "ofMain.h"

#include "LedsManager.h"
#include "AppManager.h"



LedsManager::LedsManager(): Manager(), m_isNewFrame(false), m_is3D(true)
{
	//Intentionally left empty
}


LedsManager::~LedsManager()
{
    ofLogNotice() <<"LedsManager::Destructor" ;
}


void LedsManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    
    //this->createLedPositions();
    
    ofLogNotice() <<"LedsManager::initialized" ;
    
}


void LedsManager::setupLeds()
{
    ofLogNotice() <<"LedsManager::setupLeds" ;
    
    m_leds.clear();
    
    this->readLedsPosition();
    m_is3D = this->getIs3D();
    //this->sortLeds();
    this->normalizeLeds();
    this->centreLeds();
}

void LedsManager::createLedPositions()
{
    ofLogNotice() <<"LedsManager::createLedPositions" ;
    
    int size = 40;
    
    int id = 0;
    for(int i = 0; i<size/4; i++){
        for(int j = 0; j<size; j++)
        {
            ofPoint pos (j,i) ;
            createLed(pos, id);
            id++;
        }
            
    }
    
    string path = "leds/LedPositionsTest.txt";
    this->saveLeds(path);
    m_is3D = this->getIs3D();
    this->normalizeLeds();
    this->centreLeds();
}

void LedsManager::readLedsPosition()
{
    int id = 0;
    ofBuffer buffer = ofBufferFromFile(m_ledsFilePath);
    ofPoint ledPosition;
    
    if(buffer.size())
    {
        
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
        {
            string line = *it;
            if(!line.empty() && parseLedLine(line,ledPosition))
            {
                createLed(ledPosition, id);
                id++;
            }
            
        }
    }
}


bool LedsManager::getIs3D()
{
    bool is3D = false;
    if(m_leds.size()<2){
        return false;
    }
    
    for(int i=0; i<m_leds.size()-1; i++)
    {
        auto position = m_leds[i]->getPosition();
        auto positionNext= m_leds[i+1]->getPosition();
        float delta = abs(m_leds[i]->getPosition().z - m_leds[i+1]->getPosition().z);
        
        if( delta > 0.001){
            is3D = true;
        }
    }
  
    
    ofLogNotice() <<"LedsManager::getIs3D -> value = " << is3D ;
    return is3D;
}

void LedsManager::sortLeds()
{
    
    LedVector aux;
    
    while (!m_leds.empty())
    {
        int min = 10000;
        int n = 0;
        
        for (int i = 0; i<m_leds.size(); i++) {
            if(m_leds[i]->getId() < min)
            {
                n = i;
                min = m_leds[i]->getId() ;
            }
        }
        
        aux.push_back(m_leds[n]);
        m_leds.erase(m_leds.begin() + n);
    }
    
    m_leds = aux;
    //std::sort(m_leds.begin(), m_leds.end(), compare);
    
    //auto comp = []( const Led& w1, const Led& w2 ){ return w1.getId() < w2.getId(); }
    
    //std::sort(m_leds.begin(), m_leds.end(), [] (Led const& a, Led const& b) { return a.getId() < b.getId(); });
}

void LedsManager::normalizeLeds()
{
    float max = 0;
    for (auto led: m_leds)
    {
        auto position = led->getPosition();
        
        if(max < abs(position.x)){
            max = abs(position.x);
        }
        
        if(max < abs(position.y)){
            max = abs(position.y);
        }
        
        if(max < abs(position.z)){
            max = abs(position.z);
        }
    }
    
     ofLogNotice() <<"LedsManager::normalizeLeds -> max value =  " << max;
    
    for (auto led: m_leds)
    {
        auto position = led->getPosition();
        position/=max;
        led->setPosition(position);
        
        ofLogNotice() <<"LedsManager::normalizeLeds -> id " << led->getId() << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y << ", z = " << led->getPosition().z ;
    }
    
}

void LedsManager::centreLeds()
{
    
    bool firstIteration = true;
    
    for (auto led: m_leds)
    {
        auto position = led->getPosition();
        
        if(firstIteration){
            firstIteration = false;
            m_maxPos = position;
            m_minPos = position;
        }
        
        if(m_maxPos.x < position.x){
            m_maxPos.x = position.x;
        }
        
        if(m_maxPos.y < position.y){
            m_maxPos.y = position.y;
        }
        
        if(m_maxPos.z < position.z){
            m_maxPos.z = position.z;
        }
        
        if(m_minPos.x > position.x){
            m_minPos.x = position.x;
        }
        
        if(m_minPos.y > position.y){
            m_minPos.y = position.y;
        }
        
        if(m_minPos.z > position.z){
            m_minPos.z = position.z;
        }
        
    }
    
    ofLogNotice() <<"LedsManager::centreLeds -> min position: x = "  << m_minPos.x << ", y = "  << m_minPos.y << ", z = " << m_minPos.z ;
    ofLogNotice() <<"LedsManager::centreLeds -> max position: x = "  << m_maxPos.x << ", y = "  << m_maxPos.y << ", z = " << m_maxPos.z ;
    
    ofPoint shift = (m_maxPos- m_minPos)*0.5  + m_minPos;
    
    ofLogNotice() <<"LedsManager::centreLeds -> shift position: x = "  << shift.x << ", y = "  << shift.y << ", z = " << shift.z ;
    
    for (auto led: m_leds)
    {
        auto position = led->getPosition();
        position-=shift;
        led->setPosition(position);
        
    }
    
    m_maxPos -=shift;
    m_minPos -=shift;
   
}


void LedsManager::createLed(const ofPoint& position, int& id)
{
    ofPtr<Led> led = ofPtr<Led> (new Led ( position, id ) );
    float size = AppManager::getInstance().getGuiManager().getLedsSize();
    led->setWidth(size);
    m_leds.push_back(led);
    
    ofLogNotice() <<"LedsManager::createLed -> id " << led->getId() << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y << ", z = " << led->getPosition().z ;
}

bool LedsManager::parseLedLine(string& line, ofPoint& position)
{
    if(line.size() == 0){
        return false;
    }

    char chars[] = "{}";
    removeCharsFromString(line, chars);
    
    //vector <string> strings = ofSplitString(line, ". " );
    
    //id = ofToInt(strings[0]);
    
    vector <string> positionsStrings = ofSplitString(line, ", " );
    
    if(positionsStrings.size()!=3){
        return false;
    }
    
    position.x = ofToFloat(positionsStrings[0]);
    position.y = ofToFloat(positionsStrings[1]);
    position.z = ofToFloat(positionsStrings[2]);
    
    return true;
}

void LedsManager::removeCharsFromString( string &str, char* charsToRemove ) {
    for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}

void LedsManager::update()
{
    if(m_isNewFrame){
         m_isNewFrame = false;
         //AppManager::getInstance().getImageManager().update();
    }
   
}

void LedsManager::setPixels(ofPixelsRef pixels)
{
    this->setLedColors(pixels);
    
    //AppManager::getInstance().getImageManager().update();
}

void LedsManager::setLedColors(ofPixelsRef pixels)
{
    for(auto led: m_leds){
        led->setPixelColor(pixels, m_is3D);
    }
    
    m_isNewFrame = true;
    
}


void LedsManager::draw()
{
    for(auto led: m_leds){
        led->draw();
    }
}

void LedsManager::setSize(float& value)
{
    for(auto led: m_leds){
        led->setWidth(value);
    }
}



bool LedsManager::loadLeds()
{
    //Open the Open File Dialog
    ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a .txt");
    
    //Check if the user opened a file
    if (openFileResult.bSuccess){
        
        ofLogNotice() <<"LedsManager::loadLeds -> name: " <<   openFileResult.getName();
        ofLogNotice() <<"LedsManager::loadLeds -> path: " <<   openFileResult.getPath();
        
        string path = openFileResult.getPath();
        return this->load(path);
        
    }else {
        
        ofLogNotice() <<"LedsManager::loadVideo -> User hit cancel";
        return false;
    }
}

bool LedsManager::isValidFile(const string& path)
{
    ofBuffer buffer = ofBufferFromFile(path);
    
    if(buffer.size()==0){
         ofLogNotice() <<"LedsManager::isValidFile -> empty file";
        return false;
    }
    
    ofPoint ledPosition;
    
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it)
    {
        
        string line = *it;
        
        ofLogNotice() << line;
        
        // copy the line to draw later
        // make sure its not a empty line
        if(!line.empty() && !parseLedLine(line,ledPosition)) {
            ofLogNotice() <<"LedsManager::isValidFile -> File not valid";
            return false;
        }
        
        // print out the line
        //cout << line << endl;
        
    }
    
    return true;
}


bool LedsManager::load(string& path)
{
    bool success = isValidFile(path);
    if(success){
        m_ledsFilePath =  path;
        AppManager::getInstance().getGuiManager().setModelPath(m_ledsFilePath);
        AppManager::getInstance().getModelManager().resetCamera();
        this->setupLeds();
    }
    
    return success;
}

bool LedsManager::saveLeds(string& path)
{
    ofFile file;
    if( file.open(path, ofFile::WriteOnly)){
        
        ofLogNotice() <<"LedsManager::saveLeds writing to: " << path;
        for(auto led: m_leds){
            file<< "{" << led->getPosition().x << ", " << led->getPosition().y << ", " << led->getPosition().z << "}\n";
        }
        
       
    }
    else{
        ofLogNotice() <<"LedsManager::saveLeds -> File does not exist: " << path;
		return false;
	}
   
     file.close();
    
	 return true;
}

void LedsManager::loadTest()
{
    string path = "leds/LedPositionsTest.txt";
    this->load(path);
}




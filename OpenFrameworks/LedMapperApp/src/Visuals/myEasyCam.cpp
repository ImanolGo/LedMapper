#include "myEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "AppManager.h"

// when an myEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
myEasyCam::myEasyCam(){
	lastTap	= 0;
	lastDistance = 0;
	drag = 0.9f;
	sensitivityRot = 1.0f;//when 1 moving the mouse from one side to the other of the arcball (min(viewport.width, viewport.height)) will rotate 180degrees. when .5, 90 degrees.
	sensitivityXY = .5f;
	sensitivityZ= .7f;

	bDistanceSet = false; 
	bMouseInputEnabled = true;
	bDoRotate = false;
	bApplyInertia =false;
	bDoTranslate = false;
	bDoScrollZoom = false;
	bInsideArcball = true;
	bEnableMouseMiddleButton = true;
	bAutoDistance = true;
	doTranslationKey = 'm';
	bEventsSet = false;
	events = nullptr;
	rotationFactor = 0.0f;

	reset();
}

//----------------------------------------
myEasyCam::~myEasyCam(){
	disableMouseInput();
}

//----------------------------------------
void myEasyCam::update(ofEventArgs & args){
	viewport = getViewport(this->viewport);
	if(!bDistanceSet && bAutoDistance){
		setDistance(getImagePlaneDistance(viewport), true);
	}
	if(bMouseInputEnabled){

		rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);

		if(events->getMousePressed()) prevMouse = ofVec2f(events->getMouseX(),events->getMouseY());

		if (bDoRotate) {
			updateRotation();
		}else if (bDoTranslate || bDoScrollZoom) {
			updateTranslation(); 
			bDoScrollZoom = false;
		}
	}	
}

//----------------------------------------
void myEasyCam::begin(ofRectangle _viewport){
	if(!bEventsSet){
		setEvents(ofEvents());
	}
	viewport = getViewport(_viewport);
	ofCamera::begin(viewport);
}

//----------------------------------------
void myEasyCam::reset(){
	target.resetTransform();

	target.setPosition(0,0, 0);
	lookAt(target);

	resetTransform();
	setPosition(0, 0, lastDistance);

	xRot = 0;
	yRot = 0;
	zRot = 0;

	moveX = 0;
	moveY = 0;
	moveZ = 0;

	bApplyInertia = false;
	bDoTranslate = false;
	bDoRotate = false;
}

//----------------------------------------
void myEasyCam::setTarget(const ofVec3f& targetPoint){
	target.setPosition(targetPoint);
	lookAt(target);
}

//----------------------------------------
void myEasyCam::setTarget(ofNode& targetNode){
	target = targetNode;
	lookAt(target);
}

//----------------------------------------
ofNode& myEasyCam::getTarget(){
	return target;
}

//----------------------------------------
void myEasyCam::setDistance(float distance){
	setDistance(distance, true);
}

//----------------------------------------
void myEasyCam::setDistance(float distance, bool save){//should this be the distance from the camera to the target?
	if(distance > 0.0f){
		if(save){
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float myEasyCam::getDistance() const{
	return target.getPosition().distance(getPosition());
}

//----------------------------------------
void myEasyCam::setAutoDistance(bool bAutoDistance){
	this->bAutoDistance = bAutoDistance;
	if(bAutoDistance){
		bDistanceSet = false;
	}
}

//----------------------------------------
void myEasyCam::setDrag(float drag){
	this->drag = drag;
}

//----------------------------------------
float myEasyCam::getDrag() const{
	return drag;
}

//----------------------------------------
void myEasyCam::setTranslationKey(char key){
	doTranslationKey = key;
}

//----------------------------------------
char myEasyCam::getTranslationKey(){
	return doTranslationKey;
}

//----------------------------------------
void myEasyCam::enableMouseInput(){
	if(!bMouseInputEnabled && events){
		ofAddListener(events->update, this, &myEasyCam::update);
		ofAddListener(events->mouseDragged , this, &myEasyCam::mouseDragged);
		ofAddListener(events->mousePressed, this, &myEasyCam::mousePressed);
		ofAddListener(events->mouseReleased, this, &myEasyCam::mouseReleased);
		ofAddListener(events->mouseScrolled, this, &myEasyCam::mouseScrolled);
	}
	// if enableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = true;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void myEasyCam::disableMouseInput(){
	if(bMouseInputEnabled && events){
		ofRemoveListener(events->update, this, &myEasyCam::update);
		ofRemoveListener(events->mouseDragged, this, &myEasyCam::mouseDragged);
		ofRemoveListener(events->mousePressed, this, &myEasyCam::mousePressed);
		ofRemoveListener(events->mouseReleased, this, &myEasyCam::mouseReleased);
		ofRemoveListener(events->mouseScrolled, this, &myEasyCam::mouseScrolled);
	}
	// if disableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = false;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void myEasyCam::setEvents(ofCoreEvents & _events){
	// If en/disableMouseInput were called within ofApp::setup(),
	// bMouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will 
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = bMouseInputEnabled;
	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}
	bEventsSet = true;
}

//----------------------------------------
bool myEasyCam::getMouseInputEnabled(){
	return bMouseInputEnabled;
}

//----------------------------------------
void myEasyCam::enableMouseMiddleButton(){
	bEnableMouseMiddleButton = true;
}

//----------------------------------------
void myEasyCam::disableMouseMiddleButton(){
	bEnableMouseMiddleButton = false;
}

//----------------------------------------
bool myEasyCam::getMouseMiddleButtonEnabled(){
	return bEnableMouseMiddleButton;
}

//----------------------------------------
void myEasyCam::updateTranslation(){
	if(bApplyInertia){
		moveX *= drag;
		moveY *= drag;
		moveZ *= drag;
		if(ABS(moveX) <= minDifference && ABS(moveY) <= minDifference && ABS(moveZ) <= minDifference){
			bApplyInertia = false;
			bDoTranslate = false;
		}
		move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
	}else{
		setPosition(prevPosition + ofVec3f(prevAxisX * moveX) + (prevAxisY * moveY) + (prevAxisZ * moveZ));
	}
}	

//----------------------------------------
void myEasyCam::updateRotation(){
	if(bApplyInertia){
		xRot *=drag; 
		yRot *=drag;
		zRot *=drag;

		if(ABS(xRot) <= minDifference && ABS(yRot) <= minDifference && ABS(zRot) <= minDifference){
			bApplyInertia = false;
			bDoRotate = false;
		}
		curRot = ofQuaternion(xRot, getXAxis(), yRot, getYAxis(), zRot, getZAxis());
		setPosition((getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		rotate(curRot);
	}else{
		curRot = ofQuaternion(xRot, prevAxisX, yRot, prevAxisY, zRot, prevAxisZ);
		setPosition((prevPosition-target.getGlobalPosition())*curRot +target.getGlobalPosition());
		setOrientation(prevOrientation * curRot);
	}
}

void myEasyCam::mousePressed(ofMouseEventArgs & mouse){
    
    if(!this->isInside(mouse.x, mouse.y)){
        return;
    }
    
    auto scaledMouse = this->getScaledPoint(mouse.x, mouse.y);
    mouse.x = scaledMouse.x;
    mouse.y = scaledMouse.y;
    
	ofRectangle viewport = getViewport(this->viewport);
	if(viewport.inside(mouse.x, mouse.y)){
		lastMouse = mouse;
		prevMouse = mouse;
		prevAxisX = getXAxis();
		prevAxisY = getYAxis();
		prevAxisZ = getZAxis();
		prevPosition = ofCamera::getGlobalPosition();
		prevOrientation = ofCamera::getGlobalOrientation();

		if((bEnableMouseMiddleButton && mouse.button == OF_MOUSE_BUTTON_MIDDLE) || events->getKeyPressed(doTranslationKey)  || mouse.button == OF_MOUSE_BUTTON_RIGHT){
			bDoTranslate = true;
			bDoRotate = false;
		}else if(mouse.button == OF_MOUSE_BUTTON_LEFT){
			bDoTranslate = false;
			bDoRotate = true;
			if(ofVec2f(mouse.x - viewport.x - (viewport.width/2), mouse.y - viewport.y - (viewport.height/2)).length() < min(viewport.width/2, viewport.height/2)){
				bInsideArcball = true;
			}else{
				bInsideArcball = false;
			}
		}
		bApplyInertia = false;
	}
}

void myEasyCam::mouseReleased(ofMouseEventArgs & mouse){
	unsigned long curTap = ofGetElapsedTimeMillis();
	ofRectangle viewport = getViewport(this->viewport);
	if(lastTap != 0 && curTap - lastTap < doubleclickTime){
		reset();
		return;
	}
	lastTap = curTap;
	bApplyInertia = true;
	mouseVel = mouse  - prevMouse;

	updateMouse(mouse);
	ofVec2f center(viewport.width/2, viewport.height/2);
	int vFlip;
	if(isVFlipped()){
		vFlip = -1;
	}else{
		vFlip =  1;
	}
	zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(prevMouse - ofVec2f(viewport.x, viewport.y) - center);
}

void myEasyCam::mouseDragged(ofMouseEventArgs & mouse)
{
    if(!this->isInside(mouse.x, mouse.y)){
        return;
    }
    
    auto scaledMouse = this->getScaledPoint(mouse.x, mouse.y);
    
	mouseVel = scaledMouse  - lastMouse;

	updateMouse(mouse);
}

void myEasyCam::mouseScrolled(ofMouseEventArgs & mouse){
    
    if(!this->isInside(mouse.x, mouse.y)){
        return;
    }
    
	ofRectangle viewport = getViewport(this->viewport);
	prevPosition = ofCamera::getGlobalPosition();
	prevAxisZ = getZAxis();
	moveZ = mouse.scrollY * 30 * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
	bDoScrollZoom = true;
}

void myEasyCam::updateMouse(const ofMouseEventArgs & mouse){
	ofRectangle viewport = getViewport(this->viewport);
	int vFlip;
	if(isVFlipped()){
		vFlip = -1;
	}else{
		vFlip =  1;
	}
	if(bDoTranslate){
		moveX = 0;
		moveY = 0;
		moveZ = 0;
		if(mouse.button == OF_MOUSE_BUTTON_RIGHT){
			moveZ = mouseVel.y * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
		}else{
			moveX = -mouseVel.x * sensitivityXY * (getDistance() + FLT_EPSILON)/viewport.width;
			moveY = vFlip * mouseVel.y * sensitivityXY * (getDistance() + FLT_EPSILON)/viewport.height;
		}
	}else{
		xRot = 0;
		yRot = 0;
		zRot = 0;
		if(bInsideArcball){
			xRot = vFlip * -mouseVel.y * rotationFactor;
			yRot = -mouseVel.x * rotationFactor;
		}else{
			ofVec2f center(viewport.width/2, viewport.height/2);
			zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
		}
	}
}

bool myEasyCam::isInside(float x, float y)
{
    string name = "Leds";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    
    return  rect->inside(x, y);
}

ofPoint myEasyCam::getScaledPoint(float x, float y)
{
    string name = "Leds";
    auto rect = AppManager::getInstance().getLayoutManager().getWindowRect(name);
    x = ofMap(x,rect->getMinX(), rect->getMaxX(), 0.0, rect->getWidth());
    y = ofMap(y, rect->getMinY(), rect->getMaxY(), 0.0, rect->getHeight());
    
    return ofPoint(x,y);
}


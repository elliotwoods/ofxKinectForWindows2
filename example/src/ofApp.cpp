#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();

	ofSetWindowShape(640 * 2, 480 * 2);
}

//--------------------------------------------------------------
void ofApp::update(){
	this->kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	this->kinect.getDepthSource()->draw(0,0,640,480); // note that the depth texture is RAW so may appear dark
	this->kinect.getColorSource()->draw(640,0,640,480);
	this->kinect.getInfraredSource()->draw(0,480,640,480);
	this->kinect.getBodyIndexSource()->draw(640,480,640,480);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

#include "ofApp.h"

int previewWidth = 640;
int previewHeight = 480;

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	ofSetWindowShape(previewWidth * 2, previewHeight * 2);
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	float sourceRatio;
	float sourceHeight;

	sourceRatio = kinect.getDepthSource()->getHeight() / kinect.getDepthSource()->getWidth();
	sourceHeight = previewWidth * sourceRatio; 
	kinect.getDepthSource()->draw(0, 0 + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight);  // note that the depth texture is RAW so may appear dark
	
	sourceRatio = kinect.getColorSource()->getHeight() / kinect.getColorSource()->getWidth();
	sourceHeight = previewWidth * sourceRatio; 
	kinect.getColorSource()->draw(previewWidth, 0 + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight);
	kinect.getBodySource()->drawProjected(previewWidth, 0 + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight);
	
	sourceRatio = kinect.getInfraredSource()->getHeight() / kinect.getInfraredSource()->getWidth();
	sourceHeight = previewWidth * sourceRatio; 
	kinect.getInfraredSource()->draw(0, previewHeight + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight);
	
	sourceRatio = kinect.getBodyIndexSource()->getHeight() / kinect.getBodyIndexSource()->getWidth();
	sourceHeight = previewWidth * sourceRatio; 
	kinect.getBodyIndexSource()->draw(previewWidth, previewHeight + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight);
	kinect.getBodySource()->drawProjected(previewWidth, previewHeight + (previewHeight - sourceHeight) / 2.0, previewWidth, sourceHeight, ofxKFW2::ProjectionCoordinates::DepthCamera);
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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepth();
	kinect.initColor();
	kinect.initInfrared();
	kinect.initBodyIndex();

	gui.init();

	auto sources = kinect.getSources();
	for(auto source : sources) {
		auto drawingSource = dynamic_pointer_cast<ofBaseDraws>(source);
		if (drawingSource) {
			gui.add(*drawingSource, source->getTypeName());
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

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

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
		ofxCvGui::PanelPtr panel;
		if (drawingSource) {
			panel = gui.add(*drawingSource, source->getTypeName());
			auto colorSource = dynamic_pointer_cast<ofxKFW2::Source::Color>(source);
			if (colorSource) {
				panel->onDraw += [colorSource] (ofxCvGui::DrawArguments &) {
					stringstream message;
					message << "Exposure : " << colorSource->getExposure() << "us" << endl;
					message << "FrameInterval : " << colorSource->getFrameInterval() << "us" << endl;
					message << "Gain : " << colorSource->getGain() << endl;
					message << "Gamma : " << colorSource->getGamma() << endl;
					ofxCvGui::Utils::drawText(message.str(), 20, 60);
				};
			}
		}
	}

	auto worldView = gui.addWorld("World");
	worldView->onDrawWorld += [this] (ofCamera &) {
		this->kinect.getColor()->getTextureReference().bind();
		this->mesh.draw();
		this->kinect.getColor()->getTextureReference().unbind();

		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofSetColor(100, 200, 100);
		this->kinect.getDepth()->drawFrustum();
		ofSetColor(200, 100, 100);
		this->kinect.getColor()->drawFrustum();
		ofPopStyle();
	};
	worldView->onKeyboard += [this, worldView] (ofxCvGui::KeyboardArguments & args) {
		if (args.action == ofxCvGui::KeyboardArguments::Action::Pressed && args.key =='c') {
			worldView->getCamera().toggleCursorDraw();
		}
	};
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	mesh = kinect.getDepth()->getMesh(true, ofxKinectForWindows2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);
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

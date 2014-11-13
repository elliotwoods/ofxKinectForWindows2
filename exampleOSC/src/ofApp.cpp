#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	osc.setup("172.20.10.7", 3567);

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();
	kinect.initBodySource();

	gui.init();

	//setup a gui panel for every kinect source
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
		this->kinect.drawPrettyMesh();
	};

	//if we press the 'c' key on the World panel, then toggle the camera's cursor. This works best when you fullscreen that panel
	worldView->onKeyboard += [this, worldView] (ofxCvGui::KeyboardArguments & args) {
		if (args.action == ofxCvGui::KeyboardArguments::Action::Pressed && args.key =='c') {
			worldView->getCamera().toggleCursorDraw();
		}
	};
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	mesh = kinect.getDepthSource()->getMesh(true, ofxKinectForWindows2::Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);

	ofxOscMessage message;
	message.setAddress("/skeleton");
	auto bodies = kinect.getBodySource()->getBodies();
	const auto & bonesDictionary = ofxKinectForWindows2::Data::Body::getBonesAtlas();
	for(auto body : bodies) {
		if (body.tracked) {
			for(auto bone : bonesDictionary) {
				auto start = body.joints[bone.first].getPosition();
				for(int i=0; i<3; i++) {
					message.addFloatArg(start[i]);
				}
				auto end = body.joints[bone.second].getPosition();
				for(int i=0; i<3; i++) {
					message.addFloatArg(end[i]);
				}
			}
		}
	}
	if(message.getNumArgs() > 0) {
		this->osc.sendMessage(message);
		cout << message.getNumArgs() << endl;
	}
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

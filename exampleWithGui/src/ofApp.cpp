#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();

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
		//NB : This is a copy of the ofxKinectForWindows2::Device::drawPrettyMesh() function
		
		//setup some point cloud properties for kicks
		glPushAttrib(GL_POINT_BIT);
		glPointSize(5.0f);
		glEnable(GL_POINT_SMOOTH);

		ofPushStyle();

		//bind kinect color camera texture and draw mesh from depth (which has texture coordinates)
		this->kinect.getColorSource()->getTextureReference().bind();

		//draw point cloud
		this->mesh.drawVertices();
		
		//draw triangles
		ofSetColor(255, 150);
		this->mesh.drawWireframe();
		
		//draw fills faded
		ofSetColor(255, 50);
		this->mesh.drawFaces();

		//unbind colour camera
		this->kinect.getColorSource()->getTextureReference().unbind();

		ofPopStyle();

		//clear the point cloud drawing attributes
		glPopAttrib();

		//draw the view cones of depth and colour cameras
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofSetColor(100, 200, 100);
		this->kinect.getDepthSource()->drawFrustum();
		ofSetColor(200, 100, 100);
		this->kinect.getColorSource()->drawFrustum();
		ofPopStyle();
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

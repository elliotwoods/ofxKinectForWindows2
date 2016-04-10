#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//suggest running this in Release only (very slow in Debug)

	kinect.open();
	kinect.initColorSource();
	kinect.initDepthSource();

	gui.init();
	auto mainStrip = gui.addStrip();
	mainStrip->setDirection(ofxCvGui::Panels::Groups::Strip::Direction::Vertical);
	mainStrip->setCellSizes({ 33, 66 });
	
	auto widgets = ofxCvGui::Panels::makeWidgets();
	{
		widgets->addFps();
		widgets->addToggle(drawNativeWorld);
		widgets->addToggle(drawCalculatedWorld);
	}

	auto leftStrip = ofxCvGui::Panels::Groups::makeStrip();
	mainStrip->add(leftStrip);
	{
		leftStrip->add(widgets);
		leftStrip->add(ofxCvGui::Panels::makeBaseDraws(*kinect.getColorSource(), "Color"));
		leftStrip->add(ofxCvGui::Panels::makeBaseDraws(*kinect.getDepthSource(), "Depth"));
		leftStrip->add(ofxCvGui::Panels::makeBaseDraws(depthToWorldPreview, "Depth to World table"));
	}

	auto world = ofxCvGui::Panels::makeWorld();
	mainStrip->add(world);
	world->onDrawWorld += [this](ofCamera &) {
		if (this->drawNativeWorld) {
			ofPushStyle();
			{
				ofSetColor(ofColor::white);
				this->nativeWorld.drawVertices();
			}
			ofPopStyle();
		}
		if (this->drawCalculatedWorld) {
			ofPushStyle();
			{
				ofSetColor(ofColor::red);
				this->calculatedWorld.drawVertices();
			}
			ofPopStyle();
		}
	};
	world->getCamera().setPosition(-1, +1, -2);
	world->getCamera().lookAt(ofVec3f(0, 0, 3));
	world->getCamera().setCursorDrawEnabled(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	if (kinect.isFrameNew()) {
		auto depth = kinect.getDepthSource();

		//this next part only needs to happen once
		{
			//load the depth to world table
			depth->getDepthToWorldTable(depthToWorldTable);

			//load it into our preview
			depthToWorldPreview.loadData(depthToWorldTable);
		}

		//get the usual mesh
		nativeWorld = depth->getMesh();
		
		//build a mesh using the depthToWorldTable
		{
			auto size = depth->getWidth() * depth->getHeight();
			
			auto depthPixel = depth->getPixels().getData();
			auto depthToWorldRay = (ofVec2f*)depthToWorldTable.getData();

			this->calculatedWorld.clear();

			for (int i = 0; i < size; i++) {
				auto z = (float)*depthPixel / 1000.0f;

				ofVec3f vertex{
					depthToWorldRay->x * z,
					depthToWorldRay->y * z,
					z
				};
				calculatedWorld.addVertex(vertex);

				depthPixel++;
				depthToWorldRay++;
			}
		}
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
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

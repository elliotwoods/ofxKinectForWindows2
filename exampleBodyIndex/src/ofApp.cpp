#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initBodySource();
	kinect.initBodyIndexSource();

	bStitchFaces = false;
	bDrawBodies = true;

	if (ofIsGLProgrammableRenderer()) {
		shader.load("shaders_gl3/bodyIndex");
	}
	else {
		shader.load("shaders/bodyIndex");
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	ofPushMatrix();
	ofScale(100, 100, 100);

	shader.begin();
	shader.setUniform1i("uWidth", kinect.getBodyIndexSource()->getWidth());
	if (ofIsGLProgrammableRenderer()) {
		shader.setUniformTexture("uBodyIndexTex", kinect.getBodyIndexSource()->getTexture(), 1);
		shader.setUniformTexture("uColorTex", kinect.getColorSource()->getTexture(), 2);
	}
	else {
		// TEMP: Until OF master fixes texture binding for old pipeline.
		shader.setUniform1i("uBodyIndexTex", 1);
		kinect.getBodyIndexSource()->getTexture().bind(1);
		shader.setUniform1i("uColorTex", 2);
		kinect.getColorSource()->getTexture().bind(2);
	}
	
	ofSetColor(255);
	ofMesh mesh = kinect.getDepthSource()->getMesh(bStitchFaces, ofxKFW2::Source::Depth::PointCloudOptions::ColorCamera);
	mesh.draw();

	if (!ofIsGLProgrammableRenderer()) {
		// TEMP: Until OF master fixes texture binding for old pipeline.
		kinect.getColorSource()->getTexture().unbind(2);
		kinect.getBodyIndexSource()->getTexture().unbind(1);
	}
	shader.end();

	if (bDrawBodies) {
		kinect.getBodySource()->drawWorld();
	}

	ofPopMatrix();
	cam.end();

	ofSetColor(0);
	stringstream ss;
	ss << ofToString(ofGetFrameRate(), 2) << " FPS" << endl;
	ss << "Stitch [F]aces: " << (bStitchFaces? "ON":"OFF") << endl;
	ss << "Draw [B]odies: " << (bDrawBodies? "ON":"OFF") << endl;
	ofDrawBitmapString(ss.str(), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'b':
		bDrawBodies ^= 1;
		break;
	case 'f':
		bStitchFaces ^= 1;
		break;
	}
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

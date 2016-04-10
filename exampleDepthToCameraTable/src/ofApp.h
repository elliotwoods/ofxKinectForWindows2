#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxCvGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofxKinectForWindows2::Device kinect;
		ofxCvGui::Builder gui;
		ofFloatPixels depthToWorldTable;
		ofTexture depthToWorldPreview;

		ofMesh nativeWorld;
		ofMesh calculatedWorld;

		ofParameter<bool> drawNativeWorld{ "Draw native world", false };
		ofParameter<bool> drawCalculatedWorld{ "Draw calculated world", true };
};

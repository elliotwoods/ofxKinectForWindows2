#pragma once

#define USE_PROGRAMMABLE_PIPELINE 1

#include "ofMain.h"
#include "ofxKinectForWindows2.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxKFW2::Device kinect;

		ofEasyCam cam;
		ofShader shader;

		bool bStitchFaces;
		bool bDrawBodies;
};

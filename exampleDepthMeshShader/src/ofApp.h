/*
	Author: Lesley van Hoek
	https://github.com/lshoek
*/

#pragma once
#include "ofMain.h"
#include "ofxKinectForWindows2.h"

class ofApp : public ofBaseApp
{
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
	
		ofxKFW2::Device kinect;

		ofxKFW2::Source::Depth::PointCloudOptions pointCloudOpts;
		ICoordinateMapper* coordMapper;

		ofFloatPixels depthToCameraSpaceTable;
		ofFloatPixels colorToDepthSpaceTable;

		ofTexture colorTexture;
		ofTexture depthTexture;

		ofShader meshShader;
		ofShader indexShader;

		ofBufferObject depthToCameraSpaceBuffer;
		ofBufferObject vertexBuffer;
		ofBufferObject indexBuffer;

		ofVboMesh depthVboMesh;
		ofMesh depthMesh;

		ofEasyCam cam;

		uint64_t gpuMeshUpdateTimer;
		uint64_t cpuMeshUpdateTimer;

		int meshWidth;
		int meshHeight;

		// config
		bool bFull = true;
		bool bWireFrame = false;
		bool bPoints = false;
		bool bCompareMeshUpdates = false;

		// app
		bool bIsKinectOpen = false;
		bool bMeshDataInitialized = false;

		const int margin = 20;
};

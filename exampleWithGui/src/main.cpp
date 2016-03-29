#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings windowSettings;
	windowSettings.setGLVersion(3, 2);
	windowSettings.width = 1920;
	windowSettings.height = 1080;
	auto window = ofCreateWindow(windowSettings);

	ofRunApp(new ofApp());

}

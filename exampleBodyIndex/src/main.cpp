#include "ofMain.h"
#include "ofApp.h"

//--------------------------------------------------------------
int main(){
#ifdef USE_PROGRAMMABLE_PIPELINE
	ofGLWindowSettings settings;
	settings.setGLVersion(4,3);
	settings.width = 1024;
	settings.height = 768;
	ofCreateWindow(settings);
#else
	ofSetupOpenGL(1024, 768, OF_WINDOW);
#endif

	ofRunApp(new ofApp());
}

#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main()
{
	ofGLFWWindowSettings settings;
	settings.setGLVersion(4, 5);
	settings.setSize(1024, 768);

	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}

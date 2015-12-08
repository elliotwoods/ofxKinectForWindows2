#pragma once

#ifdef _M_IX86
	#error Kinect For Windows 2 requires x64 target. Win32 is not supported.
#endif

#include "ofxKinectForWindows2/Device.h"

#define ofxKFW2 ofxKinectForWindows2
ofxKinectForWindows2
====================

<img src="https://raw.github.com/elliotwoods/ofxKinectForWindows2/master/screenshot.png" />

Implementation of Kinect For Windows v2 API using COM (not KinectCommonBridge)

## Features

Currently supports: 
* Grab RGB, IR, Long Exposure IR, Depth, BodyIndex
* Body tracking (aka skeleton tracking)
* Ground plane detection
* Some remapping functions

Doesn't support:
* Audio
* Face tracking
* All remapping functions

## License

MIT License
http://en.wikipedia.org/wiki/MIT_License

## Usage

1. Add the ofxKinectForWindows2Lib.vcxproj to your solution
2. In `Property Manager`, right click on your project to select `Add Existing Property Sheet...` and select the `ofxKinectForWindows2.props` file

## Notes

0. The depth image comes in as RAW (i'm not rescaling it), so it may appear dark. Look closely :)
1. This addon does not ship with any libraries, it'll look locally on your system for them (and hence should work with VS2012, VS2013, etc)
2. You'll need to get a copy of the Kinect v2 SDK (i'm testing with v1404 April release)

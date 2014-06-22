ofxKinectForWindows2
====================

<img src="https://raw.github.com/elliotwoods/ofxKinectForWindows2/master/screenshot.png" />

Implementation of Kinect For Windows v2 API using COM (not KinectCommonBridge)

## License

MIT License
http://en.wikipedia.org/wiki/MIT_License

## Usage

1. Add the ofxKinectForWindows2Lib.vcxproj to your solution
2. In `Property Manager`, right click on your project to select `Add Existing Property Sheet...` and select the `ofxKinectForWindows2.props` file

## Notes

1. This references external libraries (not internal libraries), so should work with VS2012, VS2013, etc
2. You'll need to get a copy of the Kinect v2 SDK (i'm testing with v1404 April release)

ofxKinectForWindows2
====================

<img src="https://raw.github.com/elliotwoods/ofxKinectForWindows2/master/screenshot.png" />

Implementation of Kinect For Windows v2 API using COM (not KinectCommonBridge)

## Warning

Following PR https://github.com/elliotwoods/ofxKinectForWindows2/pull/13 , we are now working against oF 0.9.0. If you intend to work with 0.8.4 or earlier, please use the following previous version : https://github.com/elliotwoods/ofxKinectForWindows2/tree/0.8.4

## Features

Currently lets you:

* Grab all the image streams (color, depth, IR, long exposure IR, body index)
* Generate a 3D ofMesh (point cloud or stitched mesh) with texture coordinates
* Track bodies (skeleton points, bone maps)
* Transfer coordinates

Currently doesn't support:

* Audio
* Some remapping functions

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

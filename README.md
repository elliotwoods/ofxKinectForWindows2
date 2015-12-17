ofxKinectForWindows2
====================

<img src="https://raw.github.com/elliotwoods/ofxKinectForWindows2/master/screenshot.png" />

Implementation of Kinect For Windows v2 API using COM (not KinectCommonBridge)

## Important notes

The current version is designed for oF 0.9.0. If you are working with 0.8.4 or earlier, please use the previous version : https://github.com/elliotwoods/ofxKinectForWindows2/tree/0.8.4

The Kinect 2 SDK requires an x64 platform target. If you're receiving linker errors regarding _GetDefaultKinectSensor, please ensure that your platform is set to `x64` and not `Win32`.

![x64 Target](https://raw.githubusercontent.com/elliotwoods/ofxKinectForWindows2/master/Notes_Targetx64.PNG)

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
3. Right click on your project (e.g. 'mySketch') and select 'Add Reference...', and add a reference to `ofxKinectForWindows2Lib`.
4. 
## Notes

0. The depth image comes in as RAW (i'm not rescaling it), so it may appear dark. Look closely :)
1. This addon does not ship with any libraries, it'll look locally on your system for them (and hence should work with VS2012, VS2013, etc)
2. You'll need to get a copy of the Kinect v2 SDK (i'm testing with v1404 April release)
3. Make sure you're compiling for x64. See note in Warning regarding linker error.

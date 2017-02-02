ofxKinectForWindows2
====================

![exampleWithGui](https://raw.github.com/elliotwoods/ofxKinectForWindows2/master/screenshots/exampleWithGui.png)

Implementation of Kinect For Windows v2 API using COM (not KinectCommonBridge)

## Important notes

The current version is designed for oF 0.9.0. If you are working with 0.8.4 or earlier, please use the previous version : https://github.com/elliotwoods/ofxKinectForWindows2/tree/0.8.4 . This addon works with Visual Studio 2015 (it is not compatible with non-windows platforms).

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

First install the [Kinect for Windows SDK 2.0](https://www.microsoft.com/en-gb/download/details.aspx?id=44561) installed __(you will need to restart after installing this before all system paths are correctly set)__.

Try the examples to see if they build and run. Then try with your own project:

1. Make your project with Project Generator, but do __not__ select ofxKinectForWindows2 as an addon at this stage
2. Open the solution, and add the ofxKinectForWindows2Lib.vcxproj to your solution (right click on the Solution and choose `Add > Existing Project...`)
3. In `Property Manager` (open it from `View -> Other Windows -> Property Manager`), right click on your project to select `Add Existing Property Sheet...` and select the `ofxKinectForWindows2.props` file.
4. Go back to `Solution Explorer`, right click on your project (e.g. 'mySketch') and select 'Add Reference...', and add a reference to `ofxKinectForWindows2Lib`.

### Alternative Usage with OF Project Generator 

This method requires the [OF nightly build](http://ci.openframeworks.cc/nightlybuilds.html) of Project Generator.  It will not work with the Project Generator from OF 0.9.8 or earlier.

1. Make your project with Project Generator, **including** the ofxKinectForWindows2 addon, and open in IDE (Visual Studio 2015)
2. In `Property Manager` (open it from `View -> Other Windows -> Property Manager`), right click on your project to select `Add Existing Property Sheet...` and select the `ofxKinectForWindows2.props` file.


## Notes

0. The depth image comes in as 'RAW' mm values (i'm not amplifying the values), so it may appear dark. Look closely :)
1. This addon does not ship with any libraries, it'll look locally on your system for them
2. You'll need to get a copy of the Kinect v2 SDK (i'm testing with v1404 April release)

# Troubleshooting

## Can't find `Kinect.h` or `ofxKinectForWindows2.h`

First, please check that you have the Kinect SDK installed. The `Kinect.h` file should be here on your drive:

![Kinect SDK includes](https://github.com/elliotwoods/ofxKinectForWindows2/blob/master/screenshots/KinectSDKIncludes.PNG?raw=true)

Second check that one of the examples shipped with ofxKinectForWindows2 builds and runs.

Third please make sure you're using standard paths, e.g.:

```
addons/ofxKinectForWindows2
apps/myAppFolder/myApp/myApp.sln
```

**If you're using your own path structure then ofxKinectForWindows2 may not work.**

Check that your Solution Explorer has the right settings, e.g.:

![Solution Explorer](https://github.com/elliotwoods/ofxKinectForWindows2/blob/master/screenshots/SolutionExplorer.PNG?raw=true)

Check that the `.props` file has been added correctly, e.g.:

![Property Manager](https://github.com/elliotwoods/ofxKinectForWindows2/blob/master/screenshots/PropertyManager.PNG?raw=true)

## Make sure the file path to your project isn't too long.

If you encounter errors like `The specified task executable CL.exe could not be run` (or similarly ambiguous errors) it may be that the path to your project is too long for Visual Studio to compile (e.g. if your `openFrameworks` folder is in a subfolder on your Desktop).  While there are many solutions to making a shorter path, one easy solution is to move your openFrameworks root directory to something like `C:\of`. [reference](https://github.com/elliotwoods/ofxKinectForWindows2/issues/72).

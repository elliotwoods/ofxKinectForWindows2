#pragma once

#include "ofMain.h"

// If your compiler is complaining that it can't find Kinect.h, then please make sure:
//	1) You have installed the Kinect SDK (e.g. v2.0_1409 is the latest version as of October 2015)
//	2) You have rebooted since installing the SDK. Some environment variables (e.g. the Kinect SDK folder) are only set after restarting.
#include <Kinect.h>

namespace ofxKinectForWindows2 {
	enum ProjectionCoordinates {
		DepthCamera,
		ColorCamera
	};

	namespace Data {
		class Joint
		{
		public:
			Joint(){}
			Joint(const _Joint& j, const _JointOrientation& jo) {
				set(j, jo);
			}

			void set(const _Joint& j, const _JointOrientation& jo) {
				joint = j;
				jOrient = jo;
				orientation.set(jo.Orientation.x, jo.Orientation.y, jo.Orientation.z, jo.Orientation.w);
				position.set(j.Position.X, j.Position.Y, j.Position.Z);
				type = j.JointType;
				trackingState = j.TrackingState;
			}

			JointType getType() const {
				return type;
			}

			ofVec3f getPosition() const {
				return position;
			}

			ofVec2f getProjected(ICoordinateMapper * coordinateMapper, ProjectionCoordinates proj = ColorCamera) const {
				switch (proj) {
				case ColorCamera: {
					ColorSpacePoint projected = { 0 };
					coordinateMapper->MapCameraPointToColorSpace(joint.Position, &projected);
					return ofVec2f(projected.X, projected.Y);
				}
				case DepthCamera: {
					DepthSpacePoint projected = { 0 };
					coordinateMapper->MapCameraPointToDepthSpace(joint.Position, &projected);
					return ofVec2f(projected.X, projected.Y);
				}
				default:
					return ofVec2f();
				}
			}

			ofQuaternion getOrientation() const {
				return orientation;
			}

			TrackingState getTrackingState() const {
				return trackingState;
			}

		protected:
			ofVec3f position;
			ofQuaternion orientation;
			JointType type;
			TrackingState trackingState;

			_Joint joint;
			_JointOrientation jOrient;
		};
	}
}

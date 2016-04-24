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
			Joint(const _Joint& joint, const _JointOrientation& jointOrientation, ICoordinateMapper *);
			Joint(const _Joint& joint, const _JointOrientation& jointOrientation, const ofVec2f & positionInDepthMap);
			void set(const _Joint& joint, const _JointOrientation& jointOrientation, ICoordinateMapper *);
			void set(const _Joint& joint, const _JointOrientation& jointOrientation, const ofVec2f & positionInDepthMap);
			JointType getType() const;
			ofVec3f getPositionInWorld() const;
			ofVec2f getPositionInDepthMap() const;
			ofVec3f getPosition() const {
				return this->getPositionInWorld();
			}
			ofVec2f getProjected(ICoordinateMapper * coordinateMapper, ProjectionCoordinates proj = ColorCamera) const;
			ofQuaternion getOrientation() const;
			TrackingState getTrackingState() const;

			_Joint getRawJoint() const;
			_JointOrientation getRawJointOrientation() const;

			Joint operator*(const ofMatrix4x4 &) const;

		protected:
			ofVec3f positionInWorld;
			ofVec2f positionInDepthMap;
			ofQuaternion orientation;
			JointType type;
			TrackingState trackingState;

			_Joint joint;
			_JointOrientation jointOrientation;
		};
	}
}

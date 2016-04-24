#include "Joint.h"

namespace ofxKinectForWindows2 {
	namespace Data {
		//----------
		Joint::Joint(const _Joint& joint, const _JointOrientation& jointOrientation, ICoordinateMapper * coordinateMapper) {
			this->set(joint, jointOrientation, coordinateMapper);
		}

		//----------
		Joint::Joint(const _Joint& joint, const _JointOrientation& jointOrientation, const ofVec2f & jointInDepthSpace) {
			this->set(joint, jointOrientation, jointInDepthSpace);
		}

		//----------
		void Joint::set(const _Joint& joint, const _JointOrientation& jointOrientation, ICoordinateMapper * coordinateMapper) {
			DepthSpacePoint depthSpacePoint;
			coordinateMapper->MapCameraPointsToDepthSpace(1, &joint.Position, 1, &depthSpacePoint);
			ofVec2f positionInDepthMap(depthSpacePoint.X, depthSpacePoint.Y);
			
			this->set(joint, jointOrientation, positionInDepthMap);
		}

		//----------
		void Joint::set(const _Joint& joint, const _JointOrientation& jointOrientation, const ofVec2f & positionInDepthMap) {
			this->joint = joint;
			this->positionInWorld.set(joint.Position.X, joint.Position.Y, joint.Position.Z);
			this->positionInDepthMap = positionInDepthMap;

			this->type = joint.JointType;
			this->trackingState = joint.TrackingState;

			this->jointOrientation = jointOrientation;
			this->orientation.set(jointOrientation.Orientation.x, jointOrientation.Orientation.y, jointOrientation.Orientation.z, jointOrientation.Orientation.w);
		}

		//----------
		JointType Joint::getType() const {
			return type;
		}

		//----------
		ofVec3f Joint::getPositionInWorld() const {
			return positionInWorld;
		}

		//----------
		ofVec2f Joint::getPositionInDepthMap() const {
			return this->positionInDepthMap;
		}

		//----------
		ofVec2f Joint::getProjected(ICoordinateMapper * coordinateMapper, ProjectionCoordinates proj) const {
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

		//----------
		ofQuaternion Joint::getOrientation() const {
			return orientation;
		}

		//----------
		TrackingState Joint::getTrackingState() const {
			return trackingState;
		}

		//----------
		_Joint Joint::getRawJoint() const {
			return this->joint;
		}

		//----------
		_JointOrientation Joint::getRawJointOrientation() const {
			return this->jointOrientation;
		}

		//----------
		Joint Joint::operator*(const ofMatrix4x4 & transform) const {
			auto copy = *this;
			copy.positionInWorld = copy.positionInWorld * transform;

			ofQuaternion rotationTransform;
			rotationTransform.set(transform);
			copy.orientation = copy.orientation * rotationTransform;

			return copy;
		}
	}
}
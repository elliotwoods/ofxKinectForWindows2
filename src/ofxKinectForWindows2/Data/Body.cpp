#include "Body.h"

namespace ofxKinectForWindows2 {
	namespace Data {
		//----------
		vector<pair<JointType, JointType> > * Body::bonesAtlas = 0;
		
		//----------
		Body::Body() {
			this->bodyId = -1;
			this->trackingId = -1;
			this->tracked = false;
			this->leftHandState = HandState::HandState_Unknown;
			this->rightHandState = HandState::HandState_Unknown;
		}

		//----------
		void Body::drawWorld() const {
			if (!this->tracked) {
				return;
			}
			const auto & boneAtlas = this->getBonesAtlas();
			for(auto & bone : boneAtlas) {
				try {
					const auto & firstJoint = this->joints.at(bone.first);
					const auto & secondJoint = this->joints.at(bone.second);
					if (firstJoint.getTrackingState() != TrackingState::TrackingState_NotTracked && secondJoint.getTrackingState() != TrackingState::TrackingState_NotTracked) {
						ofDrawLine(firstJoint.getPosition(), secondJoint.getPosition());
					}
				}
				catch (...)	{

				}
			}
		}

		//----------
		void Body::clear() {
			joints.clear();
			leftHandState = HandState_Unknown;
			rightHandState = HandState_Unknown;
			tracked = false;
		}

		//----------
		Body Body::operator*(const ofMatrix4x4 & transform) const {
			auto copy = *this;
			for (auto & joint : copy.joints) {
				joint.second = joint.second * transform;
			}
			return copy;
		}

		//----------
		const std::vector<pair<JointType, JointType> > & Body::getBonesAtlas() {
			//if pointer isn't valid, let's initialise the atlas
			if (!bonesAtlas) {
				initBonesAtlas();
			}
			return * Body::bonesAtlas;
		}

		//----------
		void Body::initBonesAtlas() {
			Body::bonesAtlas = new vector<pair<JointType, JointType> >();

#define BONEDEF_ADD(J1, J2) Body::bonesAtlas->push_back( make_pair<JointType, JointType>(JointType_ ## J1, JointType_ ## J2) )
			// Torso
			BONEDEF_ADD	(Head,			Neck);
			BONEDEF_ADD	(Neck,			SpineShoulder);
			BONEDEF_ADD	(SpineShoulder,	SpineMid);
			BONEDEF_ADD	(SpineMid,		SpineBase);
			BONEDEF_ADD	(SpineShoulder,	ShoulderRight);
			BONEDEF_ADD	(SpineShoulder,	ShoulderLeft);
			BONEDEF_ADD	(SpineBase,		HipRight);
			BONEDEF_ADD	(SpineBase,		HipLeft);

			// Right Arm
			BONEDEF_ADD	(ShoulderRight,	ElbowRight);
			BONEDEF_ADD	(ElbowRight,	WristRight);
			BONEDEF_ADD	(WristRight,	HandRight);
			BONEDEF_ADD	(HandRight,		HandTipRight);
			BONEDEF_ADD	(WristRight,	ThumbRight);

			// Left Arm
			BONEDEF_ADD	(ShoulderLeft,	ElbowLeft);
			BONEDEF_ADD	(ElbowLeft,		WristLeft);
			BONEDEF_ADD	(WristLeft,		HandLeft);
			BONEDEF_ADD	(HandLeft,		HandTipLeft);
			BONEDEF_ADD	(WristLeft,		ThumbLeft);

			// Right Leg
			BONEDEF_ADD	(HipRight,		KneeRight);
			BONEDEF_ADD	(KneeRight,		AnkleRight);
			BONEDEF_ADD	(AnkleRight,	FootRight);

			// Left Leg
			BONEDEF_ADD	(HipLeft,	KneeLeft);
			BONEDEF_ADD	(KneeLeft,	AnkleLeft);
			BONEDEF_ADD	(AnkleLeft,	FootLeft);
#undef BONEDEF_ADD
		}
	}
}
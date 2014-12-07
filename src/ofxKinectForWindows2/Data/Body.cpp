#include "Body.h"

namespace ofxKinectForWindows2 {
	namespace Data {
		//----------
		vector<pair<JointType, JointType> > * Body::bonesAtlas = 0;
		
		//----------
		void Body::drawWorld() {
			const auto & boneAtlas = this->getBonesAtlas();
			for(auto & bone : boneAtlas) {
				ofLine(this->joints[bone.first].getPosition(), this->joints[bone.second].getPosition());
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
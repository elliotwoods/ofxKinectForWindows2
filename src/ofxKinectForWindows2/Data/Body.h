#pragma once

#include "Joint.h"

#include <map>
#include <Kinect.h>

namespace ofxKinectForWindows2 {
	namespace Data {
		class Body {
		public:
			Body();
			uint8_t bodyId;
			uint64_t trackingId;
			bool tracked;
			HandState leftHandState;
			HandState rightHandState;
			std::map<JointType, Joint> joints;
			std::map<Activity, DetectionResult> activity;

			void drawWorld() const;
			void clear();

			Body operator*(const ofMatrix4x4 &) const;
			static const std::vector<std::pair<JointType, JointType> > & getBonesAtlas();
		protected:
			static void initBonesAtlas();
			static std::vector<std::pair<JointType, JointType> > * bonesAtlas;
		};
	}
}
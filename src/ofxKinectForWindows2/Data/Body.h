#pragma once

#include "Joint.h"

#include <Kinect.h>

namespace ofxKinectForWindows2 {
	namespace Data {
		class Body {
		public:
			int trackingId;
			bool tracked;
			HandState leftHandState;
			HandState rightHandState;
			std::map<JointType, Joint> joints;
			std::map<Activity, DetectionResult> activity;

			void drawWorld();
			void clear();

			static const std::vector<pair<JointType, JointType> > & getBonesAtlas();

		protected:
			static void initBonesAtlas();
			static vector<pair<JointType, JointType> > * bonesAtlas;
		};
	}
}
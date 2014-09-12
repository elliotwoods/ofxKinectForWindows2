#pragma once

#include <Kinect.h>

#include "Joint.h"

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

			void clear() {
				joints.clear();
				leftHandState = HandState_Unknown;
				rightHandState = HandState_Unknown;
				tracked = false;
			}
		};
	}
}
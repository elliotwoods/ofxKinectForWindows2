#include "Utils.h"

using namespace std;

namespace ofxKinectForWindows2 {
	//----------
	Exception::Exception(const std::string & message) {
		this->message = message;
	}

	//----------
	const char * Exception::what() const {
		return this->message.c_str();
	}

	//----------
#define TO_STRING_CASE(TYPE, VALUE) \
case TYPE ## :: ## TYPE ## _ ## VALUE ## : { \
	return #VALUE; \
}

	string toString(const JointType & jointType) {
		switch (jointType) {
			TO_STRING_CASE(JointType, SpineBase)
			TO_STRING_CASE(JointType, SpineMid)
			TO_STRING_CASE(JointType, Neck)
			TO_STRING_CASE(JointType, Head)
			TO_STRING_CASE(JointType, ShoulderLeft)
			TO_STRING_CASE(JointType, ElbowLeft)
			TO_STRING_CASE(JointType, WristLeft)
			TO_STRING_CASE(JointType, HandLeft)
			TO_STRING_CASE(JointType, ShoulderRight)
			TO_STRING_CASE(JointType, ElbowRight)
			TO_STRING_CASE(JointType, WristRight)
			TO_STRING_CASE(JointType, HandRight)
			TO_STRING_CASE(JointType, HipLeft)
			TO_STRING_CASE(JointType, KneeLeft)
			TO_STRING_CASE(JointType, AnkleLeft)
			TO_STRING_CASE(JointType, FootLeft)
			TO_STRING_CASE(JointType, HipRight)
			TO_STRING_CASE(JointType, KneeRight)
			TO_STRING_CASE(JointType, AnkleRight)
			TO_STRING_CASE(JointType, FootRight)
			TO_STRING_CASE(JointType, SpineShoulder)
			TO_STRING_CASE(JointType, HandTipLeft)
			TO_STRING_CASE(JointType, ThumbLeft)
			TO_STRING_CASE(JointType, HandTipRight)
			TO_STRING_CASE(JointType, ThumbRight)
		default:
			return "Unknown";
		}
	}
}
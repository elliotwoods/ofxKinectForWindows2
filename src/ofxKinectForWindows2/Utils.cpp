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
}
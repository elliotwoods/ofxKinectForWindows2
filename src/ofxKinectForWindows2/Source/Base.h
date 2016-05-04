#pragma once

#include <string>
#include <Kinect.h>

namespace ofxKinectForWindows2 {
	namespace Source {
		class Base {
		public:
			virtual std::string getTypeName() const = 0;
			virtual void init(IKinectSensor *, bool) = 0;
			virtual void update() = 0;
			virtual void update(IMultiSourceFrame *) = 0;
			virtual bool isFrameNew() const = 0;
			virtual bool hasReader() const = 0;
		};
	}
}
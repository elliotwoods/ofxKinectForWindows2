#include "Depth.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string Depth::getTypeName() const {
			return "Depth";
		}

		//----------
		void Depth::init(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IDepthFrameSource * source = NULL;

				if (FAILED(sensor->get_DepthFrameSource(& source))) {
					throw(Exception("Failed to initialise Depth source"));
				}

				if (FAILED(source->OpenReader(& this->reader))) {
					throw(Exception("Failed to initialise Depth reader"));
				}

				SafeRelease(source);

			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}
	}
}
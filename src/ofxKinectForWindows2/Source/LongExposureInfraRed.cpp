#include "LongExposureInfrared.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string LongExposureInfrared::getTypeName() const {
			return "LongExposureInfrared";
		}

		//----------
		void LongExposureInfrared::init(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				ILongExposureInfraredFrameSource * source = NULL;
				
				if (FAILED(sensor->get_LongExposureInfraredFrameSource(& source))) {
					throw(Exception("Failed to initialise LongExposureInfrared source"));
				}

				if (FAILED(source->OpenReader(& this->reader))) {
					throw(Exception("Failed to initialise LongExposureInfrared reader"));
				}

				SafeRelease(source);

			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}
	}
}
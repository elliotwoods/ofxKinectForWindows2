#include "Infrared.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string Infrared::getTypeName() const {
			return "Infrared";
		}

		//----------
		void Infrared::init(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IInfraredFrameSource * source = NULL;
				
				if (FAILED(sensor->get_InfraredFrameSource(& source))) {
					throw(Exception("Failed to initialise Infrared source"));
				}

				if (FAILED(source->OpenReader(& this->reader))) {
					throw(Exception("Failed to initialise Infrared reader"));
				}

				SafeRelease(source);

			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}
	}
}
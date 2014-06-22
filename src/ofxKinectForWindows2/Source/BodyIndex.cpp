#include "BodyIndex.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string BodyIndex::getTypeName() const {
			return "BodyIndex";
		}

		//----------
		void BodyIndex::init(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IBodyIndexFrameSource * source = NULL;

				if (FAILED(sensor->get_BodyIndexFrameSource(& source))) {
					throw(Exception("Failed to initialise BodyIndex source"));
				}

				if (FAILED(source->OpenReader(& this->reader))) {
					throw(Exception("Failed to initialise BodyIndex reader"));
				}

				SafeRelease(source);

			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}
	}
}
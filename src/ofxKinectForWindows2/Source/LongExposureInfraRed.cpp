#include "LongExposureInfrared.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string LongExposureInfrared::getTypeName() const {
			return "LongExposureInfrared";
		}

		//----------
		void LongExposureInfrared::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				ILongExposureInfraredFrameSource * source = NULL;

				if (FAILED(sensor->get_LongExposureInfraredFrameSource(&source))) {
					throw(Exception("Failed to initialise LongExposureInfrared source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise LongExposureInfrared reader"));
				}

				SafeRelease(source);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void LongExposureInfrared::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			ILongExposureInfraredFrame * frame = NULL;
			ILongExposureInfraredFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_LongExposureInfraredFrameReference(&reference))) {
					return; // we often throw here when no new frame is available
				}
				if (FAILED(reference->AcquireFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}
				BaseImageSimple::update(frame);
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(reference);
			SafeRelease(frame);
		}
	}
}
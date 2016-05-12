#include "Infrared.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string Infrared::getTypeName() const {
			return "Infrared";
		}

		//----------
		void Infrared::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IInfraredFrameSource * source = NULL;

				if (FAILED(sensor->get_InfraredFrameSource(&source))) {
					throw(Exception("Failed to initialise Infrared source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise Infrared reader"));
				}

				SafeRelease(source);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Infrared::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			IInfraredFrame * frame = NULL;
			IInfraredFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_InfraredFrameReference(&reference))) {
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
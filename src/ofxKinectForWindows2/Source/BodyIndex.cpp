#include "BodyIndex.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string BodyIndex::getTypeName() const {
			return "BodyIndex";
		}

		//----------
		void BodyIndex::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IBodyIndexFrameSource * source = NULL;

				if (FAILED(sensor->get_BodyIndexFrameSource(&source))) {
					throw(Exception("Failed to initialise BodyIndex source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise BodyIndex reader"));
				}

				SafeRelease(source);

			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void BodyIndex::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			IBodyIndexFrame * frame = NULL;
			IBodyIndexFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_BodyIndexFrameReference(&reference))) {
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
#include "Color.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		Color::Color() {

		}

		//----------
		string Color::getTypeName() const {
			return "Color";
		}

		//----------
		void Color::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IColorFrameSource * source = NULL;

				if (FAILED(sensor->get_ColorFrameSource(&source))) {
					throw(Exception("Failed to initialise Color source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise Color reader"));
				}

				SafeRelease(source);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Color::update(IColorFrame * frame) {
			this->isFrameNewFlag = true;
			IFrameDescription * frameDescription = NULL;
			try {
				//allocate pixels and texture if we need to
				if (FAILED(frame->get_FrameDescription(&frameDescription))) {
					throw Exception("Failed to get frame description");
				}

				int width, height;
				if (FAILED(frameDescription->get_Width(&width)) || FAILED(frameDescription->get_Height(&height))) {
					throw Exception("Failed to get width and height of frame");
				}
				if (width != this->pixels.getWidth() || height != this->texture.getHeight()) {
					this->pixels.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
					this->texture.allocate(this->pixels);
				}

				//update local rgba image
				if (this->rgbaPixelsEnabled) {
					if (FAILED(frame->CopyConvertedFrameDataToArray(this->pixels.size(), this->pixels.getData(), ColorImageFormat_Rgba))) {
						throw Exception("Couldn't pull pixel buffer to converted rgba pixels");
					}
					if (this->useTexture) {
						this->texture.loadData(this->pixels);
					}
				}

				//update yuv
				if (this->yuvPixelsEnabled) {
					if (width != this->yuvPixels.getWidth() || height != this->yuvPixels.getHeight()) {
						this->yuvPixels.allocate(width, height, OF_PIXELS_YUY2);
					}
					if (FAILED(frame->CopyRawFrameDataToArray(this->yuvPixels.size(), this->yuvPixels.getData()))) {
						throw Exception("Couldn't pull raw YUV pixel buffer");
					}
				}

				//update field of view
				if (FAILED(frameDescription->get_HorizontalFieldOfView(&this->horizontalFieldOfView))) {
					throw Exception("Failed to get horizontal field of view");
				}
				if (FAILED(frameDescription->get_VerticalFieldOfView(&this->verticalFieldOfView))) {
					throw Exception("Failed to get vertical field of view");
				}
				if (FAILED(frameDescription->get_DiagonalFieldOfView(&this->diagonalFieldOfView))) {
					throw Exception("Failed to get diagonal field of view");
				}

				IColorCameraSettings * cameraSettings;
				if (FAILED(frame->get_ColorCameraSettings(&cameraSettings))) {
					throw Exception("Failed to get color camera settings");
				}
				cameraSettings->get_ExposureTime(&this->exposure);
				cameraSettings->get_FrameInterval(&this->frameInterval);
				cameraSettings->get_Gain(&this->gain);
				cameraSettings->get_Gamma(&this->gamma);
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
		}

		//----------
		void Color::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			IColorFrame * frame = NULL;
			IColorFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_ColorFrameReference(&reference))) {
					return; // we often throw here when no new frame is available
				}
				if (FAILED(reference->AcquireFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}
				update(frame);
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(reference);
			SafeRelease(frame);
		}

		//----------
		long int Color::getExposure() const {
			return this->exposure;
		}

		//----------
		long int Color::getFrameInterval() const {
			return this->frameInterval;
		}

		//----------
		float Color::getGain() const {
			return this->gain;
		}

		//----------
		float Color::getGamma() const {
			return this->gamma;
		}

		//----------
		void Color::setRgbaPixelsEnabled(bool rgbPixelsEnabled) {
			this->rgbaPixelsEnabled = rgbPixelsEnabled;
		}

		//----------
		bool Color::getRgbaPixelsEnabled() const {
			return this->rgbaPixelsEnabled;
		}

		//----------
		void Color::setYuvPixelsEnabled(bool yuvPixelsEnabled) {
			this->yuvPixelsEnabled = yuvPixelsEnabled;
		}

		//----------
		bool Color::getYuvPixelsEnabled() const {
			return this->yuvPixelsEnabled;
		}

		//----------
		const ofPixels & Color::getYuvPixels() const {
			return this->yuvPixels;
		}
}
}

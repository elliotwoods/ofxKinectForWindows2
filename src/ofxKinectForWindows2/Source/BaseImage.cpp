#include "BaseImage.h"
#include "ofMain.h"

#define CHECK_OPEN if(!this->reader) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Reader is not open"; }

namespace ofxKinectForWindows2 {
	namespace Source {
#pragma mark BaseImageSlim
		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::BaseImageSlim() {
			this->reader = NULL;
			this->useTexture = true;
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::~BaseImageSlim() {
			SafeRelease(this->reader);
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		ofTexture & BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::getTextureReference() {
			return this->texture;
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		void BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::setUseTexture(bool useTexture) {
			this->useTexture = useTexture;
			if (!useTexture) {
				this->texture.clear();
			}
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		PixelType * BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::getPixels() {
			return this->pixels.getPixels();
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		ofPixels_<PixelType> & BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::getPixelsRef() {
			return this->pixels;
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		void BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::draw(float x, float y) {
			this->texture.draw(x, y);
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		void BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::draw(float x, float y, float width, float height) {
			this->texture.draw(x, y, width, height);
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		float BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::getWidth() {
			return this->pixels.getWidth();
		}

		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		float BaseImageSlim OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::getHeight() {
			return this->pixels.getHeight();
		}

#pragma mark BaseImage
		//----------
		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM::update() {
			CHECK_OPEN

			FrameType * frame = NULL;
			IFrameDescription * frameDescription = NULL;
			try {
				//acquire frame
				if (FAILED(this->reader->AcquireLatestFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}

				//allocate pixels and texture if we need to
				if (FAILED(frame->get_FrameDescription(&frameDescription))) {
					throw Exception("Failed to get frame description");
				}
				int width, height;
				if (FAILED(frameDescription->get_Width(&width)) || FAILED(frameDescription->get_Height(&height))) {
					throw Exception("Failed to get width and height of frame");
				}
				if (width != this->pixels.getWidth() || height != this->texture.getHeight()) {
					this->pixels.allocate(width, height, OF_IMAGE_GRAYSCALE);
					this->texture.allocate(this->pixels);
				}

				//update local assets
				if (FAILED(frame->CopyFrameDataToArray(this->pixels.size(), this->pixels.getPixels()))) {
					throw Exception("Couldn't pull pixel buffer ");
				}
				if (this->useTexture) {
					this->texture.loadData(this->pixels);
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
			SafeRelease(frame);
		}

		//----------
		template class BaseImage<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImage<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImage<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImage<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImageSlim<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImageSlim<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImageSlim<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImageSlim<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImageSlim<unsigned char, IColorFrameReader, IColorFrame>;
	}
}
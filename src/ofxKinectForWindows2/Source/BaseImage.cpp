#include "BaseImage.h"
#include "ofMain.h"

#define CHECK_OPEN if(!this->reader) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Reader is not open"; }

namespace ofxKinectForWindows2 {
	namespace Source {

#pragma mark BaseFrame
		//----------
		template <typename ReaderType, typename FrameType>
		BaseFrame<typename ReaderType, typename FrameType>::BaseFrame() {
			this->reader = NULL;
			this->isFrameNewFlag = false;
		}

		//----------
		template <typename ReaderType, typename FrameType>
		BaseFrame<typename ReaderType, typename FrameType>::~BaseFrame() {
			SafeRelease(this->reader);
		}

		//----------
		template <typename ReaderType, typename FrameType>
		void BaseFrame <typename ReaderType, typename FrameType>::init(IKinectSensor * sensor, bool reader) {
			if (reader)
				initReader(sensor);
		}

		//----------
		template <typename ReaderType, typename FrameType>
			bool BaseFrame <typename ReaderType, typename FrameType>::isFrameNew() const {
			return this->isFrameNewFlag;
		}

		//----------
		template <typename ReaderType, typename FrameType>
		bool BaseFrame <typename ReaderType, typename FrameType>::hasReader() const {
			if (this->reader)
				return true;
			else
				return false;
		}

		//----------
		template <typename ReaderType, typename FrameType>
		void BaseFrame<typename ReaderType, typename FrameType>::update() {
			CHECK_OPEN

			this->isFrameNewFlag = false;
			FrameType * frame = NULL;
			try {
				//acquire frame
				if (FAILED(this->reader->AcquireLatestFrame(&frame))) {
					SafeRelease(frame);
					return; // we often throw here when no new frame is available
				}
				update(frame);
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frame);
		}

#pragma mark BaseImage
		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ofMesh BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::frustumMesh;

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::BaseImage() {
			this->useTexture = true;
			this->diagonalFieldOfView = 0.0f;
			this->horizontalFieldOfView = 0.0f;
			this->verticalFieldOfView = 0.0f;
			this->lastFrameTime = 0;

			if (this->frustumMesh.getVertices().empty()) {
				this->frustumMesh.addVertex(ofVec3f(0.0f, 0.0f, 0.0f));
				this->frustumMesh.addVertex(ofVec3f(-1.0f, -1.0f, 1.0f));
				this->frustumMesh.addVertex(ofVec3f(-1.0f, +1.0f, 1.0f));
				this->frustumMesh.addVertex(ofVec3f(+1.0f, +1.0f, 1.0f));
				this->frustumMesh.addVertex(ofVec3f(+1.0f, -1.0f, 1.0f));
				const ofIndexType indices[6] = {0, 1, 2, 3, 4, 1};
				this->frustumMesh.addIndices(indices, 6);
				this->frustumMesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLE_FAN);
			}
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ofTexture & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getTexture() {
			return this->texture;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		const ofTexture & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getTexture() const {
			return this->texture;
		}


		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::setUseTexture(bool useTexture) {
			this->useTexture = useTexture;
			if (!useTexture) {
				this->texture.clear();
			}
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		ofPixels_<PixelType> & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getPixels() {
			return this->pixels;
		}

				//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		const ofPixels_<PixelType> & BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getPixels() const {
			return this->pixels;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::draw(float x, float y) const {
			this->texture.draw(x, y);
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::draw(float x, float y, float width, float height) const {
			this->texture.draw(x, y, width, height);
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
			float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getWidth() const {
				return this->pixels.getWidth();
			}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
			float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getHeight() const {
				return this->pixels.getHeight();
			}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getDiagonalFieldOfView() const {
			return this->diagonalFieldOfView;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getHorizontalFieldOfView() const {
			return this->horizontalFieldOfView;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		float BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::getVerticalFieldOfView() const {
			return this->verticalFieldOfView;
		}

		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::drawFrustum() const {
			ofPushMatrix();
			ofScale(tan(DEG_TO_RAD * this->getHorizontalFieldOfView() / 2.0f), tan(DEG_TO_RAD * this->getVerticalFieldOfView() / 2.0f), 1.0f);
			if (ofGetStyle().bFill) {
				this->frustumMesh.drawFaces();
			} else {
				this->frustumMesh.drawWireframe();
			}
			ofPopMatrix();
		}

#pragma mark BaseImageSimple
		//----------
		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		void BaseImageSimple OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM::update(FrameType * frame) {
			this->isFrameNewFlag = true;
			IFrameDescription * frameDescription = NULL;
			try {
				INT64 relativeTime = 0;
				if (FAILED(frame->get_RelativeTime(&relativeTime))) {
					throw Exception("Failed to get relative time");
				}
				
				if (relativeTime > this->lastFrameTime) {
					relativeTime = this->lastFrameTime;
				} 
				else {
					return;
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
				if (FAILED(frame->CopyFrameDataToArray(width * height, this->pixels.getData()))) {
					throw Exception("Couldn't pull pixel buffer ");
				}
				if (this->useTexture) {
					this->texture.loadData(this->pixels);
				}

				//update field of view
				if (FAILED(frameDescription->get_HorizontalFieldOfView(&this->horizontalFieldOfView))) {
					throw Exception("Failed to get horizonal field of view");
				}
				if (FAILED(frameDescription->get_VerticalFieldOfView(&this->verticalFieldOfView))) {
					throw Exception("Failed to get vertical field of view");
				}
				if (FAILED(frameDescription->get_DiagonalFieldOfView(&this->diagonalFieldOfView))) {
					throw Exception("Failed to get diagonal field of view");
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
		}

		//---------
		template class BaseImageSimple<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImageSimple<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImageSimple<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImageSimple<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImage<unsigned short, IDepthFrameReader, IDepthFrame>;
		template class BaseImage<unsigned short, IInfraredFrameReader, IInfraredFrame>;
		template class BaseImage<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame>;
		template class BaseImage<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame>;
		template class BaseImage<unsigned char, IColorFrameReader, IColorFrame>;
		template class BaseFrame<IBodyFrameReader, IBodyFrame>;
	}
}
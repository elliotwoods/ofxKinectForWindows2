#include "Depth.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		Depth::PointCloudOptions::PointCloudOptions() {
			this->stitchFaces = true;
			this->textureCoordinates = TextureCoordinates::None;
			this->steps = 1;
			this->facesMaxLength = 0.3f;
		}

		//----------
		Depth::PointCloudOptions::PointCloudOptions(bool stitchFaces, TextureCoordinates textureCoordinates) {
			this->stitchFaces = stitchFaces;
			this->textureCoordinates = textureCoordinates;
			this->steps = 1;
			this->facesMaxLength = 0.3f;
		}

		//----------
		string Depth::getTypeName() const {
			return "Depth";
		}

		//----------
		void Depth::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IDepthFrameSource * source = NULL;

				if (FAILED(sensor->get_DepthFrameSource(&source))) {
					throw(Exception("Failed to initialise Depth source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise Depth reader"));
				}

				SafeRelease(source);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Depth::init(IKinectSensor * sensor, bool reader) {
			try {
				BaseFrame::init(sensor, reader);

				if (FAILED(sensor->get_CoordinateMapper(&this->coordinateMapper))) {
					throw(Exception("Failed to acquire coordinate mapper"));
				}

				// Try and get color frame size from frame description.
				// Quietly ignore any possible error, but correctly release any
				// resource already acquired. In this case default values are kept
				IColorFrameSource * source = NULL;
				if (SUCCEEDED(sensor->get_ColorFrameSource(&source))) {
					IFrameDescription * frameDescription = NULL;
					if (SUCCEEDED(source->get_FrameDescription(&frameDescription))) {
						if (SUCCEEDED(frameDescription->get_Width(&this->colorFrameWidth)) &&
							SUCCEEDED(frameDescription->get_Height(&this->colorFrameHeight))) {
							this->colorFrameSize = this->colorFrameWidth * this->colorFrameHeight;
						}
						else {
							OFXKINECTFORWINDOWS2_WARNING << "Failed to get color frame size during depth frame init";
						}
						SafeRelease(frameDescription);
					}
					SafeRelease(source);
				}
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Depth::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			IDepthFrame * frame = NULL;
			IDepthFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_DepthFrameReference(&reference))) {
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

		//----------
		ofMesh Depth::getMesh(const PointCloudOptions &opts) {
			const int width = this->getWidth();
			const int height = this->getHeight();
			const auto frameSize = width * height;

			ofMesh mesh;
			mesh.setMode(opts.stitchFaces ? ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES : ofPrimitiveMode::OF_PRIMITIVE_POINTS);
			mesh.getVertices().resize(frameSize);
			auto vertices = mesh.getVerticesPointer();
			
			this->coordinateMapper->MapDepthFrameToCameraSpace(frameSize, this->pixels.getData(), frameSize, (CameraSpacePoint*) mesh.getVerticesPointer());

			if (opts.stitchFaces) {
				int steps = opts.steps;
				for(int i=0; i<width-steps; i+=steps) {
					for(int j=0; j<height-steps; j+=steps) {
						auto topLeft = width * j + i;
						auto topRight = topLeft + steps;
						auto bottomLeft = topLeft + width * steps;
						auto bottomRight = bottomLeft + steps;
						
						const ofVec3f & vTL = vertices[topLeft];
						const ofVec3f & vTR = vertices[topRight];
						const ofVec3f & vBL = vertices[bottomLeft];
						const ofVec3f & vBR = vertices[bottomRight];

						//upper left triangle
						if (vTL.z > 0 && vTR.z > 0 && vBL.z > 0
							&& abs(vTL.z - vTR.z) < opts.facesMaxLength
							&& abs(vTL.z - vBL.z) < opts.facesMaxLength) {
							const ofIndexType indices[3] = {topLeft, bottomLeft, topRight};
							mesh.addIndices(indices, 3);
						}

						//bottom right triangle
						if (vBR.z > 0 && vTR.z > 0 && vBL.z > 0
							&& abs(vBR.z - vTR.z) < opts.facesMaxLength
							&& abs(vBR.z - vBL.z) < opts.facesMaxLength) {
							const ofIndexType indices[3] = {topRight, bottomRight, bottomLeft};
							mesh.addIndices(indices, 3);
						}
					}
				}
			}

			switch(opts.textureCoordinates) {
			case PointCloudOptions::TextureCoordinates::ColorCamera:
				{
					mesh.getTexCoords().resize(frameSize);
					this->coordinateMapper->MapDepthFrameToColorSpace(frameSize, this->pixels.getData(), frameSize, (ColorSpacePoint*) mesh.getTexCoordsPointer());
				}
				break;
			case PointCloudOptions::TextureCoordinates::DepthCamera:
				{
					mesh.getTexCoords().resize(frameSize);
					auto texCoords = mesh.getTexCoordsPointer();
					for(int i=0; i<frameSize; i++) {
						texCoords[i] = ofVec2f(i % width, i / width);
					}
				}
				break;
			case PointCloudOptions::TextureCoordinates::None:
			default:
				break;
			}

			return mesh;
		}

		//----------
		ofMesh Depth::getMesh(bool stitchFaces, PointCloudOptions::TextureCoordinates textureCoordinates) {
			return this->getMesh(PointCloudOptions(stitchFaces, textureCoordinates));
		}

		//----------
		ofVbo Depth::getVbo(const PointCloudOptions & pointCloudOptions) {
			//this function should be a more optimised version of getMesh(). But we keep this here for compatability right now.
			auto mesh = this->getMesh(pointCloudOptions);
			ofVbo vbo;
			vbo.setIndexData(mesh.getIndexPointer(), mesh.getNumIndices(), GL_STATIC_DRAW);
			vbo.setVertexData(mesh.getVerticesPointer(), mesh.getNumVertices(), GL_STATIC_DRAW);
			vbo.setVertexData(mesh.getTexCoordsPointer(), mesh.getNumTexCoords(), GL_STATIC_DRAW);
			return vbo;
		}

		//----------
		void Depth::getWorldInColorFrame(ofFloatPixels & world) const {
			world.allocate(this->colorFrameWidth, this->colorFrameHeight, ofPixelFormat::OF_PIXELS_RGB);
			this->coordinateMapper->MapColorFrameToCameraSpace(
				this->pixels.size(), this->pixels.getData(),
				this->colorFrameSize, reinterpret_cast<CameraSpacePoint*>(world.getData()));
		}

		//----------
		void Depth::getWorldInDepthFrame(ofFloatPixels & world) const {
			world.allocate(this->getWidth(), this->getHeight(), ofPixelFormat::OF_PIXELS_RGB);
			this->coordinateMapper->MapDepthFrameToCameraSpace(
				this->pixels.size(), this->pixels.getData(),
				this->pixels.size(), reinterpret_cast<CameraSpacePoint*>(world.getData()));
		}

		//----------
		void Depth::getColorInDepthFrameMapping(ofFloatPixels & colorInDepthFrameMapping) const {
			colorInDepthFrameMapping.allocate(this->getWidth(), this->getHeight(), OF_PIXELS_RG);
			this->coordinateMapper->MapDepthFrameToColorSpace(
				this->pixels.size(), this->pixels.getData(),
				this->pixels.size(), reinterpret_cast<ColorSpacePoint*>(colorInDepthFrameMapping.getData()));
		}

		//----------
		void Depth::getDepthInColorFrameMapping(ofFloatPixels & depthInColorFrameMapping) const {
			depthInColorFrameMapping.allocate(this->colorFrameWidth, this->colorFrameHeight, OF_PIXELS_RG);
			this->coordinateMapper->MapColorFrameToDepthSpace(
				this->pixels.size(), this->pixels.getData(),
				this->colorFrameSize, reinterpret_cast<DepthSpacePoint*>(depthInColorFrameMapping.getData()));
		}

		//----------
		void Depth::getDepthToWorldTable(ofFloatPixels & world) const {
			UINT32 tableEntryCount;
			PointF * tableEntries;
			if (FAILED(this->coordinateMapper->GetDepthFrameToCameraSpaceTable(&tableEntryCount, &tableEntries))) {
				OFXKINECTFORWINDOWS2_ERROR << "GetDepthFrameToCameraSpaceTable failed";
				return;
			}

			if (tableEntryCount != this->getWidth() * this->getHeight()) {
				OFXKINECTFORWINDOWS2_ERROR << "wrong tableEntryCount";
			}
			else {
				world.setFromPixels((float*) tableEntries, this->getWidth(), this->getHeight(), 2);
			}
			// The table of camera space points must be released with a call to CoTaskMemFree
			CoTaskMemFree(tableEntries);
		}

		//----------
		ICoordinateMapper * Depth::getCoordinateMapper() const {
			return this->coordinateMapper;
		}
	}
}
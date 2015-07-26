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
		void Depth::init(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IDepthFrameSource * source = NULL;

				if (FAILED(sensor->get_DepthFrameSource(& source))) {
					throw(Exception("Failed to initialise Depth source"));
				}

				if (FAILED(source->OpenReader(& this->reader))) {
					throw(Exception("Failed to initialise Depth reader"));
				}

				SafeRelease(source);

				if (FAILED(sensor->get_CoordinateMapper(&this->coordinateMapper))) {
					throw(Exception("Failed to acquire coordinate mapper"));
				}
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
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
			
			this->coordinateMapper->MapDepthFrameToCameraSpace(frameSize, this->pixels.getPixels(), frameSize, (CameraSpacePoint*) mesh.getVerticesPointer());

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
					this->coordinateMapper->MapDepthFrameToColorSpace(frameSize, this->pixels.getPixels(), frameSize, (ColorSpacePoint*) mesh.getTexCoordsPointer());
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
		ofFloatPixels Depth::getColorToWorldMap(int colorImageWidth, int colorImageHeight) const {
			ofFloatPixels colorToWorldMap;
			colorToWorldMap.allocate(colorImageWidth, colorImageHeight, ofPixelFormat::OF_PIXELS_RGB);
			this->coordinateMapper->MapColorFrameToCameraSpace(this->pixels.getWidth() * this->pixels.getHeight(), this->pixels.getPixels(), colorImageWidth * colorImageHeight, (CameraSpacePoint*)colorToWorldMap.getPixels());
			return colorToWorldMap;
		}

		//----------
		ofFloatPixels Depth::getDepthToWorldMap() const {
			ofFloatPixels depthToWorldMap;
			depthToWorldMap.allocate(this->getWidth(), this->getHeight(), ofPixelFormat::OF_PIXELS_RGB);
			this->coordinateMapper->MapColorFrameToCameraSpace(this->pixels.getWidth() * this->pixels.getHeight(), this->pixels.getPixels(), depthToWorldMap.getWidth() * depthToWorldMap.getHeight(), (CameraSpacePoint*)depthToWorldMap.getPixels());
			return depthToWorldMap;
		}
	}
}
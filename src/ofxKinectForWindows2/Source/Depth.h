#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class Depth : public BaseImageSimple<unsigned short, IDepthFrameReader, IDepthFrame> {
		public:
			struct PointCloudOptions {
				enum TextureCoordinates {
					None,
					DepthCamera,
					ColorCamera
				};

				PointCloudOptions();
				PointCloudOptions(bool stitchFaces, TextureCoordinates textureCoordinates);

				int steps;
				bool stitchFaces;
				float facesMaxLength;
				TextureCoordinates textureCoordinates;
			};

			string getTypeName() const override;
			void init(IKinectSensor *) override;

			ofMesh getMesh(const PointCloudOptions & pointCloudOptions = PointCloudOptions());
			ofMesh getMesh(bool stitchFaces, PointCloudOptions::TextureCoordinates textureCoordinates);
			ofVbo getVbo(const PointCloudOptions & pointCloudOptions = PointCloudOptions());

			void getWorldInColorFrame(ofFloatPixels & world, int colorImageWidth = 1920, int colorImageHeight = 1080) const;
			void getWorldInDepthFrame(ofFloatPixels & world) const;
			void getColorInDepthFrameMapping(ofFloatPixels & colorInDepthFrameMapping) const;
			void getDepthInColorFrameMapping(ofFloatPixels & depthInColorFrameMapping) const;

			ICoordinateMapper * getCoordinateMapper() const;
		protected:
			ICoordinateMapper * coordinateMapper;
		};
	}
}
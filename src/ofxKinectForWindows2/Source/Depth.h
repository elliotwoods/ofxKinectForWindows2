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

			ofFloatPixels getColorToWorldMap(int colorImageWidth = 1920, int colorImageHeight = 1080) const;
			ofFloatPixels getDepthToWorldMap() const;
		protected:
			ICoordinateMapper * coordinateMapper;
		};
	}
}
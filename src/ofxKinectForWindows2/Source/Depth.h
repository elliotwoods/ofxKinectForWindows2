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
				PointCloudOptions(bool stitchFaces, bool useColor, TextureCoordinates textureCoordinates, int steps=1);

				int steps;
				bool stitchFaces;
				TextureCoordinates textureCoordinates;
			};

			string getTypeName() const override;
			void init(IKinectSensor *) override;

			ofMesh getMesh(bool stitchFaces, PointCloudOptions::TextureCoordinates textureCoordinates, int step=1);
			ofMesh getMesh(const PointCloudOptions & pointCloudOptions = PointCloudOptions());
		protected:
			ICoordinateMapper * coordinateMapper;
		};
	}
}
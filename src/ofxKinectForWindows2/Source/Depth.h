#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class Depth : public BaseImage<unsigned short, IDepthFrameReader, IDepthFrame> {
		public:
			string getTypeName() const override;
			void init(IKinectSensor *) override;
		};
	}
}
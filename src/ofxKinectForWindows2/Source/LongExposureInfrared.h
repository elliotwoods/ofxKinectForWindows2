#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class LongExposureInfrared : public BaseImage<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame> {
		public:
			string getTypeName() const override;
			void init(IKinectSensor *) override;
		};
	}
}
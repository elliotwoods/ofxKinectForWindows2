#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class LongExposureInfrared : public BaseImageSimple<unsigned short, ILongExposureInfraredFrameReader, ILongExposureInfraredFrame> {
		public:
			string getTypeName() const override;
			void init(IKinectSensor *) override;
			void update(IMultiSourceFrame *) override;
		};
	}
}
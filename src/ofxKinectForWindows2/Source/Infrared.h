#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class Infrared : public BaseImageSimple<unsigned short, IInfraredFrameReader, IInfraredFrame> {
		public:
			string getTypeName() const override;
			void init(IKinectSensor *) override;
			void update(IMultiSourceFrame *) override;
		};
	}
}
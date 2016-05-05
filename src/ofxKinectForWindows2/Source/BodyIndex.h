#pragma once

#include "BaseImage.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class BodyIndex : public BaseImageSimple<unsigned char, IBodyIndexFrameReader, IBodyIndexFrame> {
		public:
			string getTypeName() const override;
			void update(IMultiSourceFrame *) override;
		protected:
			void initReader(IKinectSensor *) override;
		};
	}
}
#pragma once

#include "BaseImage.h"
#include "../Utils.h"

#include "ofBaseTypes.h"
#include "ofPixels.h"
#include "ofTexture.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		class Color : public BaseImage<unsigned char, IColorFrameReader, IColorFrame> {
		public:
			Color();
			string getTypeName() const override;
			void init(IKinectSensor *) override;
			void update() override;

			long int getExposure() const;
			long int getFrameInterval() const;
			float getGain() const;
			float getGamma() const;
		protected:
			TIMESPAN exposure;
			TIMESPAN frameInterval;
			float gain;
			float gamma;
		};
	}
}
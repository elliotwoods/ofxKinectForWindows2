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

			void update(IColorFrame *) override;
			void update(IMultiSourceFrame *) override;
			long int getExposure() const;
			long int getFrameInterval() const;
			float getGain() const;
			float getGamma() const;

			void setYuvPixelsEnabled(bool yuvPixelsEnabled);
			bool getYuvPixelsEnabled() const;
			const ofPixels & getYuvPixels() const;
		protected:
			void initReader(IKinectSensor *) override;

			TIMESPAN exposure;
			TIMESPAN frameInterval;
			float gain;
			float gamma;

			bool yuvPixelsEnabled;
			ofPixels yuvPixels;
		};
	}
}
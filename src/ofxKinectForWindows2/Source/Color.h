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

			void setRgbaPixelsEnabled(bool rgbaPixelsEnabled);
			bool getRgbaPixelsEnabled() const;

			void setYuvPixelsEnabled(bool yuvPixelsEnabled);
			bool getYuvPixelsEnabled() const;
			const ofPixels & getYuvPixels() const;
		protected:
			void initReader(IKinectSensor *) override;

			TIMESPAN exposure = 0;
			TIMESPAN frameInterval = 0;
			float gain = 0;
			float gamma = 0;

			bool rgbaPixelsEnabled = true;
			bool yuvPixelsEnabled = false;
			ofPixels yuvPixels;
		};
	}
}
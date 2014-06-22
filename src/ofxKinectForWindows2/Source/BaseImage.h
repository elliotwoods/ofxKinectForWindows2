#pragma once

#include "Base.h"
#include "../Utils.h"

#include "ofBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

namespace ofxKinectForWindows2 {
	namespace Source {
#define OFXKFW2_BASEIMAGE_TEMPLATE_ARGS <typename PixelType, typename ReaderType, typename FrameType>
#define OFXKFW2_BASEIMAGE_TEMPLATE_ARGS_TRIM <typename PixelType, typename ReaderType, typename FrameType>

		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		class BaseImageSlim  : public Base, public ofBaseHasTexture, public ofBaseHasPixels_<PixelType>, public ofBaseDraws {
		public:
			BaseImageSlim();
			~BaseImageSlim();

			//ofBaseHasTexture
			ofTexture & getTextureReference() override;
			void setUseTexture(bool) override;

			//ofBaseHasPixels
			PixelType * getPixels() override;
			ofPixels_<PixelType> & getPixelsRef() override;

			//ofBaseDraws
			void draw(float, float) override;
			void draw(float, float, float, float) override;
			float getWidth() override;
			float getHeight() override;
		protected:
			ReaderType * reader;
			bool useTexture;
			ofTexture texture;
			ofPixels_<PixelType> pixels;
		};

		template OFXKFW2_BASEIMAGE_TEMPLATE_ARGS
		class BaseImage : public BaseImageSlim<PixelType, ReaderType, FrameType> {
		public:
			void update() override;
		};
	};
}
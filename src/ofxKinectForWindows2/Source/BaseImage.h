#pragma once

#include "../Utils.h"

#include "ofBaseTypes.h"
#include "ofTexture.h"
#include "ofPixels.h"

#include "Base.h"

namespace ofxKinectForWindows2 {
	namespace Source {
#define OFXKFW2_BaseImageSimple_TEMPLATE_ARGS <typename PixelType, typename ReaderType, typename FrameType>
#define OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM <typename PixelType, typename ReaderType, typename FrameType>

		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		class BaseImage  : public Base, public ofBaseHasTexture, public ofBaseHasPixels_<PixelType>, public ofBaseDraws {
		public:
			BaseImage();
			~BaseImage();

			ReaderType * getReader();

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

			float getDiagonalFieldOfView() const;
			float getHorizontalFieldOfView() const;
			float getVerticalFieldOfView() const;

			void drawFrustum() const;
		protected:
			static ofMesh frustumMesh;

			ReaderType * reader;
			bool useTexture;
			ofTexture texture;
			ofPixels_<PixelType> pixels;

			float diagonalFieldOfView;
			float horizontalFieldOfView;
			float verticalFieldOfView;
		};

		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		class BaseImageSimple : public BaseImage<PixelType, ReaderType, FrameType> {
		public:
			void update() override;
		};
	};
}
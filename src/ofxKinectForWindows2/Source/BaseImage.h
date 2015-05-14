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
			ofTexture & getTexture() override;
			const ofTexture & getTexture() const override;
			void setUseTexture(bool) override;
			bool isUsingTexture() const override { return useTexture; }

			//ofBaseHasPixels
			ofPixels_<PixelType> & getPixels() override;
			const ofPixels_<PixelType> & getPixels() const override;

			//ofBaseDraws
			void draw(float, float) const override;
			void draw(float, float, float, float) const override;
			float getWidth() const override;
			float getHeight() const override;
			
			float getDiagonalFieldOfView() const;
			float getHorizontalFieldOfView() const;
			float getVerticalFieldOfView() const;

			void drawFrustum() const;

			bool IsFrameNew() const;
		protected:
			static ofMesh frustumMesh;

			ReaderType * reader;
			bool useTexture;
			ofTexture texture;
			ofPixels_<PixelType> pixels;

			float diagonalFieldOfView;
			float horizontalFieldOfView;
			float verticalFieldOfView;
			INT64 lastFrameTime;
			bool  isFrameNew;
		};

		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		class BaseImageSimple : public BaseImage<PixelType, ReaderType, FrameType> {
		public:
			void update() override;
		};
	};
}
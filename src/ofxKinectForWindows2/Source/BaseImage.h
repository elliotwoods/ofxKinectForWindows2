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

		template <typename ReaderType, typename FrameType>
		class BaseFrame : public Base {
		public:
			BaseFrame();
			~BaseFrame();

			virtual void update(FrameType *) = 0;
			
			void init(IKinectSensor *, bool) override;
			void update() override;
			bool isFrameNew() const override;
			bool hasReader() const override;
		protected:
			virtual void initReader(IKinectSensor *) = 0;

			ReaderType * reader;
			bool  isFrameNewFlag;
		};

		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		class BaseImage : public BaseFrame<ReaderType, FrameType>, public ofBaseHasTexture, public ofBaseHasPixels_<PixelType>, public ofBaseDraws {
		public:
			BaseImage();

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
		protected:
			static ofMesh frustumMesh;

			bool useTexture;
			ofTexture texture;
			ofPixels_<PixelType> pixels;

			float diagonalFieldOfView;
			float horizontalFieldOfView;
			float verticalFieldOfView;
			long  lastFrameTime;
		};

		template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
		class BaseImageSimple : public BaseImage<PixelType, ReaderType, FrameType> {
		public:
			void update(FrameType *) override;
		};
	};
}
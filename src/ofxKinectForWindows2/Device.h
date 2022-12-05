#pragma once

#include "Utils.h"
#include "Source/Depth.h"
#include "Source/Color.h"
#include "Source/Infrared.h"
#include "Source/LongExposureInfrared.h"
#include "Source/BodyIndex.h"
#include "Source/Body.h"

#include <memory>
#include <vector>

namespace ofxKinectForWindows2 {
	class Device {
	public:
		Device();
		virtual ~Device();
		
		void open();
		void close();
		bool isOpen() const;

		void initMultiSource(std::initializer_list<FrameSourceTypes> frameSourceTypes);

		std::shared_ptr<Source::Depth> initDepthSource();
		std::shared_ptr<Source::Color> initColorSource();
		std::shared_ptr<Source::Infrared> initInfraredSource();
		std::shared_ptr<Source::LongExposureInfrared> initLongExposureInfraredSource();
		std::shared_ptr<Source::BodyIndex> initBodyIndexSource();
		std::shared_ptr<Source::Body> initBodySource();

		bool releaseMultiSource();

		bool releaseDepthSource();
		bool releaseColorSource();
		bool releaseInfraredSource();
		bool releaseLongExposureInfraredSource();
		bool releaseBodyIndexSource();
		bool releaseBodySource();

		void update();
		bool isFrameNew() const;

		template<typename SourceType>
		bool hasSource() const {
			if (this->getSource<SourceType>()) {
				return true;
			}
			else {
				return false;
			}
		}

		template<typename SourceType>
		std::shared_ptr<SourceType> getSource() const {
			for(auto source : this->sources) {
				auto castSource = std::dynamic_pointer_cast<SourceType>(source);
				if (castSource) {
					return castSource;
				}
			}
			return std::shared_ptr<SourceType>();
		}

		const std::vector<std::shared_ptr<Source::Base>> & getSources() const;

		std::shared_ptr<Source::Depth> getDepthSource() const;
		std::shared_ptr<Source::Color> getColorSource() const;
		std::shared_ptr<Source::Infrared> getInfraredSource() const;
		std::shared_ptr<Source::LongExposureInfrared> getLongExposureInfraredSource() const;
		std::shared_ptr<Source::BodyIndex> getBodyIndexSource() const;
		std::shared_ptr<Source::Body> getBodySource() const;

		IKinectSensor * getSensor();

		void drawWorld();
		void setUseTextures(bool);
	protected: 
		template<typename SourceType>
		std::shared_ptr<SourceType> initSource(bool initReader);

		template<typename SourceType>
		bool releaseSource();

		IKinectSensor * sensor;
		IMultiSourceFrameReader * reader;

		std::vector<std::shared_ptr<Source::Base>> sources;
		bool isFrameNewFlag;
	};
}
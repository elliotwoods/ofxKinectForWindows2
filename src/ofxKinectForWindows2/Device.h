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

		template<typename SourceType>
		shared_ptr<SourceType> initSource();

		shared_ptr<Source::Depth> initDepthSource();
		shared_ptr<Source::Color> initColorSource();
		shared_ptr<Source::Infrared> initInfraredSource();
		shared_ptr<Source::LongExposureInfrared> initLongExposureInfraredSource();
		shared_ptr<Source::BodyIndex> initBodyIndexSource();
		shared_ptr<Source::Body> initBodySource();

		void update();
		bool isFrameNew() const;

		template<typename SourceType>
		bool hasSource() const {
			return getSource<SourceType>();
		}

		template<typename SourceType>
		shared_ptr<SourceType> getSource() const {
			for(auto source : this->sources) {
				auto castSource = dynamic_pointer_cast<SourceType>(source);
				if (castSource) {
					return castSource;
				}
			}
			return shared_ptr<SourceType>();
		}

		const vector<shared_ptr<Source::Base>> & getSources() const;

		shared_ptr<Source::Depth> getDepthSource() const;
		shared_ptr<Source::Color> getColorSource() const;
		shared_ptr<Source::Infrared> getInfraredSource() const;
		shared_ptr<Source::LongExposureInfrared> getLongExposureInfraredSource() const;
		shared_ptr<Source::BodyIndex> getBodyIndexSource() const;
		shared_ptr<Source::Body> getBodySource() const;

		IKinectSensor * getSensor();

		void drawWorld();
	protected: 
		IKinectSensor * sensor;
		vector<shared_ptr<Source::Base>> sources;
		bool isFrameNewFlag;
	};
}
#pragma once

#include "Utils.h"
#include "Source/Depth.h"
#include "Source/Color.h"
#include "Source/Infrared.h"
#include "Source/LongExposureInfrared.h"
#include "Source/BodyIndex.h"
#include "Source/BodyFrame.h"

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

		shared_ptr<Source::Depth> initDepth();
		shared_ptr<Source::Color> initColor();
		shared_ptr<Source::Infrared> initInfrared();
		shared_ptr<Source::LongExposureInfrared> initLongExposureInfrared();
		shared_ptr<Source::BodyIndex> initBodyIndex();
		shared_ptr<Source::BodyFrame> initBodyFrame();

		void update();

		template<typename SourceType>
		bool hasSource() {
			return getSource<SourceType>();
		}

		template<typename SourceType>
		shared_ptr<SourceType> getSource() {
			for(auto source : this->sources) {
				auto castSource = dynamic_pointer_cast<SourceType>(source);
				if (castSource) {
					return castSource;
				}
			}
			return shared_ptr<SourceType>();
		}

		vector<shared_ptr<Source::Base>> getSources();

		shared_ptr<Source::Depth> getDepth();
		shared_ptr<Source::Color> getColor();
		shared_ptr<Source::Infrared> getInfrared();
		shared_ptr<Source::LongExposureInfrared> getLongExposureInfrared();
		shared_ptr<Source::BodyIndex> getBodyIndex();
		shared_ptr<Source::BodyFrame> getBodyFrame();

		IKinectSensor * getSensor();

	protected: 
		IKinectSensor * sensor;
		vector<shared_ptr<Source::Base>> sources;
	};
}
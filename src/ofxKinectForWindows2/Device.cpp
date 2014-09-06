#include "Device.h"
#include "ofMain.h"

#define CHECK_OPEN if(!this->sensor) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Sensor is not open"; }

namespace ofxKinectForWindows2 {
	//----------
	Device::Device() {
		this->sensor = nullptr;
	}

	//----------
	Device::~Device() {
		this->close();
	}

	//----------
	void Device::open() {
		try {
			if (FAILED(GetDefaultKinectSensor(&this->sensor))) {
				throw(Exception("Failed to find sensor"));
			}

			if (FAILED(this->sensor->Open())) {
				throw(Exception("Failed to open sensor"));
			}
		} catch (std::exception & e) {
			OFXKINECTFORWINDOWS2_ERROR << e.what();
			this->sensor = nullptr;
		}
	}

	//----------
	void Device::close() {
		if (!this->sensor) {
			return;
		}

		this->sensor->Close();
		this->sensor = nullptr;
	}

	//----------
	bool Device::isOpen() const {
		if (!this->sensor) {
			return false;
		}
		BOOLEAN result;
		if (!FAILED(this->sensor->get_IsOpen(&result))) {
			return result;
		} else {
			OFXKINECTFORWINDOWS2_ERROR << "Failed to check if sensor is open";
			return false;
		}
	}
	
	//----------
	template<typename SourceType>
	shared_ptr<SourceType> Device::initSource() {
		CHECK_OPEN
		try {
			auto depthSource = MAKE(SourceType);
			depthSource->init(this->sensor);
			this->sources.push_back(depthSource);
			return depthSource;
		} catch (std::exception & e) {
			OFXKINECTFORWINDOWS2_ERROR << e.what();
			return shared_ptr<SourceType>();
		}
	}

	//----------
	shared_ptr<Source::Depth> Device::initDepth() {
		return this->initSource<Source::Depth>();
	}

	//----------
	shared_ptr<Source::Color> Device::initColor() {
		return this->initSource<Source::Color>();
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::initInfrared() {
		return this->initSource<Source::Infrared>();
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::initLongExposureInfrared() {
		return this->initSource<Source::LongExposureInfrared>();
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::initBodyIndex() {
		return this->initSource<Source::BodyIndex>();
	}

	//----------
	shared_ptr<Source::BodyFrame> Device::initBodyFrame() {
		return this->initSource<Source::BodyFrame>();
	}

	//----------
	void Device::update() {
		for(auto source : this->sources) {
			source->update();
		}
	}

	//----------
	vector<shared_ptr<Source::Base>> Device::getSources() {
		return this->sources;
	}

	//----------
	shared_ptr<Source::Depth> Device::getDepth() {
		return this->getSource<Source::Depth>();
	}

	//----------
	shared_ptr<Source::Color> Device::getColor() {
		return this->getSource<Source::Color>();
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::getInfrared() {
		return this->getSource<Source::Infrared>();
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::getLongExposureInfrared() {
		return this->getSource<Source::LongExposureInfrared>();
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::getBodyIndex() {
		return this->getSource<Source::BodyIndex>();
	}

	//----------
	shared_ptr<Source::BodyFrame> Device::getBodyFrame() {
		return this->getSource<Source::BodyFrame>();
	}

	//----------
	IKinectSensor * Device::getSensor() {
		return this->sensor;
	}
}
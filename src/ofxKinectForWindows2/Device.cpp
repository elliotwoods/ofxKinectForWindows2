#include "Device.h"
#include "ofMain.h"

#include "ofConstants.h"

#define CHECK_OPEN if(!this->sensor) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Sensor is not open"; }

namespace ofxKinectForWindows2 {
	//----------
	Device::Device() {
		this->sensor = nullptr;
		this->reader = nullptr;
		this->isFrameNewFlag = false;
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
		SafeRelease(this->reader);

		if (!this->sensor) {
			return;
		}

		//release all sources.
		this->sources.clear();

		this->sensor->Close();
		SafeRelease(this->sensor);
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
	void Device::initMultiSource(std::initializer_list<FrameSourceTypes> frameSourceTypes) {
		CHECK_OPEN;

		if (!this->reader) {
			DWORD enabledFrameSourceTypes = 0;
			for (auto frameSourceType : frameSourceTypes) {
				enabledFrameSourceTypes |= frameSourceType;
			}
			try {
				if (!FAILED(this->sensor->OpenMultiSourceFrameReader(enabledFrameSourceTypes, &reader))) {
					if (enabledFrameSourceTypes & FrameSourceTypes_Color) {
						this->initSource<Source::Color>(false);
					}
					if (enabledFrameSourceTypes & FrameSourceTypes_Infrared) {
						this->initSource<Source::Infrared>(false);
					}
					if (enabledFrameSourceTypes & FrameSourceTypes_LongExposureInfrared) {
						this->initSource<Source::LongExposureInfrared>(false);
					}
					if (enabledFrameSourceTypes & FrameSourceTypes_Depth) {
						this->initSource<Source::Depth>(false);
					}
					if (enabledFrameSourceTypes & FrameSourceTypes_BodyIndex) {
						this->initSource<Source::BodyIndex>(false);
					}
					if (enabledFrameSourceTypes & FrameSourceTypes_Body) {
						this->initSource<Source::Body>(false);
					}
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
		}
	}

	//----------
	template<typename SourceType>
	shared_ptr<SourceType> Device::initSource(bool initReader) {
		CHECK_OPEN;

		//first check if it already exists
		{
			auto source = this->getSource<SourceType>();
			if (source) {
				OFXKINECTFORWINDOWS2_WARNING << "Source of type " << typeid(SourceType).name() << " already initialised.";
				return source;
			}
		}

		//if not then open it
		try {
			auto source = MAKE(SourceType);
			source->init(this->sensor, initReader);
			this->sources.push_back(source);
			return source;
		} catch (std::exception & e) {
			OFXKINECTFORWINDOWS2_ERROR << e.what();
			return shared_ptr<SourceType>();
		}
	}

	//----------
	shared_ptr<Source::Depth> Device::initDepthSource() {
		return this->initSource<Source::Depth>(true);
	}

	//----------
	shared_ptr<Source::Color> Device::initColorSource() {
		return this->initSource<Source::Color>(true);
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::initInfraredSource() {
		return this->initSource<Source::Infrared>(true);
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::initLongExposureInfraredSource() {
		return this->initSource<Source::LongExposureInfrared>(true);
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::initBodyIndexSource() {
		return this->initSource<Source::BodyIndex>(true);
	}

	//----------
	shared_ptr<Source::Body> Device::initBodySource() {
		return this->initSource<Source::Body>(true);
	}

	//----------
	bool Device::releaseMultiSource() {
		// look for sources initialized with MultiSource (those without their own reader)
		// and erase them. They are consecutive in the vector
		auto first = this->sources.begin();
		while (first != this->sources.end() && (*first)->hasReader())
			first++;

		if (first == this->sources.end()) {
			OFXKINECTFORWINDOWS2_WARNING << "No sources initialized with MultiSource.";
			return false;
		}

		auto last = first;
		while (last != this->sources.end() && !(*last)->hasReader())
			last++;

		this->sources.erase(first, last);

		return true;
	}

	//----------
	template<typename SourceType>
	bool Device::releaseSource() {
		CHECK_OPEN;

		//check if it already exists
		auto source = this->getSource<SourceType>();
		if (source && source->hasReader()) {
			this->sources.erase(std::remove(this->sources.begin(), this->sources.end(), source), this->sources.end());
			return true;
		}

		//does not exist
		OFXKINECTFORWINDOWS2_WARNING << "Source of type " << typeid(SourceType).name() << " not initialised, or in a MultiSource.";
		return false;
	}

	//----------
	bool Device::releaseDepthSource() {
		return this->releaseSource<Source::Depth>();
	}

	//----------
	bool Device::releaseColorSource() {
		return this->releaseSource<Source::Color>();
	}

	//----------
	bool Device::releaseInfraredSource() {
		return this->releaseSource<Source::Infrared>();
	}

	//----------
	bool Device::releaseLongExposureInfraredSource() {
		return this->releaseSource<Source::LongExposureInfrared>();
	}

	//----------
	bool Device::releaseBodyIndexSource() {
		return this->releaseSource<Source::BodyIndex>();
	}

	//----------
	bool Device::releaseBodySource() {
		return this->releaseSource<Source::Body>();
	}

	//----------
	void Device::update() {
		this->isFrameNewFlag = false;
		IMultiSourceFrame * frame = NULL;
		if (this->reader) {
			try {
				//acquire frame
				if (FAILED(this->reader->AcquireLatestFrame(&frame))) {
					SafeRelease(frame);
					return; // we often throw here when no new frame is available
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
		}
		for (auto source : this->sources) {
			if (frame && !source->hasReader())
				source->update(frame);
			else
				source->update();
			this->isFrameNewFlag |= source->isFrameNew();
		}
		SafeRelease(frame);
	}

	//----------
	bool Device::isFrameNew() const {
		return this->isFrameNewFlag;
	}

	//----------
	const vector<shared_ptr<Source::Base>> & Device::getSources() const {
		return this->sources;
	}

	//----------
	shared_ptr<Source::Depth> Device::getDepthSource() const {
		return this->getSource<Source::Depth>();
	}

	//----------
	shared_ptr<Source::Color> Device::getColorSource() const {
		return this->getSource<Source::Color>();
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::getInfraredSource() const {
		return this->getSource<Source::Infrared>();
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::getLongExposureInfraredSource() const {
		return this->getSource<Source::LongExposureInfrared>();
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::getBodyIndexSource() const {
		return this->getSource<Source::BodyIndex>();
	}

	//----------
	shared_ptr<Source::Body> Device::getBodySource() const {
		return this->getSource<Source::Body>();
	}

	//----------
	IKinectSensor * Device::getSensor() {
		return this->sensor;
	}

	//----------
	void Device::drawWorld() {
		auto colorSource = this->getColorSource();
		auto depthSource = this->getDepthSource();
		auto bodySource = this->getBodySource();

		if (!depthSource) {
			ofLogError("ofxKinectForWindows2::Device::drawPrettyMesh") << "No depth source initialised";
			return;
		}
		
		//point cloud
		{
			//setup some point cloud properties for kicks
			bool usePointSize = true;

#if OF_VERSION_MAJOR > 0 || OF_VERSION_MINOR >= 10
			auto mainWindow = std::static_pointer_cast<ofAppGLFWWindow>(ofGetCurrentWindow());
			usePointSize = mainWindow ? mainWindow->getSettings().glVersionMajor <= 2 : false;
#endif

			usePointSize = false;

			if (usePointSize) {
				glPushAttrib(GL_POINT_BIT);
				glPointSize(5.0f);
				glEnable(GL_POINT_SMOOTH);
			}

			ofPushStyle();

			bool useColor = colorSource.get();
			if (useColor) {
				useColor &= colorSource->getTexture().isAllocated();
			}

			if (useColor) {
				//bind kinect color camera texture and draw mesh from depth (which has texture coordinates)
				colorSource->getTexture().bind();
			}

			auto opts = Source::Depth::PointCloudOptions(true, Source::Depth::PointCloudOptions::TextureCoordinates::ColorCamera);
			auto mesh = depthSource->getMesh(opts);

			//draw point cloud
			mesh.drawVertices();

			//draw triangles
			ofSetColor(255, 150);
			mesh.drawWireframe();

			//draw fills faded
			ofSetColor(255, 50);
			mesh.drawFaces();

			if (useColor) {
				//unbind colour camera
				colorSource->getTexture().unbind();
			}

			ofPopStyle();

			//clear the point cloud drawing attributes
			if (usePointSize) {
				glPopAttrib();
			}
		}
		
		//bodies and floor
		if (bodySource) {
			bodySource->drawWorld();

			ofPushMatrix();
			ofRotateDeg(90, 0, 0, 1);
			ofMultMatrix(bodySource->getFloorTransform());
			ofDrawGridPlane(5.0f);
			ofPopMatrix();
		}

		//draw the view cones of depth and colour cameras
		ofPushStyle();
		ofNoFill();
		ofSetLineWidth(2.0f);
		ofSetColor(100, 200, 100);
		depthSource->drawFrustum();
		if (colorSource) {
			ofSetColor(200, 100, 100);
			colorSource->drawFrustum();
		}
		ofPopStyle();
	}

	//----------
	void Device::setUseTextures(bool useTexture) {
		auto sources = this->getSources();
		for (auto source : sources) {
			auto imageSource = dynamic_pointer_cast<ofBaseHasTexture>(source);
			if (imageSource) {
				imageSource->setUseTexture(useTexture);
			}
		}
	}
}
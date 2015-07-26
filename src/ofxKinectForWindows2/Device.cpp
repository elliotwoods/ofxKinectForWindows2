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
	shared_ptr<Source::Depth> Device::initDepthSource() {
		return this->initSource<Source::Depth>();
	}

	//----------
	shared_ptr<Source::Color> Device::initColorSource() {
		return this->initSource<Source::Color>();
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::initInfraredSource() {
		return this->initSource<Source::Infrared>();
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::initLongExposureInfraredSource() {
		return this->initSource<Source::LongExposureInfrared>();
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::initBodyIndexSource() {
		return this->initSource<Source::BodyIndex>();
	}

	//----------
	shared_ptr<Source::Body> Device::initBodySource() {
		return this->initSource<Source::Body>();
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
	shared_ptr<Source::Depth> Device::getDepthSource() {
		return this->getSource<Source::Depth>();
	}

	//----------
	shared_ptr<Source::Color> Device::getColorSource() {
		return this->getSource<Source::Color>();
	}
	
	//----------
	shared_ptr<Source::Infrared> Device::getInfraredSource() {
		return this->getSource<Source::Infrared>();
	}

	//----------
	shared_ptr<Source::LongExposureInfrared> Device::getLongExposureInfraredSource() {
		return this->getSource<Source::LongExposureInfrared>();
	}

	//----------
	shared_ptr<Source::BodyIndex> Device::getBodyIndexSource() {
		return this->getSource<Source::BodyIndex>();
	}

	//----------
	shared_ptr<Source::Body> Device::getBodySource() {
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
			glPushAttrib(GL_POINT_BIT);
			glPointSize(5.0f);
			glEnable(GL_POINT_SMOOTH);

			ofPushStyle();

			if (colorSource) {
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

			if (colorSource) {
				//unbind colour camera
				colorSource->getTexture().unbind();
			}

			ofPopStyle();

			//clear the point cloud drawing attributes
			glPopAttrib();
		}
		
		//bodies and floor
		if (bodySource) {
			bodySource->drawWorld();

			ofPushMatrix();
			ofRotate(90, 0, 0, 1);
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
}
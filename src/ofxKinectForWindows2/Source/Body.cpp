#include "Body.h"
#include "ofMain.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string Body::getTypeName() const {
			return "Body";
		}

		//----------
		const vector<Data::Body> & Body::getBodies() const {
			return bodies;
		}

		//----------
		ofMatrix4x4 Body::getFloorTransform() {
			ofNode helper;
			helper.lookAt(ofVec3f(floorClipPlane.x, floorClipPlane.z, -floorClipPlane.y));
			helper.boom(-floorClipPlane.w);
			ofMatrix4x4 transform = glm::inverse(helper.getGlobalTransformMatrix());
			return transform;
		}

		//----------
		void Body::initReader(IKinectSensor * sensor) {
			this->reader = NULL;
			try {
				IBodyFrameSource * source = NULL;

				if (FAILED(sensor->get_BodyFrameSource(&source))) {
					throw(Exception("Failed to initialise BodyFrame source"));
				}

				if (FAILED(source->OpenReader(&this->reader))) {
					throw(Exception("Failed to initialise BodyFrame reader"));
				}

				SafeRelease(source);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Body::init(IKinectSensor * sensor, bool reader) {
			try {
				BaseFrame::init(sensor, reader);

				if (FAILED(sensor->get_CoordinateMapper(&this->coordinateMapper))) {
					throw(Exception("Failed to acquire coordinate mapper"));
				}

				bodies.resize(BODY_COUNT);
			} catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void Body::update(IMultiSourceFrame * multiFrame) {
			this->isFrameNewFlag = false;
			IBodyFrame * frame = NULL;
			IBodyFrameReference * reference;
			try {
				//acquire frame
				if (FAILED(multiFrame->get_BodyFrameReference(&reference))) {
					return; // we often throw here when no new frame is available
				}
				if (FAILED(reference->AcquireFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}
				update(frame);
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(reference);
			SafeRelease(frame);
		}

		//----------
		void Body::update(IBodyFrame * frame) {
			this->isFrameNewFlag = true;
			IFrameDescription * frameDescription = NULL;
			try {
				INT64 nTime = 0;
				if (FAILED(frame->get_RelativeTime(&nTime))) {
					throw Exception("Failed to get relative time");
				}
				
				if (FAILED(frame->get_FloorClipPlane(&floorClipPlane))) {
					throw(Exception("Failed to get floor clip plane"));
				}

				IBody* ppBodies[BODY_COUNT] = {0};
				if (FAILED(frame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies))) {
					throw Exception("Failed to refresh body data");
				}

				for (int i = 0; i < BODY_COUNT; ++i) {
					auto & body = bodies[i];
					body.clear();

					IBody* pBody = ppBodies[i];
					if (pBody)
					{
						BOOLEAN bTracked = false;
						if (FAILED(pBody->get_IsTracked(&bTracked))) {
							throw Exception("Failed to get tracking status");
						}
						body.tracked = bTracked;
						body.bodyId = i;

						if (bTracked)
						{
							// retrieve tracking id

							UINT64 trackingId = -1;

							if (FAILED(pBody->get_TrackingId(&trackingId))) {
								throw Exception("Failed to get tracking id");
							}

							body.trackingId = trackingId;

							// retrieve joint position & orientation

							_Joint joints[JointType_Count];
							_JointOrientation jointsOrient[JointType_Count];

							if (FAILED(pBody->GetJoints(JointType_Count, joints))){
								throw Exception("Failed to get joints");
							}
							if (FAILED(pBody->GetJointOrientations(JointType_Count, jointsOrient))){
								throw Exception("Failed to get joints orientation");
							}

							for (int j = 0; j < JointType_Count; ++j) {
								body.joints[joints[j].JointType] = Data::Joint(joints[j], jointsOrient[j], this->getCoordinateMapper());
							}

							// retrieve hand states

							HandState leftHandState = HandState_Unknown;
							HandState rightHandState = HandState_Unknown;

							if (FAILED(pBody->get_HandLeftState(&leftHandState))){
								throw Exception("Failed to get left hand state");
							}
							if (FAILED(pBody->get_HandRightState(&rightHandState))){
								throw Exception("Failed to get right hand state");
							}

							body.leftHandState = leftHandState;
							body.rightHandState = rightHandState;
						}
					}
				}

				for (int i = 0; i < _countof(ppBodies); ++i)
				{
					SafeRelease(ppBodies[i]);
				}
			} catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
		}

		//----------
		map<JointType, ofVec2f> Body::getProjectedJoints(int bodyIdx, ProjectionCoordinates proj) {
			map<JointType, ofVec2f> result;

			const auto & body = bodies[bodyIdx];
			if (!body.tracked) return result;

			for (auto & joint : body.joints) {
				ofVec2f & position = result[joint.second.getType()] = ofVec2f();

				TrackingState state = joint.second.getTrackingState();
				if (state == TrackingState_NotTracked) {
					continue;
				}

				position.set(joint.second.getProjected(coordinateMapper, proj));
			}

			return result;
		}

		//----------
		void Body::drawProjected(int x, int y, int width, int height, ProjectionCoordinates proj) {
			ofPushStyle();
			int w, h;
			switch (proj) {
			case ColorCamera: w = 1920; h = 1080; break;
			case DepthCamera: w = 512; h = 424; break;
			}

			const auto & bonesAtlas = Data::Body::getBonesAtlas();

			for (auto & body : bodies) {
				if (!body.tracked) continue;

				map<JointType, ofVec2f> jntsProj;

				for (auto & j : body.joints) {
					ofVec2f & p = jntsProj[j.second.getType()] = ofVec2f();

					TrackingState state = j.second.getTrackingState();
					if (state == TrackingState_NotTracked) continue;

					p.set(j.second.getProjected(coordinateMapper, proj));
					p.x = x + p.x / w * width;
					p.y = y + p.y / h * height;

					int radius = (state == TrackingState_Inferred) ? 2 : 8;
					ofSetColor(0, 255, 0);
					ofDrawCircle(p.x, p.y, radius);
				}
				
				for (auto & bone : bonesAtlas) {
					drawProjectedBone(body.joints, jntsProj, bone.first, bone.second);
				}

				drawProjectedHand(body.leftHandState, jntsProj[JointType_HandLeft]);
				drawProjectedHand(body.rightHandState, jntsProj[JointType_HandRight]);
			}

			ofPopStyle();
		}

		//----------
		void Body::drawWorld() {
			auto bodies = this->getBodies();
			int bodyIndex = 0;
			for (auto & body : bodies) {
				//draw black lines
				ofPushStyle();
				ofSetLineWidth(10.0f);
				ofSetColor(0);
				body.drawWorld();

				//draw coloured lines
				ofSetLineWidth(8.0f);
				ofColor col(200, 100, 100);
				col.setHue(255.0f / this->getBodies().size() * bodyIndex);
				ofSetColor(col);
				body.drawWorld();

				ofPopStyle();

				bodyIndex++;
			}
		}

		//----------
		ICoordinateMapper * Body::getCoordinateMapper() {
			return this->coordinateMapper;
		}

		//----------
		void Body::drawProjectedBone(map<JointType, Data::Joint> & pJoints, map<JointType, ofVec2f> & pJointPoints, JointType joint0, JointType joint1){
			TrackingState ts1 = pJoints[joint0].getTrackingState();
			TrackingState ts2 = pJoints[joint1].getTrackingState();
			if (ts1 == TrackingState_NotTracked || ts2 == TrackingState_NotTracked) return;
			if (ts1 == TrackingState_Inferred && ts2 == TrackingState_Inferred) return;

			int thickness = 5;
			ofSetColor(0, 255, 0);
			if (ts1 == TrackingState_Inferred || ts2 == TrackingState_Inferred) {
				thickness = 2;
				ofSetColor(0, 128, 0);
			}
			ofSetLineWidth(thickness);
			ofDrawLine(pJointPoints[joint0], pJointPoints[joint1]);
		}

		//----------
		void Body::drawProjectedHand(HandState handState, ofVec2f & handPos){
			ofColor color;
			switch (handState)
			{
			case HandState_Unknown: case HandState_NotTracked:
				return;
			case HandState_Open:
				color = ofColor(0, 255, 0, 80);
				break;
			case HandState_Closed :
				color = ofColor(255, 255, 0, 80);
				break;
			case HandState_Lasso:
				color = ofColor(0, 255, 255, 80);
				break;
			}
			ofEnableAlphaBlending();
			ofSetColor(color);
			ofDrawCircle(handPos, 50);
			ofDisableAlphaBlending();
		}
	}
}

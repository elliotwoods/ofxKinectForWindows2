#include "BodyFrame.h"
#include "ofMain.h"

#define CHECK_OPEN if(!this->reader) { OFXKINECTFORWINDOWS2_ERROR << "Failed : Reader is not open"; }

namespace ofxKinectForWindows2 {
	namespace Source {
		//----------
		string BodyFrame::getTypeName() const {
			return "BodyFrame";
		}

		//----------
		void BodyFrame::init(IKinectSensor * sensor) {
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

				if (FAILED(sensor->get_CoordinateMapper(&this->coordinateMapper))) {
					throw(Exception("Failed to acquire coordinate mapper"));
				}

				bodies.resize(BODY_COUNT);
			}
			catch (std::exception & e) {
				SafeRelease(this->reader);
				throw (e);
			}
		}

		//----------
		void BodyFrame::update() {
			CHECK_OPEN
			
			IBodyFrame * frame = NULL;
			IFrameDescription * frameDescription = NULL;
			try {
				//acquire frame
				if (FAILED(this->reader->AcquireLatestFrame(&frame))) {
					return; // we often throw here when no new frame is available
				}
				INT64 nTime = 0;
				if (FAILED(frame->get_RelativeTime(&nTime))) {
					throw Exception("Failed to get relative time");
				}
				// = { 0 };
				if (FAILED(frame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies))){
					throw Exception("Failed to refresh body data");
				}

				for (int i = 0; i < BODY_COUNT; ++i)
				{
					Body & body = bodies[i];
					body.clear();

					IBody* pBody = ppBodies[i];
					if (pBody)
					{
						BOOLEAN bTracked = false;
						if (FAILED(pBody->get_IsTracked(&bTracked))) {
							throw Exception("Failed to get tracking status");
						}
						body.tracked = bTracked;

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
								body.joints[joints[j].JointType] = Joint(joints[j], jointsOrient[i]);
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
			}
			catch (std::exception & e) {
				OFXKINECTFORWINDOWS2_ERROR << e.what();
			}
			SafeRelease(frameDescription);
			SafeRelease(frame);
		}

		void BodyFrame::drawProjected(int x, int y, int width, int height, ProjectionCoordinates proj) {
			ofPushStyle();

			int w, h;
			switch (proj) {
			case ColorCamera: w = 1920; h = 1080; break;
			case DepthCamera: w = 512; h = 424; break;
			}

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
					ofCircle(p.x, p.y, radius);
				}
				
				drawProjectedBody(body.joints, jntsProj);

				drawProjectedHand(body.leftHandState, jntsProj[JointType_HandLeft]);
				drawProjectedHand(body.rightHandState, jntsProj[JointType_HandRight]);
			}

			ofPopStyle();
		}

		void BodyFrame::drawProjectedBody(map<JointType, Joint> & jnts, map<JointType, ofVec2f> & jntsProj){

			// TORSO
			drawProjectedBone(jnts, jntsProj, JointType_Head,			JointType_Neck);
			drawProjectedBone(jnts, jntsProj, JointType_Neck,			JointType_SpineShoulder);
			drawProjectedBone(jnts, jntsProj, JointType_SpineShoulder,	JointType_SpineMid);
			drawProjectedBone(jnts, jntsProj, JointType_SpineMid,		JointType_SpineBase);
			drawProjectedBone(jnts, jntsProj, JointType_SpineShoulder,	JointType_ShoulderRight);
			drawProjectedBone(jnts, jntsProj, JointType_SpineShoulder,	JointType_ShoulderLeft);
			drawProjectedBone(jnts, jntsProj, JointType_SpineBase,		JointType_HipRight);
			drawProjectedBone(jnts, jntsProj, JointType_SpineBase,		JointType_HipLeft);

			// Right Arm    
			drawProjectedBone(jnts, jntsProj, JointType_ShoulderRight,	JointType_ElbowRight);
			drawProjectedBone(jnts, jntsProj, JointType_ElbowRight,		JointType_WristRight);
			drawProjectedBone(jnts, jntsProj, JointType_WristRight,		JointType_HandRight);
			drawProjectedBone(jnts, jntsProj, JointType_HandRight,		JointType_HandTipRight);
			drawProjectedBone(jnts, jntsProj, JointType_WristRight,		JointType_ThumbRight);

			// Left Arm
			drawProjectedBone(jnts, jntsProj, JointType_ShoulderLeft,	JointType_ElbowLeft);
			drawProjectedBone(jnts, jntsProj, JointType_ElbowLeft,		JointType_WristLeft);
			drawProjectedBone(jnts, jntsProj, JointType_WristLeft,		JointType_HandLeft);
			drawProjectedBone(jnts, jntsProj, JointType_HandLeft,		JointType_HandTipLeft);
			drawProjectedBone(jnts, jntsProj, JointType_WristLeft,		JointType_ThumbLeft);

			// Right Leg
			drawProjectedBone(jnts, jntsProj, JointType_HipRight,		JointType_KneeRight);
			drawProjectedBone(jnts, jntsProj, JointType_KneeRight,		JointType_AnkleRight);
			drawProjectedBone(jnts, jntsProj, JointType_AnkleRight,		JointType_FootRight);

			// Left Leg
			drawProjectedBone(jnts, jntsProj, JointType_HipLeft,		JointType_KneeLeft);
			drawProjectedBone(jnts, jntsProj, JointType_KneeLeft,		JointType_AnkleLeft);
			drawProjectedBone(jnts, jntsProj, JointType_AnkleLeft,		JointType_FootLeft);
		}

		void BodyFrame::drawProjectedBone( map<JointType, Joint> & pJoints, map<JointType, ofVec2f> & pJointPoints, JointType joint0, JointType joint1){

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
			ofLine(pJointPoints[joint0], pJointPoints[joint1]);
		}

		void BodyFrame::drawProjectedHand(HandState handState, ofVec2f & handPos){

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
			ofCircle(handPos, 50);
			ofDisableAlphaBlending();
		}
	}
}
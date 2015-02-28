#pragma once

#include "Base.h"
#include "../Utils.h"
#include "ofMain.h"

#include "../Data/Body.h"
#include "../Data/Joint.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		// -------
		class Body : public Base {
		public:
			string getTypeName() const override;
			void init(IKinectSensor *) override;
			void update();

			void drawBodies();
			void drawFloor();
			void drawProjected(int x, int y, int width, int height, ProjectionCoordinates proj = ColorCamera);

			IBodyFrameReader * getReader();
			const vector<Data::Body> & getBodies() const;
			const vector< pair<JointType, JointType> > & getBonesDef() const;

			const Vector4 getFloorClipPlane() {
				return floorClipPlane;
			}

			ofMatrix4x4 getFloorTransform();

		protected:
			void drawProjectedBone(map<JointType, Data::Joint> & pJoints, map<JointType, ofVec2f> & pJointPoints, JointType joint0, JointType joint1);
			void drawProjectedHand(HandState handState, ofVec2f & handPos);

			void initBonesDefinition();

			IBodyFrameReader * reader;
			ICoordinateMapper * coordinateMapper;

			Vector4 floorClipPlane;

			vector<Data::Body> bodies;
			vector<pair<JointType, JointType> > bonesDef;
		};
	}
}
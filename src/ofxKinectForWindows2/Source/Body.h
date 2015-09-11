#pragma once

#include "ofMain.h"

#include "Base.h"
#include "../Utils.h"

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

			void drawProjected(int x, int y, int width, int height, ProjectionCoordinates proj = ColorCamera);
			void drawWorld();

			IBodyFrameReader * getReader();
			ICoordinateMapper * getCoordinateMapper();

			const vector<Data::Body> & getBodies() const;
			map<JointType, ofVec2f> getProjectedJoints(int bodyIdx, ProjectionCoordinates proj = ColorCamera);

			const Vector4 getFloorClipPlane() {
				return floorClipPlane;
			}

			ofMatrix4x4 getFloorTransform();

			static void drawProjectedBone(map<JointType, Data::Joint> & pJoints, map<JointType, ofVec2f> & pJointPoints, JointType joint0, JointType joint1);
			static void drawProjectedHand(HandState handState, ofVec2f & handPos);

		protected:
			void initBonesDefinition();

			IBodyFrameReader * reader;
			ICoordinateMapper * coordinateMapper;

			Vector4 floorClipPlane;

			vector<Data::Body> bodies;
		};
	}
}
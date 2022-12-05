#pragma once

#include "ofVectorMath.h"

#include "BaseImage.h"
#include "../Utils.h"

#include "../Data/Body.h"
#include "../Data/Joint.h"

namespace ofxKinectForWindows2 {
	namespace Source {
		// -------
		class Body : public BaseFrame<IBodyFrameReader, IBodyFrame> {
		public:
			std::string getTypeName() const override;
			void init(IKinectSensor *, bool) override;

			void update(IBodyFrame *) override;
			void update(IMultiSourceFrame *) override;

			void drawProjected(int x, int y, int width, int height, ProjectionCoordinates proj = ColorCamera);
			void drawWorld();

			ICoordinateMapper * getCoordinateMapper();

			const std::vector<Data::Body> & getBodies() const;
			std::map<JointType, ofVec2f> getProjectedJoints(int bodyIdx, ProjectionCoordinates proj = ColorCamera);

			const Vector4 getFloorClipPlane() {
				return floorClipPlane;
			}

			ofMatrix4x4 getFloorTransform();

			static void drawProjectedBone(std::map<JointType, Data::Joint> & pJoints, std::map<JointType, ofVec2f> & pJointPoints, JointType joint0, JointType joint1);
			static void drawProjectedHand(HandState handState, ofVec2f & handPos);
		protected:
			void initReader(IKinectSensor *) override;

			ICoordinateMapper * coordinateMapper;

			Vector4 floorClipPlane;

			std::vector<Data::Body> bodies;
		};
	}
}
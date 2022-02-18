#include "pendulum.h"

struct DeadZone {
	glm::vec2
		position,
		offset;
};

class Camera {
public:

	void setup(glm::vec2& pendulumPos, float windowWidth, float windowHeight);
	void movement(Pendulum &pendulum, vector<Cameras> &cameras, Dimensions wallsDimensions, float windowWidth, float windowHeight );
	void getCameraRegion(Pendulum &pendulum, vector<Cameras> &cameras, int &actualCamera, int &actualPolygon);
	void computeDeadZonePosition(glm::vec2 &offset, glm::vec2 &pendulumPos);
	void draw();

	glm::vec2
		displayPos,
		targetPos,
		distanceToTarget,
		prevDistanceToTarget;
	float
		displayScale,
		targetScale,
		smoothAmount;
	DeadZone 
		deadZone;
};


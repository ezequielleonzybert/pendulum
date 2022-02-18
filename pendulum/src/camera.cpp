#include "camera.h"
#include "collisions.h"

void Camera::setup(glm::vec2& pendulumPos, float windowWidth, float windowHeight) {
	displayScale = 1;
	targetScale = 1;
	displayPos = -pendulumPos + glm::vec2(windowWidth / 2, windowHeight / 2);
	targetPos = -pendulumPos + glm::vec2(windowWidth / 2, windowHeight / 2);
	deadZone.position = pendulumPos;
	distanceToTarget = { 0,0 };
	smoothAmount = 0.02;
}

void Camera::movement(Pendulum &pendulum, vector<Cameras> &cameras, Dimensions wallsDimensions,  float windowWidth, float windowHeight) {
	
	int currentCamera = -1;
	int currentPolygon = -1;
	float tileSize = windowHeight / 10;

	getCameraRegion(pendulum, cameras, currentCamera, currentPolygon);

	if (currentCamera != -1) {

		const Cameras &camera = cameras[currentCamera];
		const Polygons &polygon = camera.polygons[currentPolygon];
		float camRegionHeight = polygon.dimensions.height;
		float camRegionWidth = polygon.dimensions.width;
		float camRegionMinY = polygon.dimensions.minY;
		float camRegionMaxY = polygon.dimensions.maxY;
		
		switch (currentCamera)
		{
		case 0:
			deadZone.offset = { 100,0 };
			computeDeadZonePosition(deadZone.offset, pendulum.position);
			targetPos.x = ofClamp(windowWidth / 2 - deadZone.position.x, windowWidth / 2 - wallsDimensions.maxX - tileSize, wallsDimensions.minY - tileSize);
			targetPos.y = (windowHeight - camRegionHeight) / 2 - camRegionMinY;
			targetScale = 1;
			break;
		case 1:
			deadZone.offset = { 100, 100 };
			computeDeadZonePosition(deadZone.offset, pendulum.position);
			targetPos.x = ofClamp(windowWidth / 2 - deadZone.position.x, windowWidth / 2 - wallsDimensions.maxX - tileSize, wallsDimensions.minY - tileSize);
			targetPos.y = ofClamp(windowHeight/2 - deadZone.position.y, windowHeight - camRegionMaxY - tileSize, camRegionMinY - tileSize);
			targetScale = .85;
			break;
		case 2:
			deadZone.offset = { 100, 100 };
			computeDeadZonePosition(deadZone.offset, pendulum.position);
			targetPos.x = ofClamp(windowWidth / 2 - deadZone.position.x, windowWidth / 2 - wallsDimensions.maxX - tileSize, wallsDimensions.minY - tileSize);
			targetPos.y = ofClamp(windowHeight / 2 - deadZone.position.y, windowHeight - camRegionMaxY - tileSize, camRegionMinY - tileSize);
			targetScale = .85;
			break;
		default:
			break;
		}
	}

	float
		a = 2,
		b = 2;

	prevDistanceToTarget = distanceToTarget;

	distanceToTarget = (targetPos - displayPos);

	smoothAmount = glm::distance2(prevDistanceToTarget, distanceToTarget) / 100000;
	
	distanceToTarget *= 0.02; 

	displayPos.x += min((pow(a, distanceToTarget.x) - pow(a, -distanceToTarget.x)) * b, 1000.f);
	displayPos.y += min((pow(a, distanceToTarget.y) - pow(a, -distanceToTarget.y)) * b, 1000.f);
	displayScale += (targetScale - displayScale) *.03;
	ofTranslate(displayPos);
}

void Camera::getCameraRegion(Pendulum &pendulum, vector<Cameras> &cameras, int &actualCamera, int &actualPolygon) {

	int rayIntersections = 0;
	glm::vec2 rayOrigin;

	for (int k = 0; k < cameras.size(); k++) {
		for (int j = 0; j < cameras[k].polygons.size(); j++) {
			int pointsAmount = cameras[k].polygons[j].points.size();
			for (int i = 0; i < pointsAmount; i++) { 
				if (!pendulum.hook) 
				{
					rayOrigin = pendulum.position;
				}
				else
				{
					rayOrigin = pendulum.hposition;
				}
				if (collisions::getRayToLineSegmentIntersection(rayOrigin, cameras[k].polygons[j].points[i], cameras[k].polygons[j].points[(i + 1) % pointsAmount]) != NULL) {
					rayIntersections++;
				}
			}
			if (rayIntersections % 2 != 0) {
				actualCamera = k;
				actualPolygon = j;
				return;
			}
			else {
				actualCamera = -1;
				actualPolygon = -1;
			}
		}
	}
}

void Camera::computeDeadZonePosition(glm::vec2 &offset, glm::vec2 &pendulumPos) {
	if (pendulumPos.x > deadZone.position.x + deadZone.offset.x)
	{
		deadZone.position.x += pendulumPos.x - deadZone.offset.x - deadZone.position.x;
	}
	else if (pendulumPos.x < deadZone.position.x - deadZone.offset.x)
	{
		deadZone.position.x += pendulumPos.x + deadZone.offset.x - deadZone.position.x;
	}
	if (pendulumPos.y > deadZone.position.y + deadZone.offset.y)
	{
		deadZone.position.y += pendulumPos.y - deadZone.offset.y - deadZone.position.y;
	}
	else if (pendulumPos.y < deadZone.position.y - deadZone.offset.y)
	{
		deadZone.position.y += pendulumPos.y + deadZone.offset.y - deadZone.position.y;
	}
}

void Camera::draw() {
	ofSetColor(0,100);
	ofDrawCircle(-targetPos, 20);
	ofDrawRectangle(deadZone.position - deadZone.offset, deadZone.offset.x * 2, deadZone.offset.y * 2);
}
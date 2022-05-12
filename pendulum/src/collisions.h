#pragma once
#include "ofMain.h"
#include "stage.h"
#include "pendulum.h"

namespace collisions
{
	struct Range {
		float minimum;
		float maximum;
	};

	const float eps = 1e-14;

	bool pendulumColliding(Pendulum& pendulum, std::vector<Polygons>& polygons);
	bool pointInsidePolygon(glm::vec2 &pointPosition, Polygons &polygon);
	void distanceAndAngleCircleSegment(glm::vec2 &position, float &radius, glm::vec2& point, glm::vec2& nextPoint, glm::vec2& surfacePerpendicular, float& distanceToCollider);
	float getRayToLineSegmentIntersection(glm::vec2 rayOrigin, glm::vec2 point1, glm::vec2 point2);
	bool on_one_side(glm::vec2 linePoint1, glm::vec2 linePoin2, glm::vec2 segmentPoint1, glm::vec2 segmentPoint2);
	glm::vec2 rotate_vector_90(glm::vec2 v);
	bool overlapping(float minA, float maxA, float minB, float maxB);
	bool equal_floats(float a, float b);
	bool parallel_vectors(glm::vec2 a, glm::vec2 b);
	Range sort_range(Range r);
	Range project_segment(glm::vec2 s1, glm::vec2 s2, glm::vec2 onto);
	bool overlapping_ranges(Range a, Range b);
	bool segments_collide(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2);
	bool line_segment_collide(glm::vec2 l1, glm::vec2 l2, glm::vec2 s1, glm::vec2 s2);
	glm::vec2 reflect(glm::vec2 v, glm::vec2 n);
	float Signed2DTriArea(glm::vec2 a, glm::vec2 b, glm::vec2 c);
	void closestPtPointSegment(glm::vec2 position, glm::vec2 segmentP1, glm::vec2 segmentP2, /*float &t,*/ glm::vec2 &closestPoint);
	bool test2DSegmentSegment(glm::vec2 segmentAPt1, glm::vec2 segmentAPt2, glm::vec2 segmentBPt1, glm::vec2 segmentBPt2, vector<glm::vec2> &intersectionPoints);
	float Signed2DTriArea(glm::vec2 a, glm::vec2 b, glm::vec2 c);
	bool pointLiesOnSegment(glm::vec2 point, glm::vec2 pt1, glm::vec2 pt2);
	bool circleSegmentIntersections(glm::vec2 &circlePos, float radius, glm::vec2 pt1, glm::vec2 pt2, vector<glm::vec2> &intersectionPoints);
	glm::vec2 vectorPerpendicular(glm::vec2 vector);
	bool test2DCircleRectangle(glm::vec2 circPosition, float circRadius, glm::vec2 rectPosition, float rectWidth, float rectHeight);
	float sq(float x);
}
	
#include "collisions.h"

namespace collisions 
{
	bool circleSegmentIntersections(glm::vec2 &circlePos, float radius, glm::vec2 pt1, glm::vec2 pt2, vector<glm::vec2> &intersectionPoints)
	{
		bool result = false;
		glm::vec2 segmentUnit = glm::normalize(pt2 - pt1);
		float t = glm::dot(circlePos - pt1, segmentUnit);
		glm::vec2 midPoint = pt1 + segmentUnit * t;
		glm::vec2 closestPoint;
		closestPtPointSegment(circlePos, pt1, pt2, closestPoint);
		float 
			y = glm::length(circlePos - midPoint),
			x = sqrt(radius*radius - y * y),
			t1 = t - x,
			t2 = t + x;
		glm::vec2 intersection1 = pt1 + segmentUnit * t1;
		glm::vec2 intersection2 = pt1 + segmentUnit * t2;
		if (pointLiesOnSegment(intersection1, pt1, pt2))
		{
			intersectionPoints.push_back(intersection1);
			result = true;
		}
		if (pointLiesOnSegment(intersection2, pt1, pt2))
		{
			intersectionPoints.push_back(intersection2);
			result = true;
		}
		return result;
	}

	bool pointLiesOnSegment(glm::vec2 point, glm::vec2 pt1, glm::vec2 pt2)
	{
		glm::vec2 AC, AB;
		float Kac, Kab;
		AC = point - pt1;
		AB = pt2 - pt1;
		Kac = glm::dot(AC, AB);
		Kab = glm::dot(AB, AB);
		if (Kac >= 0 && Kac <= Kab)
		{
			return true;
		}
		return false;
	}

	void closestPtPointSegment(glm::vec2 point, glm::vec2 pt1, glm::vec2 pt2,/* float &interpolation,*/ glm::vec2 &closestPoint)
	{
		// Given segment ab and point c, computes closest point d on ab.
		// Also returns t for the position of d, d(t) = a + t*(b - a)
		glm::vec2 segment = pt2 - pt1;
		// Project c onto ab, computing parameterized position d(t) = a + t*(b – a)
		float interpolation = glm::dot(point - pt1, segment) / glm::dot(segment, segment);
		// If outside segment, clamp t (and therefore d) to the closest endpoint
		if (interpolation < 0.0f) interpolation = 0.0f;
		if (interpolation > 1.0f) interpolation = 1.0f;
		// Compute projected position from the clamped t
		closestPoint = pt1 + interpolation * segment;
	}

	bool test2DSegmentSegment(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d, vector<glm::vec2> &intersectionPoints)
	{
		// Test if segments ab and cd overlap. If they do, compute and return
		// interpolation value along ab and intersectionPoint

		// Sign of areas correspond to which side of ab points c and d are
		float a1 = Signed2DTriArea(a, b, d); // Compute winding of abd (+ or -)
		float a2 = Signed2DTriArea(a, b, c); // To intersect, must have sign opposite of a1
		// If c and d are on different sides of ab, areas have different signs
		if (a1 * a2 <= 0.0f) {
			// Compute signs for a and b with respect to segment cd
			float a3 = Signed2DTriArea(c, d, a); // Compute winding of cda (+ or -)
			// Since area is constant a1 - a2 = a3 - a4, or a4 = a3 + a2 - a1
			// float a4 = Signed2DTriArea(c, d, b); // Must have opposite sign of a3
			float a4 = a3 + a2 - a1;
			// Points a and b on different sides of cd if areas have different signs
			if (a3 * a4 <= 0.0f) {
				// Segments intersect. Find intersection point along L(t) = a + t * (b - a).
				// Given height h1 of an over cd and height h2 of b over cd,
				// t = h1 / (h1 - h2) = (b*h1/2) / (b*h1/2 - b*h2/2) = a3 / (a3 - a4),
				// where b (the base of the triangles cda and cdb, i.e., the length
				// of cd) cancels out.
				float interpolation = a3 / (a3 - a4);
				intersectionPoints.push_back(a + interpolation * (b - a));
				
				return true;
			}
		}
		return false;
	}

	float Signed2DTriArea(glm::vec2 a, glm::vec2 b, glm::vec2 c)
	{
		// Returns 2 times the signed triangle area. The result is positive if
		// abc is ccw, negative if abc is cw, zero if abc is degenerate.
		return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
	}

	bool pendulumColliding(Pendulum& pendulum, std::vector<Polygons>& polygons)
	{
		if (pendulum.hook)
		{
			float hang = pendulum.hang + pendulum.angularVelocity;
			float x = pendulum.hookLength * sin(hang) + pendulum.hposition.x;
			float y = pendulum.hookLength * cos(hang) + pendulum.hposition.y;
			glm::vec2 position = { x, y };
			for (int j = 0; j < polygons.size(); j++)
			{
				int pointsAmount = polygons[j].points.size();
				for (int i = 0; i < pointsAmount; i++)
				{
					glm::vec2 closestPoint;
					closestPtPointSegment(position, polygons[j].points[i], polygons[j].points[(i + 1) % pointsAmount], closestPoint);
					if (glm::length(closestPoint - position) < pendulum.radius)
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			struct Segment
			{
				glm::vec2 point1, point2;
			};
			struct CollisionData
			{
				Segment segment;
				glm::vec2 intersectionPoint;
			};
			vector<CollisionData>
				collisionData;
			Segment
				segment,
				segmentOffsetA,
				segmentOffsetB,
				segmentPerpendicular,
				stepSegment,
				stepSegmentPerpendicular,
				stepSegmentOffsetA,
				stepSegmentOffsetB;

			for (int j = 0; j < polygons.size(); j++)
			{
				int pointsAmount = polygons[j].points.size();
				for (int i = 0; i < pointsAmount; i++)
				{
					vector<glm::vec2>
						intersectionPoints;
					glm::vec2
						closestPoint,
						offset;
					float
						interpolation; //from 0 to 1
					bool
						test1 = false,
						test2 = false,
						test3 = false,
						test4 = false,
						test5 = false;

					segment.point1 = polygons[j].points[i];
					segment.point2 = polygons[j].points[(i + 1) % pointsAmount];
					offset = vectorPerpendicular(glm::normalize(segment.point2 - segment.point1)) * pendulum.radius;
					segmentOffsetA.point1 = segment.point1 + offset;
					segmentOffsetA.point2 = segment.point2 + offset;
					segmentOffsetB.point1 = segment.point1 - offset;
					segmentOffsetB.point2 = segment.point2 - offset;
					stepSegment.point1 = pendulum.position;
					stepSegment.point2 = pendulum.position + pendulum.velocity;

					test1 = test2DSegmentSegment(segmentOffsetA.point1, segmentOffsetA.point2, stepSegment.point1, stepSegment.point2, intersectionPoints);
					test2 = test2DSegmentSegment(segmentOffsetB.point1, segmentOffsetB.point2, stepSegment.point1, stepSegment.point2, intersectionPoints);
					test3 = circleSegmentIntersections(segment.point1, pendulum.radius, stepSegment.point1, stepSegment.point2, intersectionPoints);
					test4 = circleSegmentIntersections(segment.point2, pendulum.radius, stepSegment.point1, stepSegment.point2, intersectionPoints);

					if (test1 || test2 || test3 || test4)
					{
						glm::vec2 intersectionPoint = intersectionPoints[0];
						float minDistance = glm::distance2(intersectionPoint, stepSegment.point1);
						for (int i = 1; i < intersectionPoints.size(); i++)
						{
							const float newDistance = glm::distance2(intersectionPoints[i], stepSegment.point1);
							if (newDistance < minDistance)
							{
								minDistance = newDistance;
								intersectionPoint = intersectionPoints[i];
							}
						}
						collisionData.push_back({ segment, intersectionPoint });
					}
				}
			}
			if (collisionData.empty())
			{
				return false;
			}
			else
			{
				segment = collisionData[0].segment;
				glm::vec2 intersectionPoint = collisionData[0].intersectionPoint;
				float minDistance = glm::distance2(intersectionPoint, stepSegment.point1);
				for (int i = 1; i < collisionData.size(); i++)
				{
					const float newDistance = glm::distance2(collisionData[i].intersectionPoint, stepSegment.point1);
					if (newDistance < minDistance)
					{
						minDistance = newDistance;
						intersectionPoint = collisionData[i].intersectionPoint;
						segment = collisionData[i].segment;
					}
				}

				glm::vec2 closestPoint;
				//pendulum.position = intersectionPoint;
				closestPtPointSegment(intersectionPoint, segment.point1, segment.point2, closestPoint);
				pendulum.surfacePerpendicular = glm::normalize(intersectionPoint - closestPoint);
				return true;
			}
		}
	}

	void distanceAndAngleCircleSegment(glm::vec2 &position, float &radius, glm::vec2& point, glm::vec2& nextPoint, glm::vec2& surfacePerpendicular, float &distanceToCollider)
	{
		float
			x = position.x,
			y = position.y,
			r = radius,
			x1 = point.x,
			y1 = point.y,
			x2 = nextPoint.x,
			y2 = nextPoint.y,
			param = -1;
		glm::vec2 posToP1 = { x - x1, y - y1 };
		glm::vec2 p2ToP1 = { x2 - x1, y2 - y1 };
		float dot = glm::dot(posToP1, p2ToP1);
		float len_sq = glm::length2(p2ToP1);

		if (len_sq != 0)
			param = dot / len_sq;

		float xx, yy;

		if (param < 0) {
			xx = x1;
			yy = y1;
		}
		else if (param > 1) {
			xx = x2;
			yy = y2;
		}
		else {
			xx = x1 + param * p2ToP1.x;
			yy = y1 + param * p2ToP1.y;
		}

		float
			dx = x - xx,
			dy = y - yy;

		surfacePerpendicular = { dx, dy };
		distanceToCollider = sqrt(dx * dx + dy * dy);
	}

	bool pointInsidePolygon(glm::vec2 &pointPosition, Polygons &polygon)
	{
		int rayIntersections = 0;
		int pointsAmount = polygon.points.size();
		for (int i = 0; i < pointsAmount; i++)
		{
			if (getRayToLineSegmentIntersection(pointPosition, polygon.points[i], polygon.points[(i + 1) % pointsAmount]))
			{
				rayIntersections++;
			}
		}
		if (rayIntersections % 2 != 0)
		{
			return true;
		}
		return false;
	}

	glm::vec2 reflect(glm::vec2 v, glm::vec2 n) {
		return v - 2 * glm::dot(v, n) / glm::dot(n, n) * n;
	}

	float getRayToLineSegmentIntersection(glm::vec2 rayOrigin, glm::vec2 point1, glm::vec2 point2)
	{
		auto v1 = rayOrigin - point1;
		auto v2 = point2 - point1;
		auto v3 = glm::vec2(-1, 0);


		auto dot = glm::dot(v2, v3);
		if (abs(dot) < 0.000001)
			return NULL;

		float t1 = (v2.x *  v1.y - v1.x *  v2.y) / dot;
		float t2 = glm::dot(v1, v3) / dot;

		if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0))
			return t1;

		return NULL;
	}

	bool overlapping(float minA, float maxA, float minB, float maxB)
	{
		return minB <= maxA && minA <= maxB;
	}

	bool equal_floats(float a, float b)
	{
		float threshold = 1.0f / 8192.0f;
		return fabsf(a - b) < threshold;
	}

	glm::vec2 rotate_vector_90(glm::vec2 v) {
		glm::vec2 r;
		r.x = -v.y;
		r.y = v.x;
		return r;
	}

	bool parallel_vectors(glm::vec2 a, glm::vec2 b)
	{
		glm::vec2 na = rotate_vector_90(a);
		return equal_floats(0, glm::dot(na, b));
	}

	Range sort_range(Range r)
	{
		Range sorted = r;
		if (r.minimum > r.maximum)
		{
			sorted.minimum = r.maximum;
			sorted.maximum = r.minimum;
		}
		return sorted;
	}

	Range project_segment(glm::vec2 s1, glm::vec2 s2, glm::vec2 onto)
	{
		glm::vec2 ontoUnit = glm::normalize(onto);
		Range r;
		r.minimum = glm::dot(ontoUnit, s1);
		r.maximum = glm::dot(ontoUnit, s2);
		r = sort_range(r);
		return r;
	}

	bool on_one_side(glm::vec2 linePoint1, glm::vec2 linePoint2, glm::vec2 segmentPoint1, glm::vec2 segmentPoint2)
	{
		glm::vec2 d1 = linePoint1 - segmentPoint1;
		glm::vec2 d2 = linePoint2 - segmentPoint1;
		glm::vec2 n = rotate_vector_90(segmentPoint2);
		float a = glm::dot(n, d1) * glm::dot(n, d2) > 0;
		return a;
	}

	bool overlapping_ranges(Range a, Range b)
	{
		return overlapping(a.minimum, a.maximum, b.minimum, b.maximum);
	}

	bool line_segment_collide(glm::vec2 l1, glm::vec2 l2, glm::vec2 s1, glm::vec2 s2)
	{
		bool a = !on_one_side(l1, l2, s1, s2);
		return a;
	}

	bool segments_collide(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2)
	{
		glm::vec2 axisABase, axisADirection, axisBBase, axisBDirection;

		axisABase = a1;
		axisADirection = a2 - a1;
		if (on_one_side(axisABase, axisADirection, b1, b2)) {
			return false;
		}

		axisBBase = b1;
		axisBDirection = b2 - b1;
		if (on_one_side(axisBBase, axisBDirection, a1, a2)) {
			return false;
		}
		if (parallel_vectors(axisADirection, axisBDirection)) {
			Range rangeA = project_segment(a1, a2, axisADirection);
			Range rangeB = project_segment(b1, b2, axisADirection);
			return overlapping_ranges(rangeA, rangeB);
		}
		else return true;
	}

	glm::vec2 vectorPerpendicular(glm::vec2 vector)
	{
		return { -vector.y , vector.x };
	}

	float sq(float x)
	{
		return x * x;
	}

}


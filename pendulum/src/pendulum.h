#pragma once
#include "ofMain.h"
#include "stage.h"
#include "ofSoundPlayer.h"

class Pendulum{
	
public:
	Pendulum() = default;
	Pendulum(float x, float y, Stage& stage, bool pause);
	void update(Stage& stage, bool &pause, float translateX, float translateY);
	void draw(Stage& stage);
	bool hookCollidingPlatform(vector<Polygons> polygons);

	float counter = 0;
	
	// BOB
	glm::vec2 
		newPos, 
		position,
		prevPosition,
		tempPosition,
		velocity,
		acceleration,
		surfacePerpendicular;
		
	float
		angularVelocity,
		angularAcceleration,
		radius,
		stamina,
		life,
		friction,
		colliderAngle,
		CoR,
		hookedPlusVelocity,
		distanceToCollider;

	bool
		colliding,
		trig,
		hook,
		alive,
		floating,
		start;

	ofColor color;
	ofFbo fbo;

	// HARPOON
	glm::vec2
		hposition,
		hvel,
		hacc;

	float
		hookLength,
		hang,
		hangP,
		hfriction,
		hookRadius,
		shotVelocity;

	ofSoundPlayer
		hookSound,
		pendulumCrashSound[5],
		springSound;

	int r;

};
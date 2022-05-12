#pragma once
#include "ofMain.h"

class Gem
{
public:

	void setup();
	void update();
	void draw();

	void idleAniamtion();
	void outAnimation();

	glm::vec2 position, position0;
	float 
		counter,
		width, 
		height;

private:
	float scale;
	ofColor	baseColor, shadowColor, lightColor1, lightColor2;
	ofPath 
		path1, path2, path3, path4;
	glm::vec2
		centre, a, b, c, d, e, f,
		ina, inb, inc, ind, ine, inf,
		centre0, a0, b0, c0, d0, e0, f0,
		ina0, inb0, inc0, ind0, ine0, inf0;
}; 
//Hello
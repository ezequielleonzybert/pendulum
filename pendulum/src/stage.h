// stage created with Adobe Illustrator 24.1.2
// Layers must be named "walls", "platforms", "cameraRegions"

#pragma once
#include "ofMain.h"
#include "animations.h"
#include "gem.h"

struct Dimensions {
	float
		maxX, minX,
		maxY, minY,
		width,
		height;
};
struct Command {
	char command;
	std::vector<float> values;
};
struct Polygons {
	std::vector<glm::vec2> points;
	Dimensions dimensions;	
};
struct Cameras {
	std::vector<Polygons> polygons;
};

class Stage {

public:
	glm::vec2 p1, p2,
		gravity;
	float 
		width,
		height,
		polygonResolution = .0625; // must be 1/(2^n)
	std::vector<Polygons>
		wallPolygons,
		platformPolygons,
		springPolygons;
	ofPath
		walls,
		platforms,
		springs,
		camera1,
		camera2,
		camera3;
	Gem
		gem;
	std::vector<Gem>
		gems;
	std::vector<glm::vec2>
		gemsPositions;

	std::vector<Cameras> cameras;
	
	Stage() = default;
	Stage(std::string dir, glm::vec2 gravity);
	void setup();
	void update(glm::vec2 pendulumPosition, float pendulumRadius);
	void draw();
	std::vector<Command> getCommandsFromSvg(std::string &svg, std::string layer);
	std::vector<glm::vec2> getGemsPositions(std::string &svg);
	void fillCommands(char command, std::vector<Command> &commands, std::string &svgCopy, int j, int &i);
	void fillColliders(std::vector<Command> &command, std::vector<Polygons> &polygons, ofPath *path = nullptr);
	void getDimensions(vector<Polygons> &polygons);
	string copySvgFile(string dir);
};
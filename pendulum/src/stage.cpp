#include "stage.h"
#include "collisions.h"

void Stage::setup()
{
	float gemsAmount = gemsPositions.size();
	gems.reserve(gemsAmount);
	gem.setup();
	for (int i = 0; i < gemsAmount; i++)
	{
		gem.position = gemsPositions[i];
		gem.position0 = gem.position;
		gem.counter += 20;
		gems.emplace_back(gem);
	}
}

void Stage::update(glm::vec2 pendulumPosition, float pendulumRadius)
{
	for (int i = 0; i < gems.size(); i++)
	{
		if (!collisions::test2DCircleRectangle(pendulumPosition, pendulumRadius, gems[i].position, gems[i].width, gems[i].height))
		{
			gems[i].update();
		}
		else
		{
			std::iter_swap(gems.begin() + i, gems.end() - 1);
			gems.pop_back();
		}
	}
}

void Stage::draw() {
	walls.draw();
	platforms.draw();
	springs.draw();

	ofSetLineWidth(1);
	ofSetColor(0);
	ofDrawLine(p1, p2);

	for (Gem &g : gems)
	{
		g.draw();
	}
}

Stage::Stage(std::string dir, glm::vec2 gravity) {

	this->gravity = gravity;
	string svg = copySvgFile(dir);

	std::vector<Command> wallCommands = getCommandsFromSvg(svg, "walls");
	std::vector<Command> platformCommands = getCommandsFromSvg(svg, "platforms");
	std::vector<Command> springsCommands = getCommandsFromSvg(svg, "springs");

	std::vector<Command> camera1Commands = getCommandsFromSvg(svg, "camera1");
	std::vector<Command> camera2Commands = getCommandsFromSvg(svg, "camera2");
	std::vector<Command> camera3Commands = getCommandsFromSvg(svg, "camera3");

	gemsPositions = getGemsPositions(svg);

	fillColliders(wallCommands, wallPolygons, &walls);
	fillColliders(platformCommands, platformPolygons, &platforms);
	fillColliders(springsCommands, springPolygons, &springs);

	cameras.push_back(Cameras());
	fillColliders(camera1Commands, cameras[0].polygons, &camera1);
	cameras.push_back(Cameras());
	fillColliders(camera2Commands, cameras[1].polygons, &camera2);
	cameras.push_back(Cameras());
	fillColliders(camera3Commands, cameras[2].polygons, &camera3);

	getDimensions(wallPolygons);
	getDimensions(cameras[0].polygons);
	getDimensions(cameras[1].polygons);
	getDimensions(cameras[2].polygons);

	walls.setColor(ofColor(255));
	platforms.setColor(ofColor(100,190,255));
	springs.setColor(ofColor(255, 255, 50));
	camera1.setColor(ofColor(0, 255, 0, 100));
	camera2.setColor(ofColor(0, 0, 255, 100));
	camera3.setColor(ofColor(255, 0, 0, 100));
}

void Stage::fillColliders(std::vector<Command> &command, std::vector<Polygons> &polygons, ofPath *path) {

	glm::vec2 lastPoint = { 0, 0 };
	glm::vec2 lastControlPoint = { 0, 0 };
	int j = -1;

	for (Command c : command) {
		switch (c.command)
		{
		case 'M': {
			j++;
			polygons.push_back(Polygons());
			glm::vec2 p1 = { c.values[0], c.values[1] };
			(*path).moveTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1;
			break;
		}
		case 'L': {
			glm::vec2 p1 = { c.values[0], c.values[1] };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1;
			break;
		}
		case 'l': {
			glm::vec2 p1 = { lastPoint.x + c.values[0], lastPoint.y + c.values[1] };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1;
			break;
		}
		case 'H': {
			glm::vec2 p1 = { c.values[0], lastPoint.y };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1;
			break;
		}
		case 'h': {
			glm::vec2 p1 = { lastPoint.x + c.values[0], lastPoint.y };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1; // can't simplify? lastControlPoint = lastPoint = p1;
			break;
		}
		case 'V': {
			glm::vec2 p1 = { lastPoint.x, c.values[0] };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1;
			break;
		}
		case 'v': {
			glm::vec2 p1 = { lastPoint.x, lastPoint.y + c.values[0] };
			(*path).lineTo(p1);
			polygons[j].points.push_back(p1);
			lastControlPoint = lastPoint = p1 ;
			break;
		}
		case 'C': {
			glm::vec2
				p1 = { lastPoint.x , lastPoint.y },
				c1 = { c.values[0], c.values[1] },
				c2 = { c.values[2], c.values[3] },
				p2 = { c.values[4], c.values[5] };
			(*path).bezierTo(c1, c2, p2);
			for (float t = polygonResolution; t <= 1; t += polygonResolution) {
				glm::vec2 bezierPoint = ofBezierPoint(p1, c1, c2, p2, t);
				polygons[j].points.push_back(bezierPoint);
			}
			lastControlPoint = c2;
			lastPoint = p2;
			break;
		}
		case 'c': {
			glm::vec2
				p1 = { lastPoint.x , lastPoint.y },
				c1 = { lastPoint.x + c.values[0], lastPoint.y + c.values[1] },
				c2 = { lastPoint.x + c.values[2], lastPoint.y + c.values[3] },
				p2 = { lastPoint.x + c.values[4], lastPoint.y + c.values[5] };
			(*path).bezierTo(c1, c2, p2);
			for (float t = polygonResolution; t <= 1; t += polygonResolution) {
				glm::vec2 bezierPoint = ofBezierPoint(p1, c1, c2, p2, t);
				polygons[j].points.push_back(bezierPoint);
			}
			lastControlPoint = c2;
			lastPoint = p2;
			break; 
		}
		case 'S': {
			glm::vec2
				p1 = { lastPoint.x, lastPoint.y }, 
				c1 = { lastPoint.x + (lastPoint.x - lastControlPoint.x),	lastPoint.y + (lastPoint.y - lastControlPoint.y) },// possibly wrong
				c2 = { c.values[0], c.values[1] },
				p2 = { c.values[2], c.values[3] };
			(*path).bezierTo(c1, c2, p2);
			for (float t = polygonResolution; t <= 1; t += polygonResolution) {
				glm::vec2 bezierPoint = ofBezierPoint(p1, c1, c2, p2, t);
				polygons[j].points.push_back(bezierPoint);
			}
			lastControlPoint = c2;
			lastPoint = p2;
			break;
		}
		case 's': {
			glm::vec2
				p1 = { lastPoint.x, lastPoint.y },
				c1 = { lastPoint.x + lastPoint.x - lastControlPoint.x, lastPoint.y + lastPoint.y - lastControlPoint.y },
				c2 = { lastPoint.x + c.values[0], lastPoint.y + c.values[1] },
				p2 = { lastPoint.x + c.values[2], lastPoint.y + c.values[3] };
			(*path).bezierTo(c1, c2, p2);
			for (float t = polygonResolution; t <= 1; t += polygonResolution) {
				glm::vec2 bezierPoint = ofBezierPoint(p1, c1, c2, p2, t);
				polygons[j].points.push_back(bezierPoint);
			}
			lastControlPoint = c2;
			lastPoint = p2;
			break;
		}
		case 'R': {
			j++;
			polygons.push_back(Polygons());
			float 
				x = c.values[0],
				y = c.values[1],
				w = c.values[2],
				h = c.values[3];

			glm::vec2 p[4];
				p[0] = { x, y },
				p[1] = { x + w , y},
				p[2] = { x + w, y + h },
				p[3] = { x, y + h};
				(*path).moveTo(p[0]);
				(*path).lineTo(p[1]);
				(*path).lineTo(p[2]);
				(*path).lineTo(p[3]);
			for(int i = 0; i < 4; i++) {
				polygons[j].points.push_back(p[i]);
			}
			break;
		}
		case 'P':
		{
			j++;
			polygons.push_back(Polygons());
			glm::vec2 p;
			for (int i = 0; i < c.values.size(); i += 2)
			{
				p = { c.values[i], c.values[i + 1] };
				polygons[j].points.push_back(p);
				if (i == 0)
				{
					(*path).moveTo(p);
				}
				else 
				{
					(*path).lineTo(p);
				}
			}

			break;
		}
		default:
			break;
		}
	}
	if(path) (*path).close();
}

std::vector<Command> Stage::getCommandsFromSvg(std::string &svg, std::string layer) {

	std::string svgCopy;
	std::string toFind;
	int filePointer;
	std::vector<Command> commands;
	int commandCounter = 0;
	int i;

	svgCopy = svg;
	toFind = "<g id=\"" + layer + "\">";
	if ((filePointer = svgCopy.find(toFind)) != std::string::npos)
	{
		svgCopy.erase(0, filePointer + toFind.length());
		toFind = "</g>";
		int close = svgCopy.find(toFind);
		svgCopy.erase(close, svgCopy.length());

		// Paths
		char chars[12] = { 'M', 'm', 'L', 'l', 'C', 'c', 'S', 's', 'H', 'h', 'V', 'v' };
		toFind = "<path d=\"";
		while ((filePointer = svgCopy.find(toFind)) != std::string::npos)
		{
			i = filePointer + toFind.length();
			while (svgCopy[i] != 'z')
			{
				if (find(begin(chars), end(chars), svgCopy[i]) != end(chars))
				{
					fillCommands(svgCopy[i], commands, svgCopy, commandCounter, i);
					commandCounter++;
				}
				else
				{
					i++;
				}
			}
			svgCopy.erase(filePointer, i);
		}

		// Rectangles
		string value;
		toFind = "<rect x=\"";
		while ((filePointer = svgCopy.find(toFind)) != std::string::npos)
		{
			i = filePointer + toFind.length();
			commands.push_back(Command());
			commands[commandCounter].command = 'R';
			while (svgCopy[i] != '/')
			{
				while (isdigit(svgCopy[i]) || svgCopy[i] == '.')
				{
					value += svgCopy[i];
					i++;
				}
				if (value != "")
				{
					commands[commandCounter].values.push_back(stof(value));
					value = "";
				}
				else
				{
					i++;
				}
			}
			commandCounter++;
			svgCopy.erase(filePointer, i - 2); // subtracting just i erase part of the next toFind string.
		}

		// Polygon points
		toFind = "<polygon points=\"";
		while ((filePointer = svgCopy.find(toFind)) != std::string::npos)
		{
			i = filePointer + toFind.length();
			commands.push_back(Command());
			commands[commandCounter].command = 'P';
			while (svgCopy[i] != '/')
			{
				while (isdigit(svgCopy[i]) || svgCopy[i] == '.')
				{
					value += svgCopy[i];
					i++;
				}
				if (value != "")
				{
					commands[commandCounter].values.push_back(stof(value));
					value = "";
				}
				else
				{
					i++;
				}
			}
			commandCounter++;
			svgCopy.erase(filePointer, i - 2);
		}
	}

	//for(Command c : commands) {
	//	cout << c.command << " ";
	//	for (float f : c.values) {
	//		cout << f << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	return commands;
}

void Stage::fillCommands(char command, std::vector<Command> &commands, std::string &svgCopy, int j, int &i) {
	commands.emplace_back(Command());
	commands[j].command = command;

	std::string n = "";
	int pointCount = 0;
	i++;
	while (isdigit(svgCopy[i]) || svgCopy[i] == ',' || svgCopy[i] == '.' || svgCopy[i] == '-') {

		if ((svgCopy[i] == '-' && n == "") || isdigit(svgCopy[i]) || svgCopy[i] == '.') {
			n += svgCopy[i];
		}
		else {
			commands[j].values.emplace_back(stof(n));
			n = "";
		}
		if ((svgCopy[i] != '-' && n != "" && svgCopy[i]) || svgCopy[i] == ',' || n == "-") i++;
	}
	commands[j].values.emplace_back(stof(n));
	n = "";
}

void Stage::getDimensions(vector<Polygons> &polygons) {

	//float width, height,
	//	maxX = polygons[0].points[0].x,
	//	maxY = polygons[0].points[0].y,
	//	minX = polygons[0].points[0].x,
	//	minY = polygons[0].points[0].y;

	for (Polygons &polys : polygons) {

		float 
			width, height,
			maxX = polys.points[0].x,
			maxY = polys.points[0].y,
			minX = polys.points[0].x,
			minY = polys.points[0].y;

		for (glm::vec2 points : polys.points) {

			if (points.x > maxX) maxX = points.x;
			else if (points.x < minX) minX = points.x;

			if (points.y > maxY) maxY = points.y;
			else if (points.y < minY) minY = points.y;
		}

		polys.dimensions.maxX = maxX;
		polys.dimensions.maxY = maxY;
		polys.dimensions.minX = minX;
		polys.dimensions.minY = minY;
		polys.dimensions.width = maxX - minX;
		polys.dimensions.height = maxY - minY;
	}
}

std::vector<glm::vec2> Stage::getGemsPositions(std::string &svg)
{
	std::vector<glm::vec2> result;
	float tile = 57.6;
	std::string svgCopy;
	std::string toFind;
	int filePointer;
	int i;

	svgCopy = svg;
	toFind = "<g id=\"gems\">";
	if ((filePointer = svgCopy.find(toFind)) != std::string::npos)
	{
		svgCopy.erase(0, filePointer + toFind.length());
		toFind = "</g>";
		int close = svgCopy.find(toFind);
		svgCopy.erase(close, svgCopy.length());
		string value;
		vector<float> n;
		toFind = "<rect x=\"";
		while ((filePointer = svgCopy.find(toFind)) != std::string::npos)
		{
			i = filePointer + toFind.length();
			while (svgCopy[i] != 'w')
			{
				while (isdigit(svgCopy[i]) || svgCopy[i] == '.')
				{
					value += svgCopy[i];
					i++;
				}
				if (value != "")
				{
					n.push_back(stof(value)+tile/2.f);
					value = "";
				}
				else
				{
					i++;
				}
			}
			svgCopy.erase(filePointer, i - 2);
		}
		for (int j = 0; j < n.size(); j += 2)
		{
			result.push_back({ n[j], n[j + 1] });
		}
	}
	return result;
}

string Stage::copySvgFile(string dir)
{
	ifstream file;
	std::string svg;

	file.open(dir, ios::in);

	if (file.fail()) {
		cout << "couldn't open stage svg file" << endl;
	}

	else {
		while (!file.eof()) {
			char p = file.peek();
			if (p != '\n' && p != '\t') {
				svg += file.get();
			}
			else {
				file.ignore();
			}
		}
	}
	file.close();

	return svg;
}
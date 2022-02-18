#pragma once
#include "ofMain.h"

struct String
{
	std::string string;
	ofTrueTypeFont data;
	ofVec2f position;
	float width;
	float height;
	bool draw;
	float scale;

	String(){}
	String(std::string string, float fontSize)
	{
		data.loadFont("fonts/Exo-ExtraBoldItalic.ttf", fontSize,true,false,true);
		this->string = string;
		width = data.stringWidth(string);
		height = data.stringHeight(string);
		draw = false;
		scale = 1;
	};
};

struct Audio
{
	ofSoundPlayer sound;
	bool played = false;
};

class Animations {

public:
	void setup(int screenWidth, int screenHeight);
	void update(bool& inputLock);
	void draw();
	void drawString(String& str);
private:

	float
		counterA,
		interpolationA,
		interpolationB,
		screenWidth,
		screenHeight;

	String 
		ready,
		g, 
		o;

	bool readyGoFlag;

	ofVec2f targetPosition;

	Audio readyAudio, goAudio;
};
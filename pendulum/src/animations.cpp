#include "animations.h"

void Animations::setup(int screenWidth, int screenHeight)
{	
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	ready = String("READY", 100);
	g = String("G",200);
	o = String("O",200);

	readyAudio.sound.load("audio/ready.ogg");
	goAudio.sound.load("audio/go.ogg");

	readyGoFlag = true;

	counterA = 0;
	interpolationA = -1;
	interpolationB = -1;
}

void Animations::update(bool& inputLock)
{
	counterA++;

	if (readyGoFlag)
	{
		inputLock = true;
		if (counterA > 50)
		{
			if (interpolationA < 1) {
				ready.draw = true;
				ready.position.x = pow(interpolationA, 5) * screenWidth;
				interpolationA += .015;
				if (!readyAudio.played)
				{
					readyAudio.played = true;
					readyAudio.sound.play();
				}
			}
			else
			{
				ready.draw = false;
				if (interpolationB < 0)
				{
					g.draw = true;
					o.draw = true;
					g.position.x = pow(interpolationB, 3) * screenWidth - 100;
					o.position.x = pow(-interpolationB, 3) * screenWidth + 100;
					interpolationB += .019;
				}
				else
				{
					if (g.scale < 15)
					{
						g.scale = o.scale += .2;
						g.position.x -= .5;
						o.position.x += .5;
						if (!goAudio.played)
						{
							goAudio.played = true;
							goAudio.sound.play();
						}
					}
					else
					{
						g.draw = false;
						o.draw = false;
						readyGoFlag = false;
						inputLock = false;
					}
				}
			}
		}
	}
}

void Animations::draw()
{
	ofPushMatrix();
	ofTranslate(screenWidth / 2, screenHeight / 2);

	drawString(ready);
	drawString(g);
	drawString(o);

	ofPopMatrix();
}

void Animations::drawString(String& str)
{
	if (str.draw) {
		ofPushMatrix();
		ofScale(str.scale);
		str.data.drawStringAsShapes(str.string, str.position.x - str.width / 2, str.position.y + str.height / 2);
		ofPopMatrix();
	}
}
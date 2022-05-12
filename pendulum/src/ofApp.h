#pragma once
#include "ofMain.h"
#include "stage.h"
#include "pendulum.h"
#include "camera.h"
#include "animations.h"
#include "gem.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mousePressed(int x, int y, int button);

		int windowWidth;
		int windowHeight;

		float friction;
		Stage stage;
		Pendulum pendulum;
		Camera camera;

		bool
			keyRelease,
			pause,
			inputLock,
			nextFrame;

		ofSoundPlayer music;

		Animations animations;
};

/*void mouseMoved(int x, int y );
void mouseDragged(int x, int y, int button);
void mouseReleased(int x, int y, int button);
void mouseEntered(int x, int y);
void mouseExited(int x, int y);
void windowResized(int w, int h);
void dragEvent(ofDragInfo dragInfo);
void gotMessage(ofMessage msg);*/

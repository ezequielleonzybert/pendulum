#include "ofApp.h"
#include "collisions.h"

glm::vec2 closestPoint;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetCircleResolution(50);
	ofBackground(200);

	windowWidth = 1024;
	windowHeight = 576;
	ofSetWindowShape(windowWidth, windowHeight);

	stage = Stage("bin/data/stage01.svg", { 0, .5 });
	pendulum = Pendulum(3900, 980, stage, pause); //250,250
	camera.setup(pendulum.position, windowWidth, windowHeight);

	music.load("audio/stage1.mp3");
	//music.play();

	animations.setup(windowWidth, windowHeight);

	keyRelease = true;
}

//--------------------------------------------------------------
void ofApp::update() {
	if (!pause)
	{
		pendulum.update(stage, pause, camera.displayPos.x, camera.displayPos.y);
		//animations.update(inputLock);
	}
	if (nextFrame)
		pause = true;
}
//--------------------------------------------------------------
void ofApp::draw(){
	auto start = chrono::steady_clock::now();

	ofPushMatrix();
	ofScale(camera.displayScale); 
	camera.movement(pendulum, stage.cameras, stage.wallPolygons[0].dimensions, windowWidth/camera.displayScale, windowHeight/camera.displayScale); ///wallPolygons could be more than 1 polygon
	stage.draw();
	pendulum.draw(stage);
    //animations.draw();
	//camera.draw();

	ofSetLineWidth(2);
	ofSetColor(0);
	ofDrawLine(stage.p1, stage.p2);

	ofPopMatrix();

	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << "draw() " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
}

void ofApp::keyPressed(int key) {
	if (!inputLock)
	{
		if (key == OF_KEY_RIGHT && keyRelease) {
			keyRelease = false;
			pendulum.trig = true;
			pendulum.hvel.x = 12;
			pendulum.hvel.y = -12;
		}
		if (key == OF_KEY_LEFT && keyRelease) {
			keyRelease = false;
			pendulum.trig = true;
			pendulum.hvel.x = -12;
			pendulum.hvel.y = -12;
		}
	}
	if (key == OF_KEY_RETURN) {
		pause = !pause;
		nextFrame = false;
	}
	if (key == OF_KEY_BACKSPACE) {
		pause = !pause;
		nextFrame = true;
	}
	if (key == OF_KEY_DEL) {
		pendulum = Pendulum(200, 250, stage, pause);
	}
	if (key == 'q')
	{
		pendulum.velocity.x = 50;
		pendulum.floating = false;
	}
}

void ofApp::keyReleased(int key) {
	if (key == OF_KEY_RIGHT || key == OF_KEY_LEFT) {
		keyRelease = true;
		pendulum.trig = false;
		if (pendulum.hook) {
			pendulum.hook = false;
			pendulum.velocity.x = pendulum.angularVelocity * cos(pendulum.hang) * pendulum.hookLength;
			pendulum.velocity.y = -pendulum.angularVelocity * sin(pendulum.hang) * pendulum.hookLength;
		}
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	pendulum = Pendulum(x/camera.displayScale - camera.displayPos.x, y/camera.displayScale - camera.displayPos.y, stage, pause);
}
//
////--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y ){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button){
//
//}
//
////-------------------------------------------------------------
//
////--------------------------------------------------------------
//void ofApp::mouseReleased(int x, int y, int button){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseEntered(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::mouseExited(int x, int y){
//
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h){
//
//}
//
////--------------------------------------------------------------
//void ofApp::gotMessage(ofMessage msg){
//
//}
//
////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo dragInfo){ 
//
//}

#include "pendulum.h"
#include "collisions.h"

Pendulum::Pendulum(float x, float y, Stage& stage, bool pause) {
	position = { x, y };
	velocity = { 0, 0 };
	acceleration = stage.gravity;
	prevPosition = { 0, 0 };
	tempPosition = position;
	angularVelocity = 0;
	friction = .999; //.99
	CoR = 0.7; //.8
	radius = 25;
	stamina = 50;
	start = true;
	trig = false;
	colliding = false;
	hook = false;
	alive = true;
	floating = true;
	life = 1;
	hposition = { position.x, position.y };
	hvel = { 0, 0 };
	hacc = stage.gravity / 2;
	hookLength = 0;
	hookRadius = radius / 3;
	color = ofColor(255, 100, 100);
	shotVelocity = 17;
	distanceToCollider = 1000000 ; // any value greater than radius
	hookedPlusVelocity = 1.3;

	hookSound.load("audio/hookSound.wav");
	hookSound.setVolume(.5);
	springSound.load("audio/springSound.ogg");
	for (int i = 0; i < std::size(pendulumCrashSound); i++)
	{
		pendulumCrashSound[i].load("audio/pendulumCrashSound" + to_string(i) + ".ogg"); 
	}
}

void Pendulum::update(Stage& stage, bool &pause, float translateX, float translateY) {
	if (life < 1 && alive) 
	{
		life += .0005;
	}
	if (floating) 
	{
		position.y += sin(counter)*2;
		hposition.y = position.y;
		counter+= .1;
		floating = !trig;
	}
	else 
	{	
		//Falling Physics
		if (!hook) {
			/// pendulum collisions
			velocity += acceleration;
			bool wallColliding;
			bool springColliding;
			do
			{
				wallColliding = collisions::pendulumColliding(*this, stage.wallPolygons);
				springColliding = collisions::pendulumColliding(*this, stage.springPolygons);
				if (wallColliding)
				{
					velocity = glm::reflect(velocity, surfacePerpendicular) * CoR;
					float velocityMagnitude = glm::length2(velocity);
					if (velocityMagnitude > 15)
					{
						float volume = ofClamp(velocityMagnitude*0.01, 0, 1);
						int new_r = rand() % 5;
						if (new_r == r)
						{
							new_r = (new_r + 1) % 5;
						}
						r = new_r;
						pendulumCrashSound[new_r].setVolume(volume);
						pendulumCrashSound[new_r].play();
					}
				} 
				else if (springColliding)
				{
					velocity = glm::reflect(velocity, surfacePerpendicular);
					velocity = glm::normalize(velocity + surfacePerpendicular) * 20;
					springSound.play();
				}
			} while (wallColliding || springColliding);


			//while (collisions::pendulumColliding(*this, stage.wallPolygons))
			//{
			//	using namespace collisions;
			//	//ofVec2f vy = (radius - distanceToCollider) * surfacePerpendicular;
			//	//float angle = (-velocity).angleRad(surfacePerpendicular);
			//	//float dx = -tan(angle) * vy.length();
			//	//ofVec2f newPosition = 2 * dx * surfacePerpendicular.getPerpendicular();
			//	//position += velocity + newPosition;
			//	//velocity = reflect(velocity, surfacePerpendicular);
			//	//velocity *= restitution;
			//	//velocity += surfacePerpendicular * 0.2; // to avoid error
			//	velocity = glm::reflect(velocity, surfacePerpendicular) * CoR;
			//	float velocityMagnitude = glm::length2(velocity);
			//	if (velocityMagnitude > 15)
			//	{
			//		float volume = ofClamp(velocityMagnitude*0.01, 0, 1);
			//		int new_r = rand() % 5;
			//		if (new_r == r)
			//		{
			//			new_r = (new_r + 1) % 5;
			//		}
			//		r = new_r;
			//		pendulumCrashSound[new_r].setVolume(volume);
			//		pendulumCrashSound[new_r].play();
			//	}
			//}

			///// Springs
			//while (collisions::pendulumColliding(*this, stage.springPolygons)) {
			//	velocity = glm::reflect(velocity, surfacePerpendicular); 
			//	velocity = glm::normalize(velocity) * 20;
			//	springSound.play();
			//}

			prevPosition = position;
			position += velocity;

			/// Hook:
			if (!trig) {
				hposition = position;
				hookLength = 0;
			}
			else {
				hvel.y += hacc.y;
				hposition += hvel;
				hookLength = sqrt(pow(position.x - hposition.x, 2) + pow(position.y - hposition.y, 2));
				/// Hook collisions:
				if (hookCollidingPlatform(stage.platformPolygons) && hookLength > 50) {
					hook = true;
					hang = atan2(position.x - hposition.x, position.y - hposition.y);
					hangP = atan2(position.x - prevPosition.x, position.y - prevPosition.y);
					float velMag = sqrt(pow(velocity.y, 2) + pow(velocity.x, 2));
					angularVelocity = velMag * sin(hangP - hang) / hookLength * hookedPlusVelocity;
					hookSound.play();
				}
			}
		}
		else
		{
			// Hooked Physics
			angularAcceleration = -stage.gravity.y * sin(hang) / hookLength;
			angularVelocity += angularAcceleration;
			/// Avatar collisions
			/// walls
			if (collisions::pendulumColliding(*this, stage.springPolygons))
			{
				angularVelocity = -ofSign(angularVelocity) * 0.06;
				springSound.play();
			}
			else if (collisions::pendulumColliding(*this, stage.wallPolygons)) 
			{
				angularVelocity = -angularVelocity;
				float velocityMagnitude = glm::length2(velocity);
				if (velocityMagnitude > 15)
				{
					float volume = ofClamp(velocityMagnitude*0.01, 0, 1);
					int new_r = rand() % 5;
					if (new_r == r)
					{
						new_r = (new_r + 1) % 5;
					}
					r = new_r;
					pendulumCrashSound[new_r].setVolume(volume);
					pendulumCrashSound[new_r].play();
				}
			}
			hang += angularVelocity;
			position.x = hookLength * sin(hang) + hposition.x;
			position.y = hookLength * cos(hang) + hposition.y;
		}
	}
}

void Pendulum::draw(Stage& stage) {
	ofScale(1);
	ofSetColor(color);
	ofDrawCircle(position.x, position.y, radius);
	ofDrawCircle(hposition.x, hposition.y, hookRadius);
}

bool Pendulum::hookCollidingPlatform(vector<Polygons> polygons)
{
	for (Polygons polygon : polygons)
	{
		if (collisions::pointInsidePolygon(hposition, polygon))
		{
			return true;
		}
	}
	return false;
}
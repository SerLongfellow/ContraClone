#include "Infantry.h"

Infantry::Infantry(){
	objectType = "Infantry";
	timeSinceBulletShot = 2.0f;
}

Infantry::~Infantry(){

}

void Infantry::Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd){

	//This is so the main loop doesn't try to delete it every frame
	if(isDead)
		wasDead = true;
	else{
		timeSinceBulletShot += gameTime;

		//If time elapsed since the last bullet shot is 10 milliseconds
		if(timeSinceBulletShot > 3.0f){

			objectToAdd newBullet;

			newBullet.objectType = "EnemyBullet";

			b2Vec2 pos(body->GetPosition());

			newBullet.position.x = PW2RW(pos.x) - 15.0f;
			newBullet.position.y = PW2RW(pos.y) - 8.0f;
			newBullet.position.z = 0.0f;

			newBullet.angle = body->GetAngle();

			b2Vec2 vel(-3.3f,0.0f);

			newBullet.vel = vel;

			objectsToAdd->push_back(newBullet);

			timeSinceBulletShot = 0.0f;
		}
	}
}
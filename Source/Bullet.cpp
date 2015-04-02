#include "Bullet.h"

Bullet::Bullet(){
	timeAlive = 0.0f;
}

Bullet::~Bullet(){

}

void Bullet::Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd){
	timeAlive += gameTime;

	if(timeAlive > 2.0f)
		isDead = true;
}
#ifndef BULLET_H
#define BULLET_H

#include <map>
#include "Actor.h"

class Bullet: public Actor{

	friend class BulletFactory;

public:
	Bullet();
	~Bullet();

	void Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd);
	//void Draw(float gameTime, Camera* camera);
	
private:
	float timeAlive;
};

#endif
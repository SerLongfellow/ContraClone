#ifndef INFANTRY_H
#define INFANTRY_H

#include "Actor.h"

class Infantry: public Actor{

	friend class InfantryFactory;

public:
	Infantry();
	~Infantry();

	void Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd);
	//void Draw(float gameTime, Camera* camera);
	
private:

};

#endif
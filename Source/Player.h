#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include "Actor.h"
#include "Animation.h"
#include <Windows.h>

class Player: public Actor{

	friend class PlayerFactory;
	friend class ContactListener;

public:
	Player();
	~Player();

	void Update(float gameTime, InputDevice* inputDevice, ActorVector* objectsToAdd);
	void Draw(float gameTime, Camera* camera);
	
private:
	float timeSinceJumped, invincibleTime, timeSinceUpdateAnim, timeSinceLanded;
	float cameraOffsetX;

	int lastState;

	Animation* animation;
	RECT srcRect;
};

#endif
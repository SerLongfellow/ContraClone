#ifndef ACTOR_H
#define ACTOR_H

#include <d3dx9.h>
#include <Box2D\Box2D.h>
#include <string>
#include "Sprite.h"
#include "Definitions.h"
#include "GameFunctions.h"
#include "SoundMap.h"

class Actor{
	friend class ActorFactory;
	friend class ActorComponent;
	friend class ContactListener;

public:
	Actor();
	~Actor();

	//zIndex for determining in which order to render objects
	virtual bool Initialize(float x, float y, float zIndex, float angularVelocity, float forceMag, b2Body* body, std::string objectID, Sprite* sprite);
	bool IsInitialized();
	virtual void Update(float gameTime, InputDevice* inputDevice, ActorVector* objectToAdd) = 0;
	virtual void Draw(float gameTime, Camera* camera);
	void CheckCollisions(ActorMap* objectContainer, float gameTime);

	//Getters and setters
	void SetPosition(float x, float y);
	void SetAngle(float angle);
	void SetSprite(Sprite* sprite);
	void SetDead(bool cond);
	void SetFixtureDef(b2FixtureDef* shapefd);
	void SetSounds(SoundMap* sounds);

	bool CheckIfDead();
	bool CheckIfWasDead();

	std::string GetObjectID();
	std::string GetObjectType();
	b2Body* GetBody();
	b2FixtureDef* GetFixture();
	int GetHealth();

protected:

	Sprite* sprite;
	b2Body* body;

	b2FixtureDef* shapefd;

	Actor* compoundActor;

	float mass, forceToApply, forceMagnitude;
	float coeff_restitution, coeff_friction;
	float angularVelocity;
	float radius;
	bool initialized;
	bool isDead, wasDead;
	std::string objectID, objectType;

	int state;
	int health;

	float timeSinceBulletShot;

	SoundMap* sounds;
};

#endif
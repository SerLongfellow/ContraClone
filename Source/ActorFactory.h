#ifndef ACTOR_FACTORY_H
#define ACTOR_FACTORY_H

#include <string>
#include "Actor.h"
#include "ArtAssetLibrary.h"
#include "ThirdParty\tinyxml\tinyxml.h"
#include "ThirdParty\tinyxml\tinystr.h"

class ActorFactory{

public:
	ActorFactory();
	~ActorFactory();
	
	virtual Actor* CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world) = 0;
	void SetPhysics(TiXmlElement* data);//, float width, float height);

protected:
	int count;
	float linearForce, angularForce;
	float linearDamping, angularDamping;

	b2Body* body;
	b2CircleShape circleShape;
	b2PolygonShape polygonShape;
	b2EdgeShape edgeShape[4];
	b2BodyDef bd;
	b2FixtureDef shapefd;

	int health;
};


#endif
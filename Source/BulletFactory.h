#ifndef BULLET_FACTORY_H
#define BULLET_FACTORY_H

#include "Bullet.h"
#include "ActorFactory.h"
#include <sstream>

class BulletFactory: public ActorFactory{

public:
	BulletFactory();
	~BulletFactory();

	Bullet* CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world);


};

#endif
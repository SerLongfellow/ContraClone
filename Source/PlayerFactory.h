#ifndef PLAYER_FACTORY_H
#define PLAYER_FACTORY_H

#include "Player.h"
#include "ActorFactory.h"
#include <sstream>

class PlayerFactory: public ActorFactory{

public:
	PlayerFactory();
	~PlayerFactory();

	Player* CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world);


};

#endif
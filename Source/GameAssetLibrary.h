#ifndef GAME_ASSET_LIBRARY_H
#define GAME_ASSET_LIBRARY_H

#include <map>
#include <string>
#include "Actor.h"
#include "ActorFactory.h"
#include "InfantryFactory.h"
#include "BlockFactory.h"
#include "BulletFactory.h"
#include "PlayerFactory.h"
#include "Sprite.h"

class GameAssetLibrary{

public:
	GameAssetLibrary();
	~GameAssetLibrary();

	void Insert(std::string factoryType);
	void Remove(std::string factoryType);
	void Clear();
	ActorFactory* Find(std::string factoryType);

	Actor* CreateObject(std::string objectType, ArtAssetLibrary* sprites, TiXmlElement* objectData, b2World* world);
	
private:
	std::map<std::string, ActorFactory*> factoryLibrary;
	std::map<std::string, ActorFactory*>::iterator miter;

};

#endif
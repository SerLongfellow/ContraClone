#include "GameAssetLibrary.h"

GameAssetLibrary::GameAssetLibrary(){

}

GameAssetLibrary::~GameAssetLibrary(){
	factoryLibrary.clear();
}

void GameAssetLibrary::Insert(std::string factoryType){

	if(factoryType == "Infantry"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("Infantry", new InfantryFactory));
	}
	else if(factoryType == "TopBlock"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("TopBlock", new BlockFactory));
	}
	else if(factoryType == "BottomBlock"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("BottomBlock", new BlockFactory));
	}
	else if(factoryType == "Bullet"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("Bullet", new BulletFactory));
	}
	else if(factoryType == "EnemyBullet"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("EnemyBullet", new BulletFactory));
	}
	else if(factoryType == "Player"){
		factoryLibrary.insert(std::pair<std::string, ActorFactory*>("Player", new PlayerFactory));
	}
}

void GameAssetLibrary::Remove(std::string factoryType){
	factoryLibrary.erase(factoryType);
}

void GameAssetLibrary::Clear(){
	for(miter = factoryLibrary.begin(); miter != factoryLibrary.end(); ){
		delete miter->second;
		miter = factoryLibrary.erase(miter);
	}

	int x = 3;
}

ActorFactory* GameAssetLibrary::Find(std::string factoryType){
	miter = factoryLibrary.find(factoryType);

	if(miter != factoryLibrary.end())
		return(miter->second);
	else
		return(NULL);
}

Actor* GameAssetLibrary::CreateObject(std::string objectType, ArtAssetLibrary* sprites, TiXmlElement* objectData, b2World* world){
	Actor* newObject;
	
	miter = factoryLibrary.find(objectType);
	
	if(miter != factoryLibrary.end())
		newObject = miter->second->CreateObject(sprites, objectData, world);
	else
		newObject = NULL;

	return(newObject);
}
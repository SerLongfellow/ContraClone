#include "PlayerFactory.h"

PlayerFactory::PlayerFactory(){
	count = 0;
}

PlayerFactory::~PlayerFactory(){

}

Player* PlayerFactory::CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world){
	std::string objectID;
	std::stringstream ss;
	float x, y, angularVelocity;

	ss << "Z_PLAYER";

	ss >> objectID;

	ss.clear();

	x = (float)atof(data->Attribute("x"));
	y = (float)atof(data->Attribute("y"));
	angularVelocity = (float)atof(data->Attribute("angularVelocity"));

	bd.position.Set(RW2PW(x), RW2PW(y));

	body = world->CreateBody(&bd);

	shapefd.filter.categoryBits = entityCategory::PLAYER;
	shapefd.filter.maskBits = entityCategory::BLOCK | entityCategory::ENEMY | entityCategory::ENEMY_BULLET;

	Player* newPlayer = new Player();

	newPlayer->SetFixtureDef(&shapefd);

	if(newPlayer->Initialize(x, y, 0, angularVelocity, linearForce, body, objectID, sprites->Search("Player"))){
		newPlayer->animation = sprites->SearchAnimation("Player");

		newPlayer->health = health;
		body->CreateFixture(&shapefd); //Register fixture with physics world
		body->SetAngularDamping(angularDamping);
		body->SetLinearDamping(linearDamping);
		body = NULL;
		return(newPlayer);
	}
	else
		return(NULL);
}
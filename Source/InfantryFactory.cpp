#include "InfantryFactory.h"

InfantryFactory::InfantryFactory(){
	count = 0;
}

InfantryFactory::~InfantryFactory(){

}

Infantry* InfantryFactory::CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world){
	std::string objectID;
	std::stringstream ss;
	float x, y, angularVelocity;

	//b2FixtureDef sensor_shapefd;

	count++;

	ss << "INFANTRY_" << count;

	ss >> objectID;

	ss.clear();

	Infantry* newInfantry = new Infantry();

	x = (float)atof(data->Attribute("x"));
	y = (float)atof(data->Attribute("y"));
	angularVelocity = (float)atof(data->Attribute("angularVelocity"));

	bd.position.Set(RW2PW(x), RW2PW(y));

	body = world->CreateBody(&bd);

	shapefd.filter.categoryBits = entityCategory::ENEMY;
	//shapefd.filter.maskBits = entityCategory::PLAYER;

	newInfantry->SetFixtureDef(&shapefd);

	if(newInfantry->Initialize(x, y, 0, angularVelocity, linearForce, body, objectID, sprites->Search("Infantry"))){
		body->CreateFixture(&shapefd); //Register fixture with physics world
		//body->CreateFixture(&sensor_shapefd);

		body->SetAngularDamping(angularDamping);
		body->SetLinearDamping(linearDamping);
		body = NULL;
		return(newInfantry);
	}
	else
		return(NULL);
}
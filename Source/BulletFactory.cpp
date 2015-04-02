#include "BulletFactory.h"

BulletFactory::BulletFactory(){
	count = 0;
}

BulletFactory::~BulletFactory(){

}

Bullet* BulletFactory::CreateObject(ArtAssetLibrary* sprites, TiXmlElement* data, b2World* world){
	std::string objectID;
	std::stringstream ss;
	float x, y, angle;
	b2Vec2 vel = b2Vec2_zero;
	std::string bulletType;

	count++;

	Bullet* newBullet = new Bullet();


	data->QueryFloatAttribute("x", &x);
	data->QueryFloatAttribute("y", &y);
	data->QueryFloatAttribute("angle", &angle);
	data->QueryFloatAttribute("velX", &vel.x);
	data->QueryFloatAttribute("velY", &vel.y);
	bulletType = data->Attribute("bulletType");

	ss << "Z_" << bulletType << "_" << count;

	ss >> objectID;

	ss.clear();

	bd.position.Set(RW2PW(x), RW2PW(y));

	if(bulletType == "Bullet"){
		shapefd.filter.categoryBits = entityCategory::BULLET;
		shapefd.filter.maskBits = entityCategory::ENEMY;
	}
	else if(bulletType == "EnemyBullet"){
		shapefd.filter.categoryBits = entityCategory::ENEMY_BULLET;
		shapefd.filter.maskBits = entityCategory::PLAYER;
	}

	body = world->CreateBody(&bd);

	if(newBullet->Initialize(x, y, 0, 0, linearForce, body, objectID, sprites->Search(bulletType))){
		newBullet->objectType = bulletType;

		body->CreateFixture(&shapefd); //Register fixture with physics world

		body->SetAngularDamping(angularDamping);
		body->SetLinearDamping(linearDamping);

		body->SetLinearVelocity(vel);

		body = NULL;

		return(newBullet);
	}
	else
		return(NULL);
}
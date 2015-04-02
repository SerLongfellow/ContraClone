#include "Actor.h"
#include "GameFunctions.h"

Actor::Actor(){
	angularVelocity = 0;
	mass = 0;
	coeff_restitution = 0;
	coeff_friction = 1.0f;
	radius = 0;
	objectID = "OBJECT";
	objectType = "Object";
	initialized = false;
	isDead = false;
	wasDead = false;
	sprite = NULL;
	compoundActor = NULL;
	health = 0;
	timeSinceBulletShot = 0.0f;
}

Actor::~Actor(){
	if(sprite){
		sprite = NULL;
	}
	if(body){
		body = NULL;
	}
}

bool Actor::Initialize(float x, float y, float zIndex, float angularVelocity, float forceMag, b2Body* body, std::string objectID, Sprite* sprite){
	//this->angleChange = angleChange;
	this->objectID = objectID;
	this->sprite = sprite;
	this->body = body;
	this->angularVelocity = angularVelocity;
	this->forceMagnitude = forceMag;

	body->SetLinearDamping(1.0f);
	body->SetAngularDamping(1.0f);
	body->SetFixedRotation(true);

	body->SetUserData(this);

	initialized = true;
	return true;
}

bool Actor::IsInitialized(){
	return(initialized);
}

std::string Actor::GetObjectID(){
	return(objectID);
}

std::string Actor::GetObjectType(){
	return(objectType);
}

bool Actor::CheckIfDead(){
	return(isDead);
}

bool Actor::CheckIfWasDead(){
	return(wasDead);
}

void Actor::SetDead(bool cond){
	isDead = cond;
}

void Actor::SetSprite(Sprite* sprite){
	this->sprite = sprite;
}

void Actor::SetSounds(SoundMap* sounds){
	this->sounds = sounds;
}

b2Body* Actor::GetBody(){
	return(body);
}

void Actor::Draw(float gameTime, Camera* camera){

	if(sprite){	
		b2Vec2 pos = body->GetPosition();	

		D3DXVECTOR3 position;
		position.x = PW2RW(pos.x)-sprite->GetWidth()/2.0f;
		position.y = PW2RW(pos.y)-sprite->GetHeight()/2.0f;
		position.z = 0.0f;

		//Get the angle
		float angle = body->GetAngle();

		//Draw the sprite
		//sprite->Draw(gameTime, position, D3DXVECTOR3(RW2PW(pos.x), RW2PW(pos.y), 0.0f), angle, camera, NULL);
		sprite->Draw(gameTime, position, D3DXVECTOR3(PW2RW(pos.x), PW2RW(pos.y), 0.0f), angle, camera, NULL);
	}

	if(compoundActor){	
		compoundActor->Draw(gameTime, camera);
	}

}

b2FixtureDef* Actor::GetFixture(){
	return(shapefd);
}

void Actor::SetFixtureDef(b2FixtureDef* shapefd){
	this->shapefd = shapefd;
}

int Actor::GetHealth(){
	return(health);
}
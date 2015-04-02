#include "ActorFactory.h"

ActorFactory::ActorFactory(){
}

ActorFactory::~ActorFactory(){
	body = NULL;
}

void ActorFactory::SetPhysics(TiXmlElement* data){//, float width, float height){
	bd.type = b2_dynamicBody;

	std::string type = data->Attribute("type");
	float width, height;

	if(type == "dynamic"){
		bd.type = b2_dynamicBody;
	}
	else if(type == "kinematic"){
		bd.type = b2_kinematicBody;
	}
	else if(type == "static"){
		bd.type = b2_staticBody;
	}

	//Set up physics shape
	std::string shapeType = data->Attribute("shape");

	if(shapeType == "polygon"){
		polygonShape.m_radius = 0;

		int intWidth, intHeight;
		data->QueryIntAttribute("width", &intWidth);
		data->QueryIntAttribute("height", &intHeight);

		width = RW2PW(intWidth) / 2.0f;
		height = RW2PW(intHeight) / 2.0f;

		polygonShape.SetAsBox(width, height);

		shapefd.shape = &polygonShape;
	}
	else if(shapeType == "circle"){
		float radius;

		data->QueryFloatAttribute("radius", &radius);

		circleShape.m_radius = RW2PW(radius);

		shapefd.shape = &circleShape;
	}
	else if(shapeType == "edge"){

		int intWidth, intHeight;
		data->QueryIntAttribute("width", &intWidth);
		data->QueryIntAttribute("height", &intHeight);

		width = RW2PW(intWidth) / 2.0f;
		height = RW2PW(intHeight) / 2.0f;

		b2Vec2 upperLeft = b2Vec2(0 - width, 0 - height);

		b2Vec2 upperRight = b2Vec2(width, 0 - height);

		b2Vec2 lowerRight = b2Vec2(width, height);

		b2Vec2 lowerLeft = b2Vec2(0 - width, height);


		// bottom
		edgeShape[0].Set(lowerLeft, lowerRight);

		// top
		edgeShape[1].Set(upperRight, upperLeft);

		//right
		edgeShape[2].Set(upperRight, lowerRight);

		//left
		edgeShape[3].Set(upperLeft, lowerLeft);
	}

	//data->QueryFloatAttribute("friction", &shapefd.friction);
	shapefd.friction = 0.0f;
	data->QueryFloatAttribute("restitution", &shapefd.restitution);
	data->QueryFloatAttribute("density", &shapefd.density);
	data->QueryFloatAttribute("linForce", &linearForce);
	data->QueryFloatAttribute("angForce", &angularForce);
	data->QueryFloatAttribute("linDamp", &linearDamping);
	data->QueryFloatAttribute("angDamp", &angularDamping);

	bool alive;
	data->QueryBoolAttribute("alive", &alive);

	if(alive)
		data->QueryIntAttribute("health", &health);

}
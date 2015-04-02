#include "Animation.h"
#include "ThirdParty\tinyxml\tinyxml.h"
#include "GameFunctions.h"

Animation::Animation(){
	currentState = 1;
}

Animation::~Animation(){

}

void Animation::UpdateAnimation(int actorState, RECT& srcRect, b2Body* body, b2FixtureDef* shapefd){
	State* animState = NULL;

	if(!states.empty())
		animState = states.at(currentState);

	if(animState){
		if(actorState != currentState){
			//Change animations
			animState->currentState = 0;
			currentState = actorState;

			animState = states.at(currentState);
		}
		else{

			animState->currentState++;

			if(animState->currentState >= animState->numStates){
				//Restart animation
				animState->currentState = 0;
			}
		}

		srcRect.top = animState->startY;
		srcRect.bottom = animState->startY + animState->height;

		if(animState->order == "forward"){
			srcRect.left = animState->startX + (animState->currentState * animState->width);
			srcRect.right = srcRect.left + animState->width;
		}
		else{
			srcRect.right =  animState->startX + ( (animState->numStates - animState->currentState) * animState->width);
			srcRect.left = srcRect.right - animState->width;
		}

		b2Fixture* fixture = body->GetFixtureList();

		body->DestroyFixture(fixture);

		b2PolygonShape polygonShape;

		polygonShape.SetAsBox(RW2PW(animState->width) / 2.0f, RW2PW(animState->height) / 2.0f);
		polygonShape.m_radius = 0;

		shapefd->shape = &polygonShape;

		body->CreateFixture(shapefd);
	}
}

bool Animation::LoadAnimation(const char* animationFile, float scale){
	TiXmlDocument levelConfig(animationFile);

	if(!levelConfig.LoadFile()){
		std::string error = levelConfig.ErrorDesc();
		return false;
	}

	TiXmlHandle hDoc(&levelConfig);
	TiXmlElement* pRoot;
	TiXmlElement* pParm;

	pRoot = levelConfig.FirstChildElement("Animation");

	if(!pRoot) 
		return false;

	pParm = pRoot->FirstChildElement("Info");

	if(!pParm)
		return false;



	pParm = pRoot->FirstChildElement("State");

	while(pParm){

		State* newState = new State;

		newState->currentState = 0;

		pParm->QueryIntAttribute("height", &newState->height);
		pParm->QueryIntAttribute("width", &newState->width);
		pParm->QueryIntAttribute("start_x", &newState->startX);
		pParm->QueryIntAttribute("start_y", &newState->startY);
		pParm->QueryIntAttribute("num_states", &newState->numStates);

		newState->height *= scale;
		newState->width *= scale;
		newState->startX *= scale;
		newState->startY *= scale;


		newState->order = pParm->Attribute("order");

		states.push_back(newState);

		pParm = pParm->NextSiblingElement("State");
	}

	return true;
}

int Animation::GetWidth(){
	return(states.at(currentState)->width);
}

int Animation::GetHeight(){
	return(states.at(currentState)->height);
}
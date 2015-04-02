#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <map>
#include <vector>
#include <string>
#include <d3dx9.h>
#include <Box2D\Box2D.h>

class Actor;

struct objectToAdd {
	std::string objectType;
	D3DXVECTOR3 position;
	float angle;
	b2Vec2 vel;
};

enum entityCategory {
	PLAYER =			0x0001,
	BULLET =		    0x0002,
	ENEMY =				0x0004,
	BLOCK =				0x0008,
	DEAD =				0x0010,
	ENEMY_BULLET =		0x0020
};

enum PlayerState{
	STILL_LEFT,
	STILL_RIGHT,
	RUN_LEFT,
	RUN_RIGHT,
	JUMP_LEFT,
	JUMP_RIGHT,
	CROUCH_LEFT,
	CROUCH_RIGHT
};

typedef std::map<std::string, Actor*> ActorMap;
typedef std::vector<objectToAdd> ActorVector;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#endif
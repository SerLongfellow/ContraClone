#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <map>
#include <vector>
#include <ctime>
#include <Box2D\Box2D.h>

#include "Actor.h"
#include "Camera.h"
#include "ArtAssetLibrary.h"
#include "GameAssetLibrary.h"
#include "GraphicsDevice.h"
#include "InputDevice.h"
#include "Definitions.h"
#include "ContactListener.h"
#include "Box2DDebugDraw.h"
#include "SoundDevice.h"
#include "SoundLibrary.h"

#include "ThirdParty\tinyxml\tinyxml.h"
#include "ThirdParty\tinyxml\tinystr.h"

class Game{

public:

	//Constructor/Destructor
	Game();
	~Game();

	//Game Functions
	bool Initialize(HINSTANCE hInst, HWND hWnd);
	bool IsInitialized();
	bool Run();
	void Update(float deltaTime);
	void Draw(float deltaTime);


protected:
	//Private methods
	bool LoadLevel(const char* levelConfig);
	bool UnloadLevel();
	bool ResetLevel();

	bool ObjectIsOnScreen(const b2Vec2* pos);

	//Private members
	bool initialized;
	bool focusedOnObject;
	std::clock_t prevClock;
	std::clock_t clock;

	std::string levelFile;

	const b2Vec2 gravity;
	b2World* world;
	Box2DDebugDraw* debugDraw;

	GraphicsDevice* gDevice;
	InputDevice* inputDevice;
	SoundDevice* soundDevice;

	GameAssetLibrary* factoryLibrary;
	ArtAssetLibrary* spriteLibrary;
	SoundLibrary* soundLibrary;

	Camera* camera;

	ContactListener cl;

	ActorMap objectContainer;
	ActorMap::iterator miter;

	ActorVector objectsToAdd;
	ActorVector::iterator viter; 

	enum gameState{
		RUNNING,
		PLAYER_DEAD,
		LEVEL_FINISHED,
		WON,
		LOADING,
		PAUSED,
		GAME_OVER,
		QUITTING
	};

	int state;
	int levelCount;
	b2Vec2 finishPosition;
	int playerHealth;

	SoundMap* gameSounds;
};

#endif
#include "Game.h"
#include "GameFunctions.h"
#include <fstream>
#include <set>

Game::Game():gravity(0.0f, 10.0f){
	gDevice = NULL;
	inputDevice = NULL;
	soundDevice = NULL;
	factoryLibrary = NULL;
	soundLibrary = NULL;
	camera = NULL;
	world = NULL;
	gameSounds = NULL;
	playerHealth = 0;
	levelCount = 1;
}

Game::~Game(){

	if(gDevice){
		delete gDevice;
		gDevice = NULL;
	}

	if(soundDevice){
		delete soundDevice;
		soundDevice = NULL;
	}

	if(inputDevice){
		delete inputDevice;
		inputDevice = NULL;
	}

	if(factoryLibrary){
		delete factoryLibrary;
		factoryLibrary = NULL;
	}

	if(spriteLibrary){
		delete spriteLibrary;
		spriteLibrary = NULL;
	}

	if(soundLibrary){
		delete soundLibrary;
		soundLibrary = NULL;
	}

	if(camera){
		delete camera;
		camera = NULL;
	}

	if(world){
		delete world;
		world = NULL;
	}
}

bool Game::Initialize(HINSTANCE hInst, HWND hWnd){

	gDevice = new GraphicsDevice();

	if(!gDevice->Initialize(hWnd, true))
		return false;

	inputDevice = new InputDevice();

	if(!inputDevice->Initialize(hInst, hWnd))
		return false;

	soundDevice = new SoundDevice();

	if(!soundDevice->Initialize(hWnd)){
		return false;
	}

	world = new b2World(gravity);

	debugDraw = new Box2DDebugDraw();
	if(debugDraw->Initialize(gDevice->GetDevice())) {
		world->SetDebugDraw(debugDraw);
	}

	spriteLibrary = new ArtAssetLibrary();
	factoryLibrary = new GameAssetLibrary();
	soundLibrary = new SoundLibrary();

	camera = new Camera();

	if(!camera->Initialize(0, 0, 0, 0, hWnd))
		return false;

	debugDraw->camera = camera;

	levelFile = "./Config/level1.xml";

	if(!LoadLevel(levelFile.c_str()))
		return false;

	world->SetContactListener(&cl);

	prevClock = 0;
	clock = 0;
	state = RUNNING;

	initialized = true;

	return true;
}

bool Game::IsInitialized(){
	return(initialized);
}

bool Game::Run(){
	prevClock = clock;
	clock = std::clock();

	float gameTime = ((float)(clock - prevClock)) / CLOCKS_PER_SEC;

	if(inputDevice)
		inputDevice->Update();

	if(state == RUNNING){
		Update(gameTime);
		Draw(gameTime);
	}
	else if(state == PLAYER_DEAD){
		if(inputDevice->GetKeyboardState()[DIK_RETURN]  & 0x80){
			//Relod level
			if(ResetLevel()){
				state = RUNNING;
				gameTime = 0.0f;
			}
		}
		else if(inputDevice->GetKeyboardState()[DIK_ESCAPE]  & 0x80){
			//Quit game
			state = QUITTING;
		}
	}
	else if(state == QUITTING){
		return false;
	}
	else if(state == LEVEL_FINISHED){
		if(inputDevice->GetKeyboardState()[DIK_RETURN]  & 0x80 || inputDevice->GetKeyboardState()[DIK_NUMPADENTER]  & 0x80){
			//Relod level
			levelCount++;

			if(UnloadLevel()){
				std::stringstream ss;

				ss << "./Config/level" << levelCount << ".xml";
				ss >> levelFile;

				if(LoadLevel(levelFile.c_str())){
					state = RUNNING;
					gameTime = 0.0f;
				}
				else{
					state = WON;
				}
			}
		}
		else if(inputDevice->GetKeyboardState()[DIK_ESCAPE]  & 0x80){
			//Quit game
			state = QUITTING;
		}
	}
	else if(state == PAUSED){
		if(inputDevice->GetKeyboardState()[DIK_ESCAPE]  & 0x80){
			state = RUNNING;

			Sleep(200);
		}
	}
	else if(state == WON){
		if(inputDevice->GetKeyboardState()[DIK_ESCAPE]  & 0x80 || inputDevice->GetKeyboardState()[DIK_RETURN]  & 0x80){
			state = QUITTING;
		}


		gDevice->Begin();
		gDevice->Clear(D3DCOLOR_XRGB(0,0,0));
		gDevice->PrintHeaderText("You finished the game!", SCREEN_WIDTH / 2 - (int)(SCREEN_WIDTH * 0.27f), (SCREEN_HEIGHT / 2 - (int)(SCREEN_HEIGHT * 0.12f)), 0xffffffff);
		gDevice->End();
		gDevice->Present();

	}

	return true;
}

void Game::Update(float gameTime){

	if(inputDevice->GetKeyboardState()[DIK_1] & 0x80) {
		//get the flags
		uint32 flags = debugDraw->GetFlags();
		flags ^= debugDraw->e_aabbBit;	//toggle the AABB bit
		debugDraw->SetFlags(flags);		//set the flags back

		Sleep(200);						//allow me to lift my finger before updating again
	}

	if(inputDevice->GetKeyboardState()[DIK_2] & 0x80) {
		//get the flags
		uint32 flags = debugDraw->GetFlags();
		flags ^= debugDraw->e_centerOfMassBit;	//toggle the AABB bit
		debugDraw->SetFlags(flags);		//set the flags back

		Sleep(200);						//allow me to lift my finger before updating again
	}

	if(inputDevice->GetKeyboardState()[DIK_3] & 0x80) {
		//get the flags
		uint32 flags = debugDraw->GetFlags();
		flags ^= debugDraw->e_jointBit;	//toggle the AABB bit
		debugDraw->SetFlags(flags);		//set the flags back

		Sleep(200);						//allow me to lift my finger before updating again
	}

	if(inputDevice->GetKeyboardState()[DIK_4] & 0x80) {
		//get the flags
		uint32 flags = debugDraw->GetFlags();
		flags ^= debugDraw->e_shapeBit;	//toggle the AABB bit
		debugDraw->SetFlags(flags);		//set the flags back

		Sleep(200);						//allow me to lift my finger before updating again
	}

	miter = objectContainer.find("Z_PLAYER");

	b2Vec2 playerPos = b2Vec2_zero;

	if(miter != objectContainer.end()){
		Actor* player = miter->second;

		playerPos = player->GetBody()->GetPosition();
		playerHealth = player->GetHealth();
	}

	if(playerPos.x > finishPosition.x){
		state = LEVEL_FINISHED;
	}

	camera->Update(gameTime, inputDevice, &playerPos);

	//Update all the positions
	for(miter = objectContainer.begin(); miter != objectContainer.end(); miter++){

		if(ObjectIsOnScreen(&miter->second->GetBody()->GetPosition()))
			miter->second->Update(gameTime, inputDevice, &objectsToAdd);
	}

	//Add new objects, such as bullets to master pool of objects
	for(viter = objectsToAdd.begin(); viter != objectsToAdd.end(); ){

		Actor* newObject;

		std::string objectType;

		objectType = viter->objectType;

		TiXmlElement* pRoot = new TiXmlElement("Actor");

		pRoot->SetDoubleAttribute("x", viter->position.x);
		pRoot->SetDoubleAttribute("y", viter->position.y);
		pRoot->SetDoubleAttribute("angle", viter->angle);
		pRoot->SetDoubleAttribute("velX", viter->vel.x);
		pRoot->SetDoubleAttribute("velY", viter->vel.y);
		pRoot->SetAttribute("bulletType", objectType.c_str());

		newObject = factoryLibrary->CreateObject(objectType, spriteLibrary, pRoot, world);

		if(newObject){
			objectContainer.insert(std::pair<std::string, Actor*>(newObject->GetObjectID(), newObject));

			delete pRoot;
			pRoot = NULL;

			viter = objectsToAdd.erase(viter);
		}
		else{
			viter++;
		}
	}

	world->Step(1.0f / 60.0f, 4, 2);

	//Find any "dead" objects and delete them
	for(miter = objectContainer.begin(); miter != objectContainer.end();){

		//If object is dead, remove it from container
		if(miter->second->CheckIfDead()){
			std::string objectType = miter->second->GetObjectType();

			if(objectType == "Player"){
				state = PLAYER_DEAD;
				break;
			}
			else if(objectType == "Bullet"){
				//Delete object and clean up
				world->DestroyBody(miter->second->GetBody());

				delete miter->second;
				miter->second = NULL;
				miter = objectContainer.erase(miter);
			}
			else if(objectType == "Infantry" && !miter->second->CheckIfWasDead()){
				//Change sprite to dead sprite
				Sprite* deadSprite = spriteLibrary->Search("InfantryDead");

				miter->second->SetSprite(deadSprite);

				b2Fixture* fixture = miter->second->GetBody()->GetFixtureList();

				miter->second->GetBody()->DestroyFixture(fixture);

				b2FixtureDef* shapefd = miter->second->GetFixture();			
				b2PolygonShape polygonShape;

				polygonShape.m_radius = 0;
				polygonShape.SetAsBox(RW2PW((float)deadSprite->GetWidth()) / 2.0f, RW2PW((float)deadSprite->GetHeight()) / 2.0f);

				shapefd->shape = &polygonShape;

				shapefd->filter.categoryBits = entityCategory::DEAD;

				miter->second->GetBody()->CreateFixture(shapefd);

				miter++;
			}
			else{
				miter++;
			}
		}
		else{
			miter++;
		}
	}
}

void Game::Draw(float gameTime){

	gDevice->Clear(D3DCOLOR_XRGB(53, 120, 255));

	gDevice->Begin();

	//Draw all objects
	for(miter = objectContainer.begin(); miter != objectContainer.end(); miter++){
		if(ObjectIsOnScreen(&miter->second->GetBody()->GetPosition()))
			miter->second->Draw(gameTime, camera);
	}

	//Draw hearts left
	Sprite* heart = spriteLibrary->Search("Heart");

	if(heart){
		int xOffset = 0;

		for(int i = 0; i < playerHealth; i++){

			heart->Draw(gameTime, D3DXVECTOR3(20 + xOffset - camera->GetCenter().x, 20, 0), D3DXVECTOR3(30 + xOffset - camera->GetCenter().x, 30, 0), 0.0f, camera, NULL);

			xOffset += 25;
		}

	}

	world->DrawDebugData();

	if(state == PLAYER_DEAD){
		//Prompt for restart
		gDevice->PrintHeaderText("You are dead.", SCREEN_WIDTH / 2 - (int)(SCREEN_WIDTH * 0.19f), 100, 0xffffffff);
		gDevice->PrintSmallText("Press \"Enter\" to restart or \"Escape\" to quit.", SCREEN_WIDTH / 2 - (int)(SCREEN_WIDTH * 0.24f), 200, 0xffffffff);
	}
	else if(state == LEVEL_FINISHED){
		gDevice->PrintHeaderText("You won!", SCREEN_WIDTH / 2 - (int)(SCREEN_WIDTH * 0.13f), 100, 0xffffffff);
		gDevice->PrintSmallText("Press \"Enter\" to continue to the next level or \"Escape\" to quit.", SCREEN_WIDTH / 2 - (int)(SCREEN_WIDTH * 0.32f), 200, 0xffffffff);
	}

	gDevice->End();

	gDevice->Present();

	//Check here so there isn't a final "step"
	if(inputDevice->GetKeyboardState()[DIK_ESCAPE]  & 0x80){
		state = PAUSED;

		Sleep(200);
	}

	if(state == PAUSED){
		gDevice->Begin();
		gDevice->PrintHeaderText("Paused", SCREEN_WIDTH / 2 - 160, 100, 0xffffffff);
		gDevice->End();
		gDevice->Present();
	}
}

bool Game::LoadLevel(const char* levelConfigFile){

	TiXmlDocument levelConfig(levelConfigFile);

	if(!levelConfig.LoadFile()){
		std::string error = levelConfig.ErrorDesc();
		return false;
	}

	TiXmlHandle hDoc(&levelConfig);
	TiXmlElement* pRoot;
	TiXmlElement* pParm;
	TiXmlElement* pInner;

	std::set<std::string> animatedActors;

	//Set up factories-----------------------------------------------------------------------------------------------
	pRoot = levelConfig.FirstChildElement("Level");

	if(!pRoot) 
		return false;

	pParm = pRoot->FirstChildElement("RequiredResources")->FirstChildElement("Factories");

	if(!pParm)
		return false;

	pInner = pParm->FirstChildElement("Factory");

	while(pInner){
		factoryLibrary->Insert(pInner->Attribute("type"));

		pInner = pInner->NextSiblingElement("Factory");
	}
	//Done setting up the factories-----------------------------------------------------------------------------------


	//Set up sprites and object types--------------------------------------------------------------------------------
	pParm = pParm->NextSiblingElement("Sprites");

	if(!pParm)
		return false;

	std::string objectType, spriteFile;
	float width, height;

	pInner = pParm->FirstChildElement("Sprite");

	while(pInner){
		objectType = pInner->Attribute("name");
		spriteFile = pInner->Attribute("file");
		pInner->QueryFloatAttribute("width", &width);
		pInner->QueryFloatAttribute("height", &height);
		/*
		width = atof(pInner->Attribute("width"));
		height = atof(pInner->Attribute("height"));*/
		float scale = 0.0f;

		pInner->QueryFloatAttribute("scale", &scale);

		width *= scale;
		height *= scale;

		spriteLibrary->Insert(objectType, spriteFile, (UINT)width, (UINT)height, gDevice->GetDevice());

		bool animated = false;
		std::string animFile;
		pInner->QueryBoolAttribute("animated", &animated);

		if(animated){
			animFile = pInner->Attribute("animation_file");

			spriteLibrary->InsertAnimation(objectType, animFile.c_str(), scale);
			animatedActors.insert(objectType);
		}

		pInner = pInner->NextSiblingElement("Sprite");
	}
	//Done setting up sprites---------------------------------------------------------------------------------------


	//Set up sounds-------------------------------------------------------------------------------------------------
	pParm = pParm->NextSiblingElement("Sounds");

	if(!pParm)
		return false;

	std::string soundID, soundFile;

	/*pInner = pParm->FirstChildElement("Background");

	if(pInner){
		soundFile = pInner->Attribute("file");

		backgroundMusic = new GameSoundBuffer();

		backgroundMusic->Initialize(soundDevice->device, soundFile.c_str());
	}
*/
	pInner = pParm->FirstChildElement("Actor");

	while(pInner){

		TiXmlElement* pInner2 = pInner->FirstChildElement("Sound");
		objectType = pInner->Attribute("name");

		while(pInner2){
			soundID = pInner2->Attribute("name");
			soundFile = pInner2->Attribute("file");

			soundLibrary->InsertSound(objectType, soundID, soundFile, soundDevice->device);

			pInner2 = pInner2->NextSiblingElement("Sound");
		}

		pInner = pInner->NextSiblingElement("Actor");
	}
	//Done setting up sounds-----------------------------------------------------------------------------------------


	//Set up physics for each object---------------------------------------------------------------------------------
	pParm = pRoot->FirstChildElement("Physics");

	if(!pParm)
		return false;

	pInner = pParm->FirstChildElement("Definition");

	while(pInner){

		objectType = pInner->Attribute("name");
		ActorFactory* factory = factoryLibrary->Find(objectType);

		if(factory)
			factory->SetPhysics(pInner);

		pInner = pInner->NextSiblingElement("Definition");
	}
	//Done setting up physics----------------------------------------------------------------------------------------------


	//Set up Actors; fill object container---------------------------------------------------------------------------------
	pParm = pRoot->FirstChildElement("ActorPositions");

	if(!pParm)
		return false;

	pInner = pParm->FirstChildElement("Actor");

	while(pInner){

		Actor* newObject;

		objectType = pInner->Attribute("name");

		newObject = factoryLibrary->CreateObject(objectType, spriteLibrary, pInner, world);

		if(newObject){

			SoundMap* sounds = soundLibrary->Search(objectType);
			if(sounds){
				newObject->SetSounds(sounds);
			}

			objectContainer.insert(std::pair<std::string, Actor*>(newObject->GetObjectID(), newObject));
		}

		pInner = pInner->NextSiblingElement("Actor");
	}
	//Done setting up actors------------------------------------------------------------------------------------------

	pParm = pRoot->FirstChildElement("FinishPosition");

	if(!pParm)
		return false;

	pParm->QueryFloatAttribute("x", &finishPosition.x);
	pParm->QueryFloatAttribute("y", &finishPosition.y);

	finishPosition.x = RW2PW(finishPosition.x);
	finishPosition.y = RW2PW(finishPosition.y);

	gameSounds = soundLibrary->Search("Game");

	//backgroundMusic->PlaySoundA(-1500, DSBPLAY_LOOPING);
	gameSounds->PlaySoundA("background", -1450, DSBPLAY_LOOPING);

	return true;
}

bool Game::UnloadLevel(){
	//Remove all current objects, so we can replace them with the original objects and their positions
	for(miter = objectContainer.begin(); miter != objectContainer.end(); miter++){
		world->DestroyBody(miter->second->GetBody());
		delete miter->second;
		miter->second = NULL;
	}

	factoryLibrary->Clear();
	spriteLibrary->Clear();

	gameSounds->StopSound("background");

	soundLibrary->Clear();

	/*backgroundMusic->StopSound();
	delete backgroundMusic;*/


	objectContainer.clear();
	camera->Reset();

	return true;
}

bool Game::ResetLevel(){
	//Remove all current objects, so we can replace them with the original objects and their positions
	for(miter = objectContainer.begin(); miter != objectContainer.end(); miter++){
		world->DestroyBody(miter->second->GetBody());
		delete miter->second;
		miter->second = NULL;
	}

	objectContainer.clear();
	camera->Reset();

	TiXmlDocument levelConfig(levelFile.c_str());

	if(!levelConfig.LoadFile()){
		std::string error = levelConfig.ErrorDesc();
		return false;
	}

	TiXmlHandle hDoc(&levelConfig);
	TiXmlElement* pRoot;
	TiXmlElement* pParm;
	TiXmlElement* pInner;
	std::string objectType;

	//Set up factories
	pRoot = levelConfig.FirstChildElement("Level");

	if(!pRoot) 
		return false;

	//Reset physics for dead things
	pParm = pRoot->FirstChildElement("Physics");

	if(!pParm)
		return false;

	pInner = pParm->FirstChildElement("Definition");

	while(pInner){

		objectType = pInner->Attribute("name");

		ActorFactory* factory = factoryLibrary->Find(objectType);

		if(factory)
			factory->SetPhysics(pInner);//, width, height);
		//}

		pInner = pInner->NextSiblingElement("Definition");
	}
	//Done setting up physics

	//Set up Actors; fill object container
	pParm = pRoot->FirstChildElement("ActorPositions");

	if(!pParm)
		return false;

	pInner = pParm->FirstChildElement("Actor");

	while(pInner){

		Actor* newObject;

		objectType = pInner->Attribute("name");

		newObject = factoryLibrary->CreateObject(objectType, spriteLibrary, pInner, world);

		if(newObject){
			SoundMap* sounds = soundLibrary->Search(objectType);
			if(sounds){
				newObject->SetSounds(sounds);
			}

			objectContainer.insert(std::pair<std::string, Actor*>(newObject->GetObjectID(), newObject));
		}

		pInner = pInner->NextSiblingElement("Actor");
	}
	//Done setting up actors

	return true;
}

bool Game::ObjectIsOnScreen(const b2Vec2* pos){
	float offsetX = RW2PW(-camera->GetCenter().x);

	if(	(pos->x > offsetX - RW2PW(50) && pos->x < RW2PW(1.6f * SCREEN_WIDTH) + offsetX) &&
		(pos->y < SCREEN_HEIGHT + RW2PW(30))){

			return true;
	}
	else 
		return false;
}
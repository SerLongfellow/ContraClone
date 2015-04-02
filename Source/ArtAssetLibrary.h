#ifndef ART_ASSET_LIBRARY_H
#define ART_ASSET_LIBRARY_H

#include <map>
#include <string>
#include "Sprite.h"
#include "Animation.h"

class ArtAssetLibrary{

public:
	ArtAssetLibrary();
	~ArtAssetLibrary();

	void Insert(std::string ojbectType, std::string fileLocation, UINT width, UINT height, const LPDIRECT3DDEVICE9 &device);
	void Remove(std::string ojbectType);

	void Clear();

	void InsertAnimation(std::string objectType, const char* animFile, float scale);

	Sprite* Search(std::string objectType);
	Animation* SearchAnimation(std::string objectType);

	//Shouldn't ever need to use this, but I'll put it here for the sake of not 
	//crashing if someone tries to access a nonexistant sprite
	Sprite* defaultSprite;

private:

	std::map<std::string, Sprite*> spriteLibrary;
	std::map<std::string, Sprite*>::iterator miter;

	std::map<std::string, Animation*> animationLibrary;
	std::map<std::string, Animation*>::iterator anim_miter;
};

#endif
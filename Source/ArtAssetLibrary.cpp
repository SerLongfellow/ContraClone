#include "ArtAssetLibrary.h"

ArtAssetLibrary::ArtAssetLibrary(){
	defaultSprite = NULL;
}

ArtAssetLibrary::~ArtAssetLibrary(){

	for(miter = spriteLibrary.begin(); miter != spriteLibrary.end(); miter++)
		miter->second = NULL;

	spriteLibrary.clear();
}

void ArtAssetLibrary::Insert(std::string objectType, std::string fileLocation, UINT width, UINT height, const LPDIRECT3DDEVICE9 &device){

	//Make sure a sprite for this object does not already exist
	if(spriteLibrary.find(objectType) == spriteLibrary.end()){

		Sprite* newSprite = new Sprite();

		if(newSprite->Initialize(device, fileLocation, width, height)){
			spriteLibrary.insert(std::pair<std::string, Sprite*>(objectType, newSprite));
		}
		else{
			//If sprite didn't get initialized properly, assign it to the default sprite
		}
	}
}

void ArtAssetLibrary::Clear(){
	for(miter = spriteLibrary.begin(); miter != spriteLibrary.end(); ){

		delete miter->second;
		miter = spriteLibrary.erase(miter);
	}
}

void ArtAssetLibrary::Remove(std::string objectType){

	spriteLibrary.erase(objectType);
}

Sprite* ArtAssetLibrary::Search(std::string objectType){

	miter = spriteLibrary.find(objectType);

	if(miter != spriteLibrary.end())
		return(miter->second);
	else
		return(defaultSprite);
}

void ArtAssetLibrary::InsertAnimation(std::string objectType, const char* animFile, float scale){

	//Make sure a sprite for this object does not already exist
	if(animationLibrary.find(objectType) == animationLibrary.end()){

		Animation* newAnim = new Animation();

		if(newAnim->LoadAnimation(animFile, scale)){
			animationLibrary.insert(std::pair<std::string, Animation*>(objectType, newAnim));
		}
		else{
			delete newAnim;
			newAnim = NULL;
		}
	}
}

Animation* ArtAssetLibrary::SearchAnimation(std::string objectType){
	anim_miter = animationLibrary.find(objectType);

	if(anim_miter != animationLibrary.end()){
		return(anim_miter->second);
	}
	else{
		return NULL;
	}
}
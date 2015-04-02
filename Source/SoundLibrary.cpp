#include "SoundLibrary.h"

SoundLibrary::SoundLibrary(){
}

SoundLibrary::~SoundLibrary(){

	soundLibrary.clear();
}

void SoundLibrary::InsertSound(std::string objectType, std::string soundID, std::string fileLocation, IDirectSound8* device){

	miter = soundLibrary.find(objectType);
	//Make sure a sound map for this object does not already exist
	if(miter == soundLibrary.end()){

		SoundMap* newMap = new SoundMap;
		GameSoundBuffer* newSound = new GameSoundBuffer();

		if(newSound->Initialize(device, fileLocation.c_str())){
			//newMap->insert(std::pair<std::string, GameSoundBuffer*>(soundID, newSound));
			newMap->InsertSound(soundID, newSound);

			soundLibrary.insert(std::pair<std::string, SoundMap*>(objectType, newMap));
		}
	}
	else{
		GameSoundBuffer* newSound = new GameSoundBuffer();

		if(newSound->Initialize(device, fileLocation.c_str())){
			miter->second->InsertSound(soundID, newSound);
		}
	}
}

void SoundLibrary::Remove(std::string objectType){

	soundLibrary.erase(objectType);
}

void SoundLibrary::Clear(){

	for(miter = soundLibrary.begin(); miter != soundLibrary.end(); ){

		delete miter->second;
		miter = soundLibrary.erase(miter);
	}
}

SoundMap* SoundLibrary::Search(std::string objectType){

	miter = soundLibrary.find(objectType);

	if(miter != soundLibrary.end())
		return(miter->second);
	else
		return(NULL);
}
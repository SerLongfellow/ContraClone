#ifndef SOUND_MAP_H
#define SOUND_MAP_H

#include "GameSoundBuffer.h"

class SoundMap{

public:
	SoundMap(){}
	~SoundMap(){
		CleanUp();
	}

	void InsertSound(std::string soundID, GameSoundBuffer* sound){
		sounds.insert(std::pair<std::string, GameSoundBuffer*>(soundID, sound));
	}

	void PlaySound(std::string soundID, LONG lVolume, DWORD dwFlags){
		soundIter = sounds.find(soundID);

		if(soundIter != sounds.end()){
			soundIter->second->PlaySoundA(lVolume, dwFlags);
		}
	}

	void StopSound(std::string soundID){
		soundIter = sounds.find(soundID);

		if(soundIter != sounds.end()){
			soundIter->second->StopSound();
		}
	}

private:
	std::map<std::string, GameSoundBuffer*> sounds;
	std::map<std::string, GameSoundBuffer*>::iterator soundIter; 

	void CleanUp(){
		for(soundIter = sounds.begin(); soundIter != sounds.end(); ){
			delete soundIter->second;
			soundIter = sounds.erase(soundIter);
		}
	}
};

#endif
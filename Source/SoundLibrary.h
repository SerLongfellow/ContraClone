#ifndef SOUND_LIBRARY_H
#define SOUND_LIBRARY_H

#include <map>
#include <string>
#include "GameSoundBuffer.h"
#include "Definitions.h"
#include "SoundMap.h"


//typedef std::map<std::string, GameSoundBuffer*> SoundMap;

typedef std::map<std::string, SoundMap*> MasterMap;


class SoundLibrary{

public:
	SoundLibrary();
	~SoundLibrary();

	void InsertSound(std::string objectType, std::string soundID, std::string fileLocation, IDirectSound8* device);
	void Remove(std::string ojbectType);
	void Clear();

	SoundMap* Search(std::string objectType);

private:

	MasterMap soundLibrary;
	MasterMap::iterator miter;

};

#endif
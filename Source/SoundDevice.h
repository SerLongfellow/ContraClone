//Based on http://www.rastertek.com/dx10tut14.html

#ifndef SOUNDDEVICE_H
#define SOUNDDEVICE_H

#include<string>
#include<dsound.h>
#include<stdio.h>

class SoundDevice{

public:
	SoundDevice();
	~SoundDevice();

	bool Initialize(HWND);
	void Shutdown();

	IDirectSound8* device;

private:

	IDirectSoundBuffer* buffer;  //primary sound buffer
	bool initialized;

};


#endif
//Based on http://www.rastertek.com/dx10tut14.html

#ifndef GAMESOUNDBUFFER_H
#define GAMESOUNDBUFFER_H

#include<string>
#include<dsound.h>
#include<stdio.h>

class GameSoundBuffer{

public:

	GameSoundBuffer();
	~GameSoundBuffer();

	bool Initialize(IDirectSound8* device, const char* filename);
	void StopSound();
	bool PlaySound(LONG volume, DWORD dwFlags);
	
	//bool PlayWaveFile();


private:

	struct WaveHeaderType{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	IDirectSound8* device;
	IDirectSoundBuffer** buffer;

	IDirectSoundBuffer* tmp;


	bool LoadWaveFile(const char* filename, IDirectSound8* device);
	bool initialized;

	int bufferID;
	int Nbuffers;


};
 

#endif
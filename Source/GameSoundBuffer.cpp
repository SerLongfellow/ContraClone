//Based on http://www.rastertek.com/dx10tut14.html

#include<Windows.h>
#include"GameSoundBuffer.h"

GameSoundBuffer::GameSoundBuffer(){ 

	buffer = NULL;
	bufferID = 0;
	Nbuffers = 100;
	initialized = false;

}

GameSoundBuffer::~GameSoundBuffer(){

	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = NULL;
	
	// Release the secondary sound buffers
	for(int i=0;i<Nbuffers;i++){
		if(buffer[i]){
			buffer[i]->Release();
			buffer[i] = NULL;
		}
	}

}

bool GameSoundBuffer::Initialize(IDirectSound8* device, const char* filename){

	// Load a wave audio file onto a secondary buffer.
	if(!LoadWaveFile(filename,device)){
		return false;
	}

	return true;

}

void GameSoundBuffer::StopSound(){
	buffer[bufferID]->Stop();
}

bool GameSoundBuffer::LoadWaveFile(const char* filename, IDirectSound8* device){

	//To start first open the .wav file and read in the header of the file. The header will contain
	//all the information about the audio file so we can use that to create a secondary buffer to
	//accommodate the audio data. The audio file header also tells us where the data begins and how
	//big it is. You will notice I check for all the needed tags to ensure the audio file is not
	//corrupt and is the proper wave file format containing RIFF, WAVE, fmt, data, and WAVE_FORMAT_PCM tags.
	//I also do a couple other checks to ensure it is a 44.1KHz stereo 16bit audio file. If it is mono, 22.1 KHZ, 8bit,
	//or anything else then it will fail ensuring we are only loading the exact format we want.

	// Open the wave file in binary.
	if(fopen_s(&filePtr, filename, "rb") != 0){
		return false;
	}

	// Read in the wave file header.
	if(fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr) != 1){
		return false;
	}

	//Now that the wave header file has been verified we can setup the secondary buffer we will
	//load the audio data onto. We have to first set the wave format and buffer description of the
	//secondary buffer similar to how we did for the primary buffer. There are some changes though 
	//since this is secondary and not primary in terms of the dwFlags and dwBufferBytes.

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	/*waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;*/

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = waveFileHeader.sampleRate;
	waveFormat.wBitsPerSample = waveFileHeader.bitsPerSample;
	waveFormat.nChannels = waveFileHeader.numChannels;
	waveFormat.nBlockAlign = waveFileHeader.blockAlign;//(waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFileHeader.bytesPerSecond;//waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	buffer = new IDirectSoundBuffer*[Nbuffers];
	for(int i=0;i<Nbuffers;i++){

		// Create a temporary sound buffer with the specific buffer settings.
		if(!SUCCEEDED(device->CreateSoundBuffer(&bufferDesc, &buffer[i], NULL))){
			return false;
		}

	}

	//Now that the secondary buffer is ready we can load in the wave data from the audio file.
	//First I load it into a memory buffer so I can check and modify the data if I need to.
	//Once the data is in memory you then lock the secondary buffer, copy the data to it using a memcpy,
	//and then unlock it. This secondary buffer is now ready for use. Note that locking the secondary
	//buffer can actually take in two pointers and two positions to write to. This is because it is a 
	//circular buffer and if you start by writing to the middle of it you will need the size of the buffer
	//from that point so that you don't write outside the bounds of it. This is useful for streaming audio
	//and such. In this tutorial we create a buffer that is the same size as the audio file and write
	//from the beginning to make things simple.

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData){
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize){
		return false;
	}

	for(int i=0;i<Nbuffers;i++){
		// Lock the secondary buffer to write wave data into it.
		if(!SUCCEEDED(buffer[i]->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0))){
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

		// Unlock the secondary buffer after the data has been written to it.
		if(!SUCCEEDED(buffer[i]->Unlock((void*)bufferPtr, bufferSize, NULL, 0))){
			return false;
		}

	}


	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0){
		return false;
	}

	return true;
}

bool GameSoundBuffer::PlaySound(LONG volume, DWORD dwFlags){
 
	// Set position at the beginning of the sound buffer.
	if(!SUCCEEDED(buffer[bufferID]->SetCurrentPosition(0))){
		return false;
	}

	// Set volume of the buffer to 100%.
	if(!SUCCEEDED(buffer[bufferID]->SetVolume(volume))){
		return false;
	}

	//// Set frequency.
	//if(!SUCCEEDED(buffer[bufferID]->SetFrequency(5000))){
	//	return false;
	//}

	// Play the contents of the secondary sound buffer.
	if(!SUCCEEDED(buffer[bufferID]->Play(0, 0, dwFlags))){
		return false;
	}

	//Buffer wrap! 
	bufferID=(bufferID+1)%Nbuffers;

	return true;
}


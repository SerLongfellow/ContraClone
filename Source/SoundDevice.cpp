#include "SoundDevice.h"

SoundDevice::SoundDevice(){
	device = NULL;
	buffer = NULL;
	initialized = false;
}

SoundDevice::~SoundDevice(){


	if(device){
		device->Release();
		device = NULL;
	}

	if(buffer){
//		buffer->Release();
		buffer = NULL;
	}

}

bool SoundDevice::Initialize(HWND hWnd){

	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// Initialize the direct sound interface pointer for the default sound device.
	if(!SUCCEEDED(DirectSoundCreate8(NULL, &device, NULL))){
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	if(!SUCCEEDED(device->SetCooperativeLevel(hWnd,DSSCL_PRIORITY))){
		return false;
	}

	//We have to setup the description of how we want to access the primary buffer. The dwFlags are
	//the important part of this structure. In this case we just want to setup a primary buffer description
	//with the capability of adjusting its volume. There are other capabilities you can grab but we are keeping
	//it simple for now.

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	if(!SUCCEEDED(device->CreateSoundBuffer(&bufferDesc, &buffer, NULL))){
		return false;
	}

	//Now that we have control of the primary buffer on the default sound device we want to
	//change its format to our desired audio file format. Here I have decided we want high
	//quality sound so we will set it to uncompressed CD audio quality.

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo
	//(cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	//Set the primary buffer to be the wave format specified.
	if(!SUCCEEDED(buffer->SetFormat(&waveFormat))){
		return false;
	}

	initialized = true;
	return true;

}


#include "stdafx.h"
#include "Sound.h"

Sound::Sound()
{
	_directSound = 0;
	_primaryBuffer = 0;
	_secondaryBuffer1 = 0;
}

Sound::~Sound()
{

}

bool Sound::Initialise(HWND hwnd)
{
	bool result;

	//Initialise DirectSound API and the primary buffer
	result = InitialiseDirectSound(hwnd);
	if (!result)
		return false;

	//Load a WAV file and initialise the secondary buffer
	result = LoadWavFile("Resources\\Audio\\Shot.wav", &_secondaryBuffer1);
	if (!result)
		return false;

	//Plays the loaded WAV file
	result = PlayWavFile();
	if (!result)
		return false;

	return true;
}

void Sound::Close()
{
	//Frees the secondary buffer
	CloseWavFile(&_secondaryBuffer1);
	CloseDirectSound();
}

bool Sound::InitialiseDirectSound(HWND hwnd)
{
	//Handles getting an interface pointer to DirectSound and default primary buffer
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	result = DirectSoundCreate8(NULL, &_directSound, NULL);
	if (FAILED(result))
		return false;

	result = _directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	//How we access the primary buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Get control of primary buffer on default sound device
	result = _directSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL);
	if (FAILED(result))
		return false;

	//Setup primary buffer
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	result = _primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
		return false;

	return true;
}

void Sound::CloseDirectSound()
{
	if (_primaryBuffer)
	{
		_primaryBuffer->Release();
		_primaryBuffer = 0;
	}

	if (_directSound)
	{
		_directSound->Release();
		_directSound = 0;
	}
}

bool Sound::LoadWavFile(const char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	//Check if loaded sound is correct
	if ((waveFileHeader.chunkID[0]) != 'R' || (waveFileHeader.chunkID[1] != 'I') || (waveFileHeader.chunkID[2] != 'F') || (waveFileHeader.chunkID[3] != 'F'))
		return false;

	if ((waveFileHeader.chunkID[0]) != 'W' || (waveFileHeader.chunkID[1] != 'A') || (waveFileHeader.chunkID[2] != 'V') || (waveFileHeader.chunkID[3] != 'E'))
		return false;

	if ((waveFileHeader.chunkID[0]) != 'f' || (waveFileHeader.chunkID[1] != 'm') || (waveFileHeader.chunkID[2] != 't') || (waveFileHeader.chunkID[3] != ' '))
		return false;

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	if (waveFileHeader.numChannels != 2)
		return false;

	if (waveFileHeader.sampleRate != 44100)
		return false;

	if (waveFileHeader.bitsPerSample != 16)
		return false;

	if ((waveFileHeader.chunkID[0]) != 'd' || (waveFileHeader.chunkID[1] != 'a') || (waveFileHeader.chunkID[2] != 't') || (waveFileHeader.chunkID[3] != 'a'))
		return false;

	//Setup secondary buffer and load the audio data
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Create secondary buffer
	result = _directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

	//Now load the audio file
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
		return false;

	delete[]waveData;
	waveData = 0;

	return true;
}

void Sound::CloseWavFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

bool Sound::PlayWavFile()
{
	HRESULT result;

	result = _secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
		return false;

	result = _secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
		return false;

	result = _secondaryBuffer1->Play(0, 0, 0);
	if (FAILED(result))
		return false;

	return true;
}

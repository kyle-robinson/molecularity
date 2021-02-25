#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_


#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class Sound
{
private:
	struct WaveHeaderType
	{
		char chunkID[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkID[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkID[4];
		unsigned long dataSize;
	};

public:
	Sound();
	~Sound();

	bool Initialise(HWND);
	bool PlayWavFile(int, float);
	void Close();

private:
	bool InitialiseDirectSound(HWND);
	void CloseDirectSound();

	bool InitialiseSoundFiles();

	bool LoadWavFile(const char*, IDirectSoundBuffer8**);
	void CloseWavFile(IDirectSoundBuffer8**);

	IDirectSound8* _directSound;
	IDirectSoundBuffer* _primaryBuffer;
};

static IDirectSoundBuffer8* _secondaryBuffer[10];

enum SOUND_NAMES
{
	MAIN_MUSIC = 0,
	SHOT_SOUND
};

#endif // !_SOUND_H_
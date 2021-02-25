#pragma once
#ifndef SOUND_H
#define SOUND_H

#pragma comment( lib, "dsound.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )

#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

/// <summary>
/// Initializes all sound clips to be used in the game.
/// Call "PlayWavFile( 'enum', 'volume' )" to play a specified sound clip.
/// </summary>
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
	virtual ~Sound( void ) = default;

	bool Initialize( HWND );
	bool PlayWavFile( int, float );
	void Close();

private:
	bool InitializeDirectSound( HWND );
	void CloseDirectSound();

	bool InitialiseSoundFiles();

	bool LoadWavFile( const char*, IDirectSoundBuffer8** );
	void CloseWavFile( IDirectSoundBuffer8** );

	IDirectSound8* _directSound;
	IDirectSoundBuffer* _primaryBuffer;
};

// Make sure the size of the array keeps up with the sound name enum
static IDirectSoundBuffer8* _secondaryBuffer[10]; //Stores the loaded sounds

enum SOUND_NAMES
{
	MAIN_MUSIC = 0,
	SHOT_SOUND
};

#endif
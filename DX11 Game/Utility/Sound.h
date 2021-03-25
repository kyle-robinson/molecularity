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
	bool PlayWavFile( int, float, XMFLOAT3 );
	bool PlayWavFile( int, float );
	void Close();

	void UpdateListenerPos(XMFLOAT3 pos) { camPosX = pos.x; camPosY = pos.y; camPosZ = pos.z; }
	void UpdateListenerRot(XMFLOAT3 rot) { camRotX = rot.x; camRotY = rot.y; camRotZ = rot.z; }

	enum SOUND_NAMES
	{
		MAIN_MUSIC = 0,
		SHOT_SOUND,
		COLLISION_SOUND
	};

private:
	bool InitializeDirectSound( HWND );
	void CloseDirectSound();

	bool InitialiseSoundFiles();

	bool Load2DWavFile(const char*, IDirectSoundBuffer8**);
	bool Load3DWavFile( const char*, IDirectSoundBuffer8**, IDirectSound3DBuffer8** );

	void CloseWavFile( IDirectSoundBuffer8**, IDirectSound3DBuffer8** );

	IDirectSound8* _directSound;
	IDirectSoundBuffer* _primaryBuffer;

	IDirectSound3DListener8* _listener;

	// Make sure the size of the array keeps up with the sound names enum
	IDirectSoundBuffer8* _secondaryBuffer[10]; //Stores the loaded sounds
	IDirectSound3DBuffer8* _secondary3DBuffer[10]; //Stores the 3D info for loaded sounds

	float camPosX, camPosY, camPosZ;
	float camRotX, camRotY, camRotZ;
};

#endif
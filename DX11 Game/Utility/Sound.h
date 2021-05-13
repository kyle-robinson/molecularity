#pragma once
#ifndef SOUND_H
#define SOUND_H

#pragma comment(lib, "irrKlang.lib")
#include <irrklang/irrKlang.h>

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include<EventSystem/EventSystem.h>
using namespace DirectX;

class Sound: public Listener
{
public:
	Sound();
	~Sound();

	HRESULT InitialiseSounds();

	HRESULT UpdatePosition( XMFLOAT3 position, float rotation );

	HRESULT PlayMusic( int musicNum, bool loops = true ); //Plays music
	HRESULT PlaySoundEffects( int soundNum, bool loops = false, XMFLOAT3 soundPosition = { NULL, NULL, NULL }, float minDistance = 1.0f); //Plays sound effects

	float GetMusicVolume() { return musicVolume; } // Gets the volume level for music
	void SetMusicVolume( float volume ) { musicVolume = volume; for ( int i = 0; i < musicVec.size(); i++ ) { musicVec[i]->setVolume( musicVolume ); } } // Sets the volume level for music

	int GetMusicTrackNum() { return currentMusicTrack; }

	float GetSoundEffectsVolume() { return soundEffectsVolume; } // Gets the volume level for sound effects
	void SetSoundEffectsVolume( float volume ) { soundEffectsVolume = volume; for ( int i = 0; i < soundEffectsVec.size(); i++ ) { soundEffectsVec[i]->setDefaultVolume( soundEffectsVolume ); } } // Sets the volume level for sound effects

	enum MUSIC_NAMES // Names of all the music tracks
	{
		MUSIC_LEVEL = 0,
		MUSIC_MENU
	};

	enum SOUND_NAMES // Names of all the sound effects
	{
		SOUND_TOOLUSE = 0,
		SOUND_COLLISION
	};

	void SetMusicPause(bool isPause) { for (int i = 0; i < musicVec.size(); i++) { musicVec[i]->setIsPaused(isPause); } } // Sets the ispuase  for music


	//eventsystem
	void AddtoEvent();
	void HandleEvent(Event* event);

private:
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	irrklang::vec3df camPosition; // Camera's position
	irrklang::vec3df camLookDir; // Camera's rotation around the Y axis, used for look direction

	std::vector<irrklang::ISound*> musicVec; // Stores each music track
	std::vector<irrklang::ISoundSource*> soundEffectsVec; // Stores each of the sound effects

	float musicVolume = 1.0f;
	float soundEffectsVolume = 1.0f;
	int currentMusicTrack;

	//bool for shutting off sound
	bool musicOn;
	bool SoundEffectsOn;
	bool soundON;
};

#endif
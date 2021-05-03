#pragma once
#pragma comment(lib, "irrKlang.lib")
#include <irrklang/irrKlang.h>

class Sound
{
public:
	Sound();
	~Sound();

	HRESULT InitialiseSounds();

	HRESULT UpdatePosition(XMFLOAT3 position, float rotation);

	HRESULT PlayMusic(int musicNum, bool loops); //Plays music
	HRESULT PlaySFX(int soundNum, XMFLOAT3 soundPosition); //Plays sound effects
	HRESULT PlaySFX(int soundNum);

	// Only changes volume for sounds played after the change so at the moment music won't change
	float GetMusicVolume() { return musicVolume; } // Gets the volume level for music
	void SetMusicVolume(float volume) { musicVolume = volume; for (int i = 0; i < musicVec.size(); i++) { musicVec[i]->setDefaultVolume(musicVolume); } } // Sets the volume level for music

	float GetSFXVolume() { return SFXVolume; } // Gets the volume level for sound effects
	void SetSFXVolume(float volume) { SFXVolume = volume; for (int i = 0; i < SFXVec.size(); i++) { SFXVec[i]->setDefaultVolume(SFXVolume); } } // Sets the volume level for sound effects

	enum MUSIC_NAMES // Names of all the music tracks
	{
		MUSIC_MAIN = 0
	};

	enum SOUND_NAMES // Names of all the sound effects
	{
		SOUND_TOOLUSE = 0,
		SOUND_COLLISION
	};

private:
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	irrklang::vec3df camPosition; // Camera's position
	irrklang::vec3df camLookDir; // Camera's rotation around the Y axis, used for look direction

	std::vector<irrklang::ISoundSource*> musicVec; // Stores each music track
	std::vector<irrklang::ISoundSource*> SFXVec; // Stores each of the sound effects

	float musicVolume = 1.0f;
	float SFXVolume = 1.0f;
};
#pragma once
#ifndef SOUND_H
#define SOUND_H

#pragma comment(lib, "irrKlang.lib")
#include <irrklang/irrKlang.h>

#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <string>
#include<EventSystem/EventSystem.h>
using namespace DirectX;

class Sound: public Listener
{
public:
	void InitialiseMusicTrack( const char* fileLocation, std::string musicName );
	void InitialiseSoundEffect( const char* fileLocation, std::string soundName );

	void InitialiseSoundGroup( std::string groupName );

	void ClearAudio();
	void RemoveSoundEffects() { engine->removeAllSoundSources(); }

	void UpdatePosition( XMFLOAT3 position, float rotation );

	void PlayMusic( std::string musicName, bool loops = true ); // Plays music
	void PlaySoundEffect( std::string soundName, bool loops = false, XMFLOAT3 soundPosition = { NULL, NULL, NULL }, float minDistance = 1.0f ); // Plays sound effects
	std::map<std::string, irrklang::ISound*>& GetMusicTracks() { return musicTracks; }

	float& GetMusicVolume() { return musicVolume; } // Gets the volume level for music
	void SetMusicVolume( float &volume ) { musicVolume = volume; for ( auto music : musicTracks ) { music.second->setVolume( musicVolume ); } } // Sets the volume level for music

	std::string GetCurrentMusicTrack() { return currentMusicTrack; }

	float& GetSoundEffectsVolume() { return soundEffectsVolume; } // Gets the volume level for sound effects
	void SetSoundEffectsVolume( float &volume ) { soundEffectsVolume = volume; for ( auto sound : soundEffects ) { sound.second->setDefaultVolume( soundEffectsVolume ); } } // Sets the volume level for sound effects

	void SetMusicPause(bool isPause) { for (auto music : musicTracks) { music.second->setIsPaused( isPause ); } } // Sets the ispuase  for music

	//eventsystem
	void AddtoEvent();
	void HandleEvent(Event* event);

	static Sound* Instance();

private:

	Sound();
	~Sound();

	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	irrklang::vec3df camPosition; // Camera's position
	irrklang::vec3df camLookDir; // Camera's rotation around the Y axis, used for look direction

	std::map<std::string, irrklang::ISound*> musicTracks; // Stores each music track
	std::map<std::string, irrklang::ISoundSource*> soundEffects; // Stores each of the sound effects

	float musicVolume;
	float soundEffectsVolume;
	std::string currentMusicTrack;

	// Bools for shutting off sound
	bool musicOn;
	bool soundEffectsOn;
	bool masterOn;
};

#endif
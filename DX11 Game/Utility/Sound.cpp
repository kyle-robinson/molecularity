#include "stdafx.h"
#include "Sound.h"

// Currently if a sound is played from a different class you need to update its volume and camera position in that class, I've tried to find a fix but I've spent too much time on this and burnt myself out

Sound::Sound()
{
	camPosition = irrklang::vec3df( 0.0f, 9.0f, -15.0f );
	camLookDir = irrklang::vec3df( 0.0f, 0.0f, 1.0f );

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );

	AddtoEvent();
	musicVolume = 1.0f;
	soundEffectsVolume = 1.0f;
	currentMusicTrack = "MenuMusic";

	musicOn = true;
	soundEffectsOn = true;
	masterOn = true;
}

Sound::~Sound()
{
	engine->drop();
}

void Sound::InitialiseMusicTrack( const char* fileLocation, std::string musicName )
{
	musicTracks.emplace( musicName, engine->play2D( fileLocation, true, true, true ) );
}

void Sound::InitialiseSoundEffect( const char* fileLocation, std::string soundName )
{
	soundEffects.emplace( soundName, engine->addSoundSourceFromFile( fileLocation ) );
}

void Sound::ClearAudio()
{
	if (musicTracks.size() > 0)
	{
		for (auto name : musicTracks)
		{
			name.second->setIsPaused( true );
			name.second->drop();
		}

		musicTracks.clear();
	}

	if ( soundEffects.size() > 0 )
	{
		RemoveSoundEffects();
		soundEffects.clear();
	}
}

void Sound::UpdatePosition( XMFLOAT3 position, float rotation )
{
	camPosition = irrklang::vec3df( position.x, position.y, position.z );
	camLookDir = irrklang::vec3df( sin( rotation ), 0.0f, cos( rotation ) ); // Gets the player's look direction from the camera's rotation

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );
}

void Sound::PlayMusic( std::string musicName, bool loops )
{
	if ( masterOn )
	{
		for ( auto music : musicTracks )
		{
			if ( music.second->getIsPaused() == false )
				music.second->setIsPaused( true );
		}

		currentMusicTrack = musicName;

		if ( musicTracks.size() > 0 )
		{
			musicTracks[musicName]->setIsLooped( loops );
			musicTracks[musicName]->setIsPaused( false );
		}
	}
}

void Sound::PlaySoundEffects( std::string soundName, bool loops, XMFLOAT3 soundPosition, float minDistance )
{
	if ( masterOn )
	{
		if ( soundEffectsOn )
		{
			if ( soundPosition.x == NULL && soundPosition.y == NULL && soundPosition.z == NULL )
				engine->play2D( soundEffects[soundName] );
			else
			{
				soundEffects[soundName]->setDefaultMinDistance( minDistance );
				engine->play3D( soundEffects[soundName], irrklang::vec3df( soundPosition.x, soundPosition.y, soundPosition.z ), loops );
			}
		}
	}
}
void Sound::AddtoEvent()
{
	EventSystem::Instance()->AddClient( EVENTID::UpdateSettingsEvent, this );
}
#include<JSON_Helper.h>

void Sound::HandleEvent( Event* event )
{
	
	switch (event->GetEventID())
	{

	case EVENTID::UpdateSettingsEvent:
	{
		//controls 
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>( event->GetData() );
		float soundVol = 1.0f;
		bool MusicOn = false;
		for ( auto& setting : a )
		{
			if ( setting.Type == JSON::SettingType::SoundType )
			{
				//change sound
				if ( setting.Name == "SoundOn" ) {
					//music
					masterOn = std::get<bool>( setting.Setting );
					continue;
					
				}
				if ( setting.Name == "SoundVolume" ) {
					 soundVol = ( float )std::get<int>( setting.Setting ) / 100;
					 continue;
				}
				if ( setting.Name == "MusicOn" ) {
					musicOn = std::get<bool>( setting.Setting );
					MusicOn = std::get<bool>( setting.Setting );
					continue;
				}
				if ( setting.Name == "MusicVolume" ) {
					musicVolume = ( float )std::get<int>( setting.Setting ) / 100;
					continue;
				}
				if ( setting.Name == "BackgroundSoundsOn" ) {
					soundEffectsOn = std::get<bool>( setting.Setting );
					continue;
				}
				if ( setting.Name == "BackgroundSoundsVolume" ) {
					soundEffectsVolume = ( float )std::get<int>( setting.Setting ) / 100;
					continue;
				}

			}

		}
		SetMusicVolume( musicVolume* soundVol );
		SetMusicPause( true );
		SetSoundEffectsVolume( soundEffectsVolume* soundVol );
		//stop all sound
		if ( masterOn ) {
			if ( musicOn ) {
				PlayMusic( GetCurrentMusicTrack() );
			}
		}
	}
	break;
	}
}
#include "stdafx.h"
#include "Sound.h"

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
	musicTracks[musicName]->setVolume(musicVolume);
}

void Sound::InitialiseSoundEffect( const char* fileLocation, std::string soundName )
{
	soundEffects.emplace( soundName, engine->addSoundSourceFromFile( fileLocation ) );
	soundEffects[soundName]->setDefaultVolume(soundEffectsVolume);
}

void Sound::ClearAudio()
{
	if (musicTracks.size() > 0)
	{
		for ( auto name : musicTracks )
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
			musicTracks[musicName]->setIsLooped(loops);
			musicTracks[musicName]->setIsPaused(false);
		}
	}
}

void Sound::PlaySoundEffect( std::string soundName, bool loops, XMFLOAT3 soundPosition, float minDistance )
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
	
	switch ( event->GetEventID() )
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
				if (setting.Name == "MasterSoundOn") {
					//music
					masterOn = std::get<bool>( setting.Setting );
					continue;
					
				}
				if (setting.Name == "MasterSoundVolume") {
					 soundVol = (float)std::get<int>(setting.Setting) / 100;
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
				if (setting.Name == "SoundEffectsEfOn") {
					soundEffectsOn = std::get<bool>(setting.Setting);
					continue;
				}
				if (setting.Name == "SoundEffectVolume") {
					soundEffectsVolume = (float)std::get<int>(setting.Setting) / 100;
					continue;
				}

			}

		}
		float mVol = musicVolume * soundVol;
		SetMusicVolume( mVol );
		SetMusicPause( true );
		float sEVol = soundEffectsVolume * soundVol;
		SetSoundEffectsVolume( sEVol );
		//stop all sound
		if (masterOn) {
			if (musicOn) {
				PlayMusic(GetCurrentMusicTrack());
			}
		}
	}
	break;
	}
}

Sound* Sound::Instance()
{
	static Sound instance;
	return &instance;
}

#include "stdafx.h"
#include "Sound.h"
#include "JSON_Helper.h"

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

void Sound::InitialiseMusicTrack( std::string fileName, std::string fileType )
{
	musicTracks.emplace( fileName, engine->play2D( ( "Resources\\Audio\\Music\\" + fileName + fileType ).c_str(), true, true, true ) );
	musicTracks[fileName]->setVolume( musicVolume );
}

void Sound::InitialiseSoundEffect( std::string fileName, std::string fileType )
{
	soundEffects.emplace( fileName, engine->addSoundSourceFromFile( ( "Resources\\Audio\\Sounds\\" + fileName + fileType ).c_str() ) );
	soundEffects[fileName]->setDefaultVolume( soundEffectsVolume );
}

void Sound::InitialiseSoundGroup( std::string groupName )
{
	if ( groupName == "Player" )
	{
		Sound::Instance()->InitialiseSoundEffect( "ToolUse" );
		Sound::Instance()->InitialiseSoundEffect( "ToolNoEnergy" );
		Sound::Instance()->InitialiseSoundEffect( "ToolChange" );
		Sound::Instance()->InitialiseSoundEffect( "ToolSwitchMode" );
	}
	else if ( groupName == "Cube" )
	{
		Sound::Instance()->InitialiseSoundEffect( "CubePickup" );
		Sound::Instance()->InitialiseSoundEffect( "CubeThrow" );
		Sound::Instance()->InitialiseSoundEffect( "CubeCollision" );
		Sound::Instance()->InitialiseSoundEffect( "CubeSplash" );
	}
}

void Sound::ClearAudio()
{
	if ( musicTracks.size() > 0 )
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
		if ( musicOn )
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
}

void Sound::PlaySoundEffect( std::string soundName, bool loops, XMFLOAT3 soundPosition, float minDistance )
{
	if ( masterOn )
	{
		if ( soundEffectsOn && !engine->isCurrentlyPlaying( soundEffects[soundName] ) )
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

void Sound::HandleEvent( Event* event )
{

	switch ( event->GetEventID() )
	{
	case EVENTID::UpdateSettingsEvent:
	{
		// Controls
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>( event->GetData() );
		float soundVol = 1.0f;
		for ( auto& setting : a )
		{
			if ( setting.Type == JSON::SettingType::SoundType )
			{
				// Change sound
				if ( setting.Name == "MasterSoundOn" ) {
					// Music
					masterOn = std::get<bool>( setting.Setting );
					continue;
				}
				if ( setting.Name == "MasterSoundVolume" ) {
					soundVol = static_cast<float>( std::get<int>( setting.Setting ) ) / 100.0f;
					continue;
				}
				if ( setting.Name == "MusicOn" )
				{
					musicOn = std::get<bool>( setting.Setting );
					continue;
				}
				if ( setting.Name == "MusicVolume" ) {
					musicVolume = static_cast<float>( std::get<int>( setting.Setting ) ) / 100.0f;
					continue;
				}
				if ( setting.Name == "SoundEffectsEfOn" ) {
					soundEffectsOn = std::get<bool>( setting.Setting );
					continue;
				}
				if ( setting.Name == "SoundEffectVolume" )
				{
					soundEffectsVolume = static_cast<float>( std::get<int>( setting.Setting ) ) / 100.0f;
					continue;
				}
			}
		}
		
		float mVol = musicVolume * soundVol;
		SetMusicVolume( mVol );
		SetMusicPause( true );
		float sEVol = soundEffectsVolume * soundVol;
		SetSoundEffectsVolume( sEVol );
		
		// Stop all sounds
		if ( masterOn )
			if ( musicOn )
				PlayMusic( GetCurrentMusicTrack() );
	}
	break;
	}
}

Sound* Sound::Instance()
{
	static Sound instance;
	return &instance;
}
#include "stdafx.h"
#include "Sound.h"

// Currently if a sound is played from a different class you need to update its volume and camera position in that class, I've tried to find a fix but I've spent too much time on this and burnt myself out

Sound::Sound()
{
	camPosition = irrklang::vec3df( 0.0f, 9.0f, -15.0f );
	camLookDir = irrklang::vec3df( 0.0f, 0.0f, 1.0f );

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );

	InitialiseSounds();
	AddtoEvent();
	musicVolume = 1.0f;
	soundEffectsVolume = 1.0f;
}

Sound::~Sound()
{
	engine->drop();
}

HRESULT Sound::InitialiseSounds()
{
	// Music
	musicVec.push_back( engine->play2D( "Resources\\Audio\\Music\\LevelMusic.mp3", true, true, true ) );
	musicVec.push_back( engine->play2D( "Resources\\Audio\\Music\\MenuMusic.mp3", true, true, true ) );

	// Sound effects
	soundEffectsVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Sounds\\Shot.wav" ) );
	soundEffectsVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Sounds\\Collision.wav" ) );

	return S_OK;
}

HRESULT Sound::UpdatePosition( XMFLOAT3 position, float rotation )
{
	camPosition = irrklang::vec3df( position.x, position.y, position.z );
	camLookDir = irrklang::vec3df( sin( rotation ), 0.0f, cos( rotation ) ); // Gets the player's look direction from the camera's rotation

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );

	return S_OK;
}

HRESULT Sound::PlayMusic( int musicNum, bool loops )
{
	for (int i = 0; i < musicVec.size(); i++) // Stops music overlapping
	{
		if ( musicVec[i]->getIsPaused() == false )
			musicVec[i]->setIsPaused( true );
	}

	currentMusicTrack = musicNum;

	musicVec[musicNum]->setPlayPosition( 0 );
	musicVec[musicNum]->setIsLooped( loops );
	musicVec[musicNum]->setIsPaused( false );
  
	return S_OK;
}

HRESULT Sound::PlaySoundEffects( int soundNum, bool loops, XMFLOAT3 soundPosition, float minDistance )
{
	if ( soundPosition.x == NULL && soundPosition.y == NULL && soundPosition.z == NULL)
		engine->play2D( soundEffectsVec[soundNum] );
	else
	{
		soundEffectsVec[soundNum]->setDefaultMinDistance( minDistance );
		engine->play3D( soundEffectsVec[soundNum], irrklang::vec3df( soundPosition.x, soundPosition.y, soundPosition.z ), loops );
	}
	return S_OK;
}
void Sound::AddtoEvent()
{
	EventSystem::Instance()->AddClient(EVENTID::UpdateSettingsEvent, this);
}
#include<JSON_Helper.h>

void Sound::HandleEvent(Event* event)
{
	
	switch (event->GetEventID())
	{

	case EVENTID::UpdateSettingsEvent:
	{
		//controlls 
		std::vector<JSON::SettingData> a = *static_cast<std::vector<JSON::SettingData>*>(event->GetData());
		float soundVol=1.0f;
		bool MusicOn=false;
		for (auto& setting : a)
		{
			if (setting.Type == JSON::SettingType::SoundType)
			{
				//change sound
				if (setting.Name == "SoundOn") {
					//music
					soundON = std::get<bool>(setting.Setting);
					continue;
					
				}
				if (setting.Name == "SoundVolume") {
					 soundVol = (float)std::get<int>(setting.Setting) / 100;
					 continue;
				}
				if (setting.Name == "MusicOn") {
					musicOn = std::get<bool>(setting.Setting);
					MusicOn= std::get<bool>(setting.Setting);
					continue;
				}
				if (setting.Name == "MusicVolume") {
					musicVolume = (float)std::get<int>(setting.Setting) / 100;
					continue;
				}
				if (setting.Name == "BackgroundSoundsOn") {
					SoundEffectsOn = std::get<bool>(setting.Setting);
					continue;
				}
				if (setting.Name == "BackgroundSoundsVolume") {
					soundEffectsVolume = (float)std::get<int>(setting.Setting) / 100;
					continue;
				}

			}

		}

		SetMusicVolume(musicVolume* soundVol);
		SetMusicPause(true);
		SetSoundEffectsVolume(soundEffectsVolume* soundVol);
		//stop all sound
		if (soundON) {
			if (musicOn) {
				SetMusicPause(false);
							
			}
		}
	}
	break;
	}
}
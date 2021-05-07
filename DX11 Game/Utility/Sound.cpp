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
	SoundEffectsVolume = 1.0f;
}

Sound::~Sound()
{
	engine->drop();
}

HRESULT Sound::InitialiseSounds()
{
	// Music
	//musicVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Music.wav" ) );

	musicVec.push_back(engine->play2D("Resources\\Audio\\Music.wav", true, true, true));

	// Sound effects
	SoundEffectsVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Shot.wav" ) );
	SoundEffectsVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Collision.wav" ) );

	return S_OK;
}

HRESULT Sound::UpdatePosition( XMFLOAT3 position, float rotation )
{
	camPosition = irrklang::vec3df( position.x, position.y, position.z );
	camLookDir = irrklang::vec3df( sin( rotation ), 0.0f, cos( rotation ) ); // Gets look direction from camera's rotation

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );

	return S_OK;
}

HRESULT Sound::PlayMusic( int musicNum, bool loops )
{
	//engine->play2D( musicVec[musicNum], loops, false, false, true );
	musicVec[musicNum]->setIsLooped(loops);
	musicVec[musicNum]->setIsPaused(false);
	return S_OK;
}

HRESULT Sound::PlaySoundEffects(int soundNum, XMFLOAT3 soundPosition)
{

	if (SoundEffectsOn) {
		if (soundPosition.x == NULL)
			engine->play2D(SoundEffectsVec[soundNum]);
		else
		{
			irrklang::vec3df position = { soundPosition.x, soundPosition.y, soundPosition.z };
			engine->play3D(SoundEffectsVec[soundNum], position);
		}
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
					SoundEffectsVolume = (float)std::get<int>(setting.Setting) / 100;
					continue;
				}

			}

		}
					SetMusicVolume(musicVolume* soundVol);
					//musicVec set is puased
					musicVec[0]->setIsPaused(true);
					musicVec[0]->setIsPaused(false);
					SetSoundEffectsVolume(SoundEffectsVolume* soundVol);






	}
	break;
	}
}

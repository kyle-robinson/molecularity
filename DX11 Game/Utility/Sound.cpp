#include "stdafx.h"
#include "Sound.h"

// Currently if a sound is played from a different class you need to update its volume and camera position in that class, I've tried to find a fix but I've spent too much time on this and burnt myself out

Sound::Sound()
{
	camPosition = irrklang::vec3df( 0.0f, 9.0f, -15.0f );
	camLookDir = irrklang::vec3df( 0.0f, 0.0f, 1.0f );

	engine->setListenerPosition( camPosition, camLookDir, irrklang::vec3df( 0.0f, 0.0f, 0.0f ), irrklang::vec3df( 0.0f, 1.0f, 0.0f ) );

	InitialiseSounds();

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
	musicVec.push_back( engine->addSoundSourceFromFile( "Resources\\Audio\\Music.wav" ) );

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
	engine->play2D( musicVec[musicNum], loops, false, false, true );
	return S_OK;
}

HRESULT Sound::PlaySoundEffects( int soundNum, XMFLOAT3 soundPosition )
{
	irrklang::vec3df position( soundPosition.x, soundPosition.y, soundPosition.z );
	engine->play3D( SoundEffectsVec[soundNum], position );

	return S_OK;
}

HRESULT Sound::PlaySoundEffects( int soundNum )
{
	engine->play3D( SoundEffectsVec[soundNum], camPosition );
	return S_OK;
}

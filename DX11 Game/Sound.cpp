#include "stdafx.h"
#include "Sound.h"

Sound::Sound()
{
	camPosition = irrklang::vec3df(0.0f, 9.0f, -15.0f);
	camLookDir = irrklang::vec3df(0.0f, 0.0f, 1.0f);

	engine->setListenerPosition(camPosition, camLookDir, irrklang::vec3df(0.0f, 0.0f, 0.0f), irrklang::vec3df(0.0f, 1.0f, 0.0f));

	InitialiseSounds();

	SetMusicVolume(0.1f);
	SetSFXVolume(0.3f);
}

Sound::~Sound()
{
	engine->drop();
}

HRESULT Sound::InitialiseSounds()
{
	// Music
	musicVec.push_back(engine->addSoundSourceFromFile("Resources\\Audio\\Music.wav"));

	// Sound effects
	SFXVec.push_back(engine->addSoundSourceFromFile("Resources\\Audio\\Shot.wav"));
	SFXVec.push_back(engine->addSoundSourceFromFile("Resources\\Audio\\Collision.wav"));

	return S_OK;
}

HRESULT Sound::UpdatePosition(XMFLOAT3 position, float rotation)
{
	camPosition = irrklang::vec3df(position.x, position.y, position.z);
	camLookDir = irrklang::vec3df (sin(rotation), 0.0f, cos(rotation)); // Gets look direction from camera's rotation

	engine->setListenerPosition(camPosition, camLookDir, irrklang::vec3df(0.0f, 0.0f, 0.0f), irrklang::vec3df(0.0f, 1.0f, 0.0f));

	return S_OK;
}

HRESULT Sound::PlayMusic(int musicNum, bool loops)
{
	engine->play2D(musicVec[musicNum], loops, false, false, true);
	return S_OK;
}

HRESULT Sound::PlaySoundEffect(int soundNum, XMFLOAT3 soundPosition)
{
	irrklang::vec3df position(soundPosition.x, soundPosition.y, soundPosition.z);
	engine->play3D(SFXVec[soundNum], position);

	return S_OK;
}

HRESULT Sound::PlaySoundEffect(int soundNum)
{
	engine->play3D(SFXVec[soundNum], camPosition);
	return S_OK;
}
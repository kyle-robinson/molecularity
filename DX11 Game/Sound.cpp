#include "stdafx.h"
#include "Sound.h"

Sound::Sound()
{

}

Sound::~Sound()
{

}

bool Sound::Initialise()
{
	return false;
}

void Sound::Close()
{

}

bool Sound::InitialiseDirectSound()
{
	return false;
}

void Sound::CloseDirectSound()
{

}

bool Sound::LoadWavFile(char*, IDirectSoundBuffer8**)
{
	return false;
}

void Sound::CloseWavFile(IDirectSoundBuffer8**)
{

}

bool Sound::PlayWavFile()
{
	return false;
}

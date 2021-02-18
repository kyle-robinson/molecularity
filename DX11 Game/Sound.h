#pragma once
#include <dsound.h>

class Sound
{
private:
	Sound();
	~Sound();

	bool Initialise();
	void Close();

	bool InitialiseDirectSound();
	void CloseDirectSound();

	bool LoadWavFile(char*, IDirectSoundBuffer8**);
	void CloseWavFile(IDirectSoundBuffer8**);

	bool PlayWavFile();

public:
	IDirectSound8* _directSound;
	IDirectSoundBuffer8* _primaryBuffer;
	IDirectSoundBuffer8* _secondaryBuffer1;
};


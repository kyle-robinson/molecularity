#include "stdafx.h"
#include "Sound.h"
#include "..//Input//CameraMovement.h"

Sound::Sound()
{
	_directSound = 0;
	_primaryBuffer = 0;
	_listener = 0;
}

bool Sound::Initialize(HWND hwnd)
{
	bool result;
	
	// Initialize DirectSound API and the primary buffer
	result = InitializeDirectSound(hwnd);
	if ( !result ) return false;

	result = InitialiseSoundFiles();
	if ( !result ) return false;

	return true;
}

void Sound::Close()
{
	// Frees the secondary buffers
	for (int i = 0; i < (sizeof(_secondaryBuffer) / sizeof(_secondaryBuffer[0])); i++)
		CloseWavFile(&_secondaryBuffer[i], &_secondary3DBuffer[i]);
	CloseDirectSound();
}

bool Sound::InitializeDirectSound(HWND hwnd)
{
	try
	{
		// Handles getting an interface pointer to DirectSound and the default primary buffer
		HRESULT result;
		DSBUFFERDESC bufferDesc;
		WAVEFORMATEX waveFormat;

		result = DirectSoundCreate8(NULL, &_directSound, NULL);
		COM_ERROR_IF_FAILED(result, "DirectSoundCreate8 failure!" );

		result = _directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
		COM_ERROR_IF_FAILED(result, "Failed to create cooperative level!" );

		// How we access the primary buffer
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = NULL;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		// Get control of primary buffer on default sound device (emulated)
		result = _directSound->CreateSoundBuffer(&bufferDesc, &_primaryBuffer, NULL);
		COM_ERROR_IF_FAILED(result, "Failed to create sound buffer!" );

		// Setup primary buffer
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		// Set primary buffer to the wave format
		result = _primaryBuffer->SetFormat(&waveFormat);
		COM_ERROR_IF_FAILED(result, "Failed to set the audio file format!" );

		// Obtain listener interface
		result = _primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&_listener);
		COM_ERROR_IF_FAILED(result, "Failed to get listener interface!" );

		//Initial position set to origin
		_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);
	}

	catch (COMException& exception)
	{
		ErrorLogger::Log( "Could not initialize Direct Sound!" );
		return false;
	}

	return true;
}

void Sound::CloseDirectSound()
{
	if (_listener)
	{
		_listener->Release();
		_listener = 0;
	}

	if (_primaryBuffer)
	{
		_primaryBuffer->Release();
		_primaryBuffer = 0;
	}

	if (_directSound)
	{
		_directSound->Release();
		_directSound = 0;
	}
}

bool Sound::InitialiseSoundFiles()
{
	// Load a WAV file and initialize the secondary buffer
	bool result = Load2DWavFile( "Resources\\Audio\\Music.wav", &_secondaryBuffer[MAIN_MUSIC] );
	if ( !result ) return false;

	result = Load3DWavFile( "Resources\\Audio\\MonoShot.wav", &_secondaryBuffer[SHOT_SOUND], &_secondary3DBuffer[SHOT_SOUND] );
	if ( !result ) return false;

	result = Load3DWavFile( "Resources\\Audio\\Collision.wav", &_secondaryBuffer[COLLISION_SOUND], &_secondary3DBuffer[COLLISION_SOUND] );
	if ( !result ) return false;

	return true;
}

bool Sound::Load2DWavFile(const char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, fileName, "rb");
	if (error != 0)
		return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	//Check if loaded sound is correct
	if ((waveFileHeader.chunkID[0] != 'R') ||
		(waveFileHeader.chunkID[1] != 'I') ||
		(waveFileHeader.chunkID[2] != 'F') ||
		(waveFileHeader.chunkID[3] != 'F'))
		return false;

	if ((waveFileHeader.format[0] != 'W') ||
		(waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') ||
		(waveFileHeader.format[3] != 'E'))
		return false;

	if ((waveFileHeader.subChunkID[0] != 'f') ||
		(waveFileHeader.subChunkID[1] != 'm') ||
		(waveFileHeader.subChunkID[2] != 't') ||
		(waveFileHeader.subChunkID[3] != ' '))
		return false;

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	if (waveFileHeader.numChannels != 2)
		return false;

	if (waveFileHeader.sampleRate != 44100)
		return false;

	if (waveFileHeader.bitsPerSample != 16)
		return false;

	if ((waveFileHeader.dataChunkID[0] != 'd') ||
		(waveFileHeader.dataChunkID[1] != 'a') ||
		(waveFileHeader.dataChunkID[2] != 't') ||
		(waveFileHeader.dataChunkID[3] != 'a'))
		return false;

	//Setup secondary buffer and load the audio data
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	//Create secondary buffer
	result = _directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
		return false;

	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	tempBuffer->Release();
	tempBuffer = 0;

	//Now load the audio file
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	error = fclose(filePtr);
	if (error != 0)
		return false;

	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
		return false;

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
		return false;

	delete[]waveData;
	waveData = 0;

	return true;
}

bool Sound::Load3DWavFile(const char* fileName, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	error = fopen_s(&filePtr, fileName, "rb");
	if ( error != 0 ) return false;

	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if ( count != 1 ) return false;

	// Check if loaded sound is correct
	if ((waveFileHeader.chunkID[0] != 'R') ||
		(waveFileHeader.chunkID[1] != 'I') ||
		(waveFileHeader.chunkID[2] != 'F') ||
		(waveFileHeader.chunkID[3] != 'F'))
		return false;

	if ((waveFileHeader.format[0] != 'W') ||
		(waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') ||
		(waveFileHeader.format[3] != 'E'))
		return false;

	if ((waveFileHeader.subChunkID[0] != 'f') ||
		(waveFileHeader.subChunkID[1] != 'm') ||
		(waveFileHeader.subChunkID[2] != 't') ||
		(waveFileHeader.subChunkID[3] != ' '))
		return false;

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	// Make sure audio is mono
	if (waveFileHeader.numChannels != 1)
		return false;

	if (waveFileHeader.sampleRate != 44100)
		return false;

	if (waveFileHeader.bitsPerSample != 16)
		return false;

	if ((waveFileHeader.dataChunkID[0] != 'd') ||
		(waveFileHeader.dataChunkID[1] != 'a') ||
		(waveFileHeader.dataChunkID[2] != 't') ||
		(waveFileHeader.dataChunkID[3] != 'a'))
		return false;

	try
	{
		// Setup secondary buffer and load the audio data
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 1;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
		bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		result = _directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
		COM_ERROR_IF_FAILED( result, "Failed to create sound buffer!" );

		// Create secondary buffer
		result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
		COM_ERROR_IF_FAILED( result, "Failed to get secondary buffer interface!" );

		tempBuffer->Release();
		tempBuffer = 0;

		// Load the audio files
		fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

		waveData = new unsigned char[waveFileHeader.dataSize];
		if ( !waveData ) return false;

		count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
		if ( count != waveFileHeader.dataSize ) return false;

		error = fclose(filePtr);
		if ( error != 0 ) return false;

		result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
		COM_ERROR_IF_FAILED( result, "Failed to lock secondary buffer!" );

		memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

		result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
		COM_ERROR_IF_FAILED( result, "Failed to unlock secondary buffer!" );

		result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
		COM_ERROR_IF_FAILED( result, "Failed to get secondary buffer interface!" );

		delete[] waveData;
		waveData = 0;
	}

	catch (COMException& exception)
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

void Sound::CloseWavFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
}

/// <param name="num">- Enum name for sound clip -</param>
/// <param name="volume">- Local volume for sound clip -</param>
/// <param name="positionX">- X position for 3D sound clip -</param>
/// <param name="positionY">- Y position for 3D sound clip -</param>
/// <param name="position|">- Z position for 3D sound clip -</param>
bool Sound::PlayWavFile(int num, float volume, XMFLOAT3 soundPos)
{
	HRESULT result;

	try
	{
		result = _secondaryBuffer[num]->SetCurrentPosition(0);
		COM_ERROR_IF_FAILED(result, "Failed to set position of secondary buffer!");

		result = _secondaryBuffer[num]->SetVolume(DSBVOLUME_MIN + (10000 * volume));
		COM_ERROR_IF_FAILED(result, "Failed to set the volume!");

		result = _secondary3DBuffer[num]->SetPosition(soundPos.x - camPosX, soundPos.y - camPosY, soundPos.z - camPosZ, DS3D_IMMEDIATE);
		COM_ERROR_IF_FAILED(result, "Failed to set the position of the 3D secondary buffer!");

		std::string tempMessage = std::to_string(camPosX);
		char sz[1024] = { 0 };
		tempMessage = tempMessage + "\n";
		const char* c = tempMessage.c_str();
		sprintf_s(sz, c);
		OutputDebugStringA(sz);

		result = _secondaryBuffer[num]->Play(0, 0, 0);
		COM_ERROR_IF_FAILED(result, "Failed to play the sound from the secondary buffer!");
	}

	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

bool Sound::PlayWavFile(int num, float volume)
{
	HRESULT result;

	try
	{
		result = _secondaryBuffer[num]->SetCurrentPosition(0);
		COM_ERROR_IF_FAILED(result, "Failed to set position of secondary buffer!");

		result = _secondaryBuffer[num]->SetVolume(DSBVOLUME_MIN + (10000 * volume));
		COM_ERROR_IF_FAILED(result, "Failed to set the volume!");

		result = _secondaryBuffer[num]->Play(0, 0, 0);
		COM_ERROR_IF_FAILED(result, "Failed to play the sound from the secondary buffer!");
	}

	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}
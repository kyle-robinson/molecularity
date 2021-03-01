#include "stdafx.h"
#include "Sound.h"

Sound::Sound()
{
	_directSound = 0;
	_primaryBuffer = 0;
}

bool Sound::Initialize( HWND hwnd )
{
	// Initialize DirectSound API and the primary buffer
	bool result = InitializeDirectSound( hwnd );
	if ( !result ) return false;

	// Initializes all the sound files into their own secondary buffers
	result = InitialiseSoundFiles();
	if ( !result ) return false;

	return true;
}

void Sound::Close()
{
	// Frees the secondary buffer
	for (int i = 0; i < (sizeof(&_secondaryBuffer)/sizeof(&_secondaryBuffer[0])); i++)
		CloseWavFile( &_secondaryBuffer[i] );
	CloseDirectSound();
}

bool Sound::InitializeDirectSound( HWND hwnd )
{
	try
	{
		// Handles getting an interface pointer to DirectSound and default primary buffer
		HRESULT result = DirectSoundCreate8( NULL, &_directSound, NULL );
		COM_ERROR_IF_FAILED( result, "DirectSoundCreate8 failure!" );

		result = _directSound->SetCooperativeLevel( hwnd, DSSCL_PRIORITY );
		COM_ERROR_IF_FAILED( result, "Failed to set cooperative level!" );

		//How we access the primary buffer
		DSBUFFERDESC bufferDesc = { 0 };
		bufferDesc.dwSize = sizeof( DSBUFFERDESC );
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = NULL;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		//Get control of primary buffer on default sound device
		result = _directSound->CreateSoundBuffer( &bufferDesc, &_primaryBuffer, NULL );
		COM_ERROR_IF_FAILED( result, "Failed to create sound buffer!" );

		//Setup primary buffer
		WAVEFORMATEX waveFormat = { 0 };
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = ( waveFormat.wBitsPerSample / 8 ) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		result = _primaryBuffer->SetFormat( &waveFormat );
		COM_ERROR_IF_FAILED( result, "Failed to set the audio file format!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( "Could not initialize Direct Sound!" );
		return false;
	}

	return true;
}

void Sound::CloseDirectSound()
{
	if ( _primaryBuffer )
	{
		_primaryBuffer->Release();
		_primaryBuffer = 0;
	}

	if ( _directSound )
	{
		_directSound->Release();
		_directSound = 0;
	}
}

bool Sound::InitialiseSoundFiles()
{
	// Load a WAV file and initialize the secondary buffer
	bool result = LoadWavFile( "Resources\\Audio\\Music.wav", &_secondaryBuffer[MAIN_MUSIC] );
	if ( !result ) return false;

	result = LoadWavFile( "Resources\\Audio\\Shot.wav", &_secondaryBuffer[SHOT_SOUND] );
	if ( !result ) return false;

	return true;
}

bool Sound::LoadWavFile( const char* fileName, IDirectSoundBuffer8** secondaryBuffer )
{
	FILE* filePtr;
	int error = fopen_s( &filePtr, fileName, "rb" );
	if ( error != 0 ) return false;

	WaveHeaderType waveFileHeader;
	unsigned int count = fread( &waveFileHeader, sizeof( waveFileHeader ), 1, filePtr );
	if ( count != 1 ) return false;

	//Check if loaded sound is correct
	if ( ( waveFileHeader.chunkID[0] ) != 'R' ||
		( waveFileHeader.chunkID[1] != 'I' ) ||
		( waveFileHeader.chunkID[2] != 'F' ) ||
		( waveFileHeader.chunkID[3] != 'F' ) )
		return false;

	if ( ( waveFileHeader.format[0]) != 'W' ||
		( waveFileHeader.format[1] != 'A' ) ||
		( waveFileHeader.format[2] != 'V' ) ||
		( waveFileHeader.format[3] != 'E' ) )
		return false;

	if ( ( waveFileHeader.subChunkID[0]) != 'f' ||
		( waveFileHeader.subChunkID[1] != 'm' ) ||
		( waveFileHeader.subChunkID[2] != 't' ) ||
		( waveFileHeader.subChunkID[3] != ' ' ) )
		return false;

	if ( waveFileHeader.audioFormat != WAVE_FORMAT_PCM )
		return false;

	if ( waveFileHeader.numChannels != 2 )
		return false;

	if ( waveFileHeader.sampleRate != 44100 )
		return false;

	if ( waveFileHeader.bitsPerSample != 16 )
		return false;

	if ( ( waveFileHeader.dataChunkID[0]) != 'd' ||
		( waveFileHeader.dataChunkID[1] != 'a' ) ||
		( waveFileHeader.dataChunkID[2] != 't' ) ||
		( waveFileHeader.dataChunkID[3] != 'a' ) )
		return false;

	try
	{
		// Setup secondary buffer and load the audio data
		WAVEFORMATEX waveFormat = { 0 };
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = ( waveFormat.wBitsPerSample / 8 ) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		DSBUFFERDESC bufferDesc = { 0 };
		bufferDesc.dwSize = sizeof( DSBUFFERDESC );
		bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = &waveFormat;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		// Create secondary buffer
		IDirectSoundBuffer* tempBuffer;
		HRESULT result = _directSound->CreateSoundBuffer( &bufferDesc, &tempBuffer, NULL );
		COM_ERROR_IF_FAILED( result, "Failed to create sound buffer!" );

		result = tempBuffer->QueryInterface( IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer );
		COM_ERROR_IF_FAILED( result, "Failed to query sound interface!" );

		tempBuffer->Release();
		tempBuffer = 0;

		//Now load the audio file
		fseek( filePtr, sizeof( WaveHeaderType ), SEEK_SET );
		unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
		if ( !waveData ) return false;

		count = fread( waveData, 1, waveFileHeader.dataSize, filePtr );
		if ( count != waveFileHeader.dataSize ) return false;

		error = fclose( filePtr );
		if ( error != 0 ) return false;

		unsigned char* bufferPtr;
		unsigned long bufferSize;
		result = ( *secondaryBuffer )->Lock( 0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		COM_ERROR_IF_FAILED( result, "Failed to lock secondary sound buffer!" );

		memcpy( bufferPtr, waveData, waveFileHeader.dataSize );

		result = ( *secondaryBuffer )->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		COM_ERROR_IF_FAILED( result, "Failed to unlock secondary sound buffer!" );

		delete[] waveData;
		waveData = 0;
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

void Sound::CloseWavFile( IDirectSoundBuffer8** secondaryBuffer )
{
	if ( *secondaryBuffer )
	{
		( *secondaryBuffer )->Release();
		*secondaryBuffer = 0;
	}
}

/// <param name="num">- Enum name for sound clip -</param>
/// <param name="volume">- Local volume for sound clip -</param>
bool Sound::PlayWavFile( int num, float volume )
{
	try
	{
		HRESULT result = _secondaryBuffer[num]->SetCurrentPosition(0);
		COM_ERROR_IF_FAILED( result, "Failed to set current position of secondary buffer!" );

		result = _secondaryBuffer[num]->SetVolume( DSBVOLUME_MIN + ( 10000 * volume ) );
		COM_ERROR_IF_FAILED( result, "Failed to set volume of secondary buffer!" );

		result = _secondaryBuffer[num]->Play( 0, 0, 0 );
		COM_ERROR_IF_FAILED( result, "Failed to play sound clip from secondary buffer!" );
	}
	catch ( COMException& exception )
	{
		ErrorLogger::Log( exception );
		return false;
	}

	return true;
}

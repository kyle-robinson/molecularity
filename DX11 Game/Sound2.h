#pragma once
#pragma comment(lib, "xaudio2")
//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#include <xaudio2.h>
#include <x3daudio.h>
#include <winnt.h>
#include <irrklang/irrKlang.h>

// Little-Endian
#define fourccRIFF 'FFIR'	//RIFF
#define fourccDATA 'atad'	//data
#define fourccFMT ' tmf'	//fmt
#define fourccWAVE 'EVAW'	//WAVE
#define fourccXWMA 'AMWX'	//XWMA
#define fourccDPDS 'sdpd'	//dpds

class Sound2
{
public:
	Sound2();
	~Sound2();

	HRESULT Initialise();
	HRESULT InitialiseAudioFiles();

	HRESULT LoadAudio(const TCHAR* strFileName, int bufferNum);
	HRESULT PlayAudio(int bufferNum, float volume);

	//void UpdateEmitter();
	//void UpdateListener();

	enum SOUND_NAMES
	{
		MAIN_MUSIC = 0,
		SHOT_SOUND,
		COLLISION_SOUND
	};

private:
	HRESULT FindChunkData(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferoffset);

	WAVEFORMATEX wfx[3] = { 0 };
	XAUDIO2_BUFFER buffer[3] = { 0 };

	IXAudio2* pXAudio2 = nullptr;
	IXAudio2MasteringVoice* pMasterVoice = nullptr;

	X3DAUDIO_HANDLE X3DInstance;
	X3DAUDIO_LISTENER pListener;
	X3DAUDIO_EMITTER pEmitter;
	X3DAUDIO_DSP_SETTINGS DSPSettings = { 0 };
};
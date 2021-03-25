#include "stdafx.h"
#include "Sound2.h"

Sound2::Sound2()
{
}

Sound2::~Sound2()
{
}

HRESULT Sound2::Initialise()
{
	HRESULT hr;

    //Initialise XAudio2
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

    IXAudio2MasteringVoice* pMasterVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

    //Initialise X3DAudio
    DWORD dwChannelMask;
    pMasterVoice->GetChannelMask(&dwChannelMask);

    X3DAUDIO_HANDLE X3DInstance;
    //X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);

    if (FAILED(hr = InitialiseAudioFiles()))
        return hr;

	return S_OK;
}

HRESULT Sound2::InitialiseAudioFiles()
{
    HRESULT hr;

    if (FAILED(hr = LoadAudio(TEXT("Resources\\Audio\\Music.wav"), MAIN_MUSIC)))
        return hr;

    if (FAILED(hr = LoadAudio(TEXT("Resources\\Audio\\Shot.wav"), SHOT_SOUND)))
        return hr;

    if (FAILED(hr = LoadAudio(TEXT("Resources\\Audio\\Collision.wav"), COLLISION_SOUND)))
        return hr;

    return S_OK;
}

HRESULT Sound2::FindChunkData(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                return HRESULT_FROM_WIN32(GetLastError());
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;

        if (bytesRead >= dwRIFFDataSize) return S_FALSE;

    }

    return S_OK;
}

HRESULT Sound2::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

HRESULT Sound2::LoadAudio(const TCHAR* strFileName, int bufferNum)
{
    HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
        return HRESULT_FROM_WIN32(GetLastError());

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkSize;
    DWORD dwChunkPosition;
    //Check the file type, should be fourccWAVE or 'XWMA'
    FindChunkData(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
    DWORD fileType;
    ReadChunkData(hFile, &fileType, sizeof(DWORD), dwChunkPosition);
    if (fileType != fourccWAVE)
        return S_FALSE;

    FindChunkData(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
    ReadChunkData(hFile, &wfx[bufferNum], dwChunkSize, dwChunkPosition);

    FindChunkData(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
    BYTE* pDataBuffer = new BYTE[dwChunkSize];
    ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

    buffer[bufferNum].AudioBytes = dwChunkSize;        //Size of the audio buffer in bytes
    buffer[bufferNum].pAudioData = pDataBuffer;        //Buffer containing the audio data
    buffer[bufferNum].Flags = XAUDIO2_END_OF_STREAM;   //Tell the source voice not to expect any data after this buffer

    return S_OK;
}

HRESULT Sound2::PlayAudio(int bufferNum, float volume)
{
    HRESULT hr;

    IXAudio2SourceVoice* pSourceVoice;
    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx[bufferNum])))
        return hr;

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer[bufferNum])))
        return hr;

    if (FAILED(hr = pSourceVoice->SetVolume(volume)))
        return hr;

    if (FAILED(hr = pSourceVoice->Start(0)))
        return hr;

    return S_OK;
}

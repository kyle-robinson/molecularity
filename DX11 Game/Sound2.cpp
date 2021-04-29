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

    
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

    //Initialise X3DAudio
    DWORD dwChannelMask;
    pMasterVoice->GetChannelMask(&dwChannelMask);

    X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);

    //Create listener and emitter instances
    pEmitter.ChannelCount = 1;
    pEmitter.CurveDistanceScaler = FLT_MIN;

    //Create instance of DSP settings
    XAUDIO2_VOICE_DETAILS voiceDetails;
    pMasterVoice->GetVoiceDetails(&voiceDetails);
    FLOAT32* matrix = new FLOAT32[voiceDetails.InputChannels];

    DSPSettings.SrcChannelCount = 1;
    DSPSettings.DstChannelCount = voiceDetails.InputChannels;
    DSPSettings.pMatrixCoefficients = matrix;

    ////Update the listener and emitter

    ////Calculate the new settings for the voice
    //X3DAudioCalculate(X3DInstance, &pListener, &pEmitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB, &DSPSettings);

    ////Apply volume and pitch to the voice

    ////Calculate reverb level

    ////Apply calculated low pass filter direct coefficient to the voice
    //XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };

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
    if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx[bufferNum], 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL)))
        return hr;

    if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer[bufferNum])))
        return hr;

    if (FAILED(hr = pSourceVoice->SetVolume(volume)))
        return hr;

    //Update the listener and emitter
    pEmitter.OrientFront = XMFLOAT3(0.0f, 0.0f, 1.0f);
    pEmitter.OrientTop = XMFLOAT3(0.0f, 1.0f, 0.0f);
    pEmitter.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    pEmitter.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    pListener.OrientFront = XMFLOAT3(0.0f, 0.0f, 1.0f);
    pListener.OrientTop = XMFLOAT3(0.0f, 1.0f, 0.0f);
    pListener.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    pListener.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

    //Calculate the new settings for the voice
    X3DAudioCalculate(X3DInstance, &pListener, &pEmitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB, &DSPSettings);

    XAUDIO2_VOICE_DETAILS voiceDetails;
    pSourceVoice->GetVoiceDetails(&voiceDetails);
    pSourceVoice->SetOutputMatrix(pSourceVoice, 1, voiceDetails.InputChannels, DSPSettings.pMatrixCoefficients);
    pSourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);
    //pSourceVoice->SetOutputMatrix(pSourceVoice, 1, 1, &DSPSettings.LPFDirectCoefficient);
    XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient) };
    pSourceVoice->SetFilterParameters(&FilterParameters);

    if (FAILED(hr = pSourceVoice->Start(0)))
        return hr;

    return S_OK;
}

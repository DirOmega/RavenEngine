#include "AudioEngine.h"

AudioEngine* AudioEngine::AEInstance = nullptr;

AudioEngine::~AudioEngine()
{
	//pMasterVoice->DestroyVoice();
	/*CloseHandle(hFile);
	pSourceVoice->DestroyVoice();*/

	pXAudio2->Release();
	pMasterVoice->DestroyVoice();
	//delete testFile;

	/*delete[] pDataBuffer;*/
	CoUninitialize();
}

AudioEngine* AudioEngine::getInstance()
{
	if (nullptr == AudioEngine::AEInstance)
	{
		AudioEngine::AEInstance = new AudioEngine();
	}
	return AudioEngine::AEInstance;
}

HRESULT AudioEngine::createSourceVoice(IXAudio2SourceVoice*& sv, WAVEFORMATEX& wfx, UINT32 flags, float fRatio, IXAudio2VoiceCallback& callbackContext)
{
	AudioEngine* ae = AudioEngine::getInstance();
	//ae->pXAudio2->CreateSourceVoice(&sv, &wfx, flags, fRatio, &callbackContext)
	HRESULT hr;
	if (FAILED(hr = ae->pXAudio2->CreateSourceVoice(&sv, &wfx, flags, fRatio, &callbackContext)))return hr;

	return 0;
}

AudioEngine::AudioEngine()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	startXAU2();
	getMasterVoice();

	//debugPlayWAV();
}

HRESULT AudioEngine::startXAU2()
{
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		assert(pXAudio2 != NULL);
		//printf("error %x", hr);
		Trace::out("error %x", hr);
		return hr;
	}
	return hr;
}

HRESULT AudioEngine::getMasterVoice()
{
	hr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice))) {
		//printf("error %x", hr);
		Trace::out("error %x", hr);
		return hr;
	}
	return hr;
}


//void InitializeXAudio2()
//{
//	// -----------------------------------------------------------------
//	// Audio Engine Setup
//	// -----------------------------------------------------------------
//
//	HRESULT	hr;
//
//	// Needed for COM initialization
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	// -------------------------------------------------------
//	// Create an instance, use its default processor
//	// -------------------------------------------------------
//	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
//	assert(hr == S_OK);
//	assert(pXAudio2);
//
//	// -------------------------------------------------------
//	// Creating a master voice, with default settings:
//	//
//	//      InputChannels = XAUDIO2_DEFAULT_CHANNELS,
//	//      InputSampleRate = XAUDIO2_DEFAULT_SAMPLERATE,
//	//      Flags = 0,
//	//      szDeviceId = NULL,
//	//      *pEffectChain = NULL,
//	//      StreamCategory = AudioCategory_GameEffects
//	// -------------------------------------------------------
//	hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);
//	assert(hr == S_OK);
//}
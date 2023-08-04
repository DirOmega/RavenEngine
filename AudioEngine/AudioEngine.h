#pragma once

#include "AudioBuffer.h"
#include "Trace.h"

class AudioEngine
{
public:

	HRESULT hr;
	IXAudio2* pXAudio2 = NULL;
	IXAudio2MasteringVoice* pMasterVoice = NULL;
	static AudioEngine* AEInstance;

	~AudioEngine();

	static AudioEngine* getInstance();

	static HRESULT createSourceVoice(IXAudio2SourceVoice*&sv, WAVEFORMATEX& wfx, UINT32  flags, float fRatio, IXAudio2VoiceCallback& callbackContext);


private:

	AudioEngine();
	HRESULT startXAU2();
	HRESULT getMasterVoice();
};

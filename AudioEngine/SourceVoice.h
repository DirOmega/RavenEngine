#pragma once

#include "DLink.h"
#include "VoiceCallBack.h"
#include "XAudio2Wrapper.h"
#include "AudioEngine.h"
#include "TimeEvent.h"
#include "TimerEventManager.h"
#include "AudioBufferManager.h"

enum VoiceTypeID
{
	error, mono16b44100, stereo16b44100, mono32b48000, stereo32b48000
};

//want to switch to protected or something so clients can't see the DLink class
class SourceVoice : public DLink
{
public:
	VoiceTypeID Name;
	VoiceCallback* callbackContext;
	IXAudio2SourceVoice* sv;
	Handle  handle;
	int priority;
	unsigned int playTime;
	unsigned int startTime;
	unsigned int stopTime;
	float panVal = 0;

	unsigned int getPlayTime()
	{
		if (stopTime != 0)
		{
			return playTime;
		}
		else
		{
			return  TimerManager::getCurrentTime() - startTime;
		}
	}

	int getPriority()
	{
		return priority;
	}

	float getPanVal()
	{
		return panVal;
	}

	void setPriority(int priori)
	{
		priority = priori;
	}

	SourceVoice() : handle(), priority(0), playTime(0), startTime(0), stopTime(0)
	{
		sv = nullptr;
		callbackContext = nullptr;
	}

	void setpOut(CircularData* _pOut)
	{
		assert(_pOut);
		this->callbackContext->setpOut(_pOut);
	}

	virtual ~SourceVoice()
	{
		OutputDebugStringW(L"SourceVoice Destroyed!!\n");
	}

	VoiceTypeID getName()
	{
		return Name;
	}

	HRESULT init(WAVEFORMATEX& wfx)
	{
		HRESULT hr;
		callbackContext = new VoiceCallback();
		//AudioEngine::createSourcevoice(&sv,wfx,0,XAUDIO2_MAX_FREQ_RATIO,callbackContext);
		//createSourceVoice(IXAudio2SourceVoice*&sv, tWAVEFORMATEX wfx, UINT32  flags, float fRatio, IXAudio2VoiceCallback& callbackContext);
		if (FAILED(hr = AudioEngine::createSourceVoice(sv, wfx, 0, XAUDIO2_MAX_FREQ_RATIO, *callbackContext)))return hr;
		return 0;
	}

	//bad smell can't really clean anything in a source voice without breaking it.
	//unless the source voice is deleted and completely recreated but that defeats the point
	//of having the source voice pool.
	void dClean()
	{
	}

	//callback controls
	void setStreamEndCMD(Command& cmd)
	{
		callbackContext->commandCenter->setStreamEndCMD(cmd);
	}

	//source voice controls
	//I want to move most of theese out of the source voices control.

	HRESULT SubmitAudio(AudioDataBuffer& buff)
	{
		//TODO: add error handling and reporting
		sv->SubmitSourceBuffer(buff.getXBuff());
		return 0;
	}

	HRESULT SubmitSourceBuffer(XAUDIO2_BUFFER& xbuff)
	{
		//TOOD: add error handling and reporting
		sv->SubmitSourceBuffer(&xbuff);
		return 0;
	}

	HRESULT play()
	{
		HRESULT hr = 0;
		if (FAILED(hr = sv->Start(0)))
			return hr;
		//	OutputDebugStringW(L"SourceVoiceStarted\n");
		startTime = TimerManager::getCurrentTime();
		stopTime = 0u;
		return hr;
	}
	//want to move this out of the source voices control.
	/*HRESULT singlePlay(XAUDIO2_BUFFER buffer)
	{
	HRESULT hr = 0;
	buffer.LoopCount = 0;

	if (FAILED(hr = sv->SubmitSourceBuffer(&buffer)))
	return hr;

	if (FAILED(hr = sv->Start(0)))
	return hr;

	return hr;
	}*/

	HRESULT stop(int tails = XAUDIO2_PLAY_TAILS)
	{
		HRESULT hr = 0;
		stopTime = TimerManager::getCurrentTime();
		playTime = stopTime - startTime;
		if (FAILED(hr = sv->Stop(tails)))
			return hr;
		return hr;
	}

	HRESULT getVolume(float &vol)
	{
		HRESULT hr = 0;
		sv->GetVolume(&vol);
		return hr;
	}

	HRESULT setVolume(float vol)
	{
		HRESULT hr;
		if (FAILED(hr = sv->SetVolume(vol)))
			return hr;

		return hr;
	}

	HRESULT setPitch(float pitchShift)
	{
		HRESULT hr;
		if (pitchShift == 0.0f)
		{
			hr = sv->SetFrequencyRatio(1);
		}
		else if (pitchShift > 0.0f)
		{
			hr = sv->SetFrequencyRatio(pitchShift / 1);
		}
		else
		{
			hr = sv->SetFrequencyRatio(1 / -pitchShift);
		}
		return hr;
	}

	HRESULT setPitchRatio(float numerator, float denominator)
	{
		if (denominator != 0)
		{
			sv->SetFrequencyRatio(numerator / denominator);
		}
	}

	HRESULT pan2Ch(float pan)
	{
		panVal = pan;
		HRESULT hr = 0;
		float Matrix[] = { 0.5f - pan / 2, 0.5f + pan / 2 };
		if (FAILED(hr = sv->SetOutputMatrix(NULL, 1, 2, Matrix)))
			return hr;

		return hr;
	}
};

class TriggerVoiceCMD : public Command
{
public:
	SourceVoice* sv;

	TriggerVoiceCMD(SourceVoice* sv)
	{
		this->sv = sv;
	}

	void execute(unsigned int deltaTime)
	{
		//execute(); 
		deltaTime;
		sv->play();
	}
};

class TriggerVoiceCMDwPan : public Command
{
public:
	SourceVoice* sv;
	float pan;

	TriggerVoiceCMDwPan(SourceVoice* sv, float pan) : sv(sv), pan(pan)
	{
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		sv->pan2Ch(pan);
		sv->play();
	}
};

class PlayPanCMD : public Command
{
public:
	SourceVoice* sv;
	bool started;
	unsigned int remainingTime;//, deltaTime;
	int loopCount;
	float startVal, TargetVal, currentVal, deltaPan, currentPan;

	//std::chrono::seconds panTime;

	PlayPanCMD(SourceVoice* sv, float startPan, float targetVal, unsigned int durationInMs)
	{
		started = false;
		this->sv = sv;
		this->startVal = startPan;
		this->TargetVal = targetVal;
		remainingTime = durationInMs;
		deltaPan = -((startVal - TargetVal) / (durationInMs / 10u));
		//dTime = 10;
		currentPan = startVal;
		loopCount = 0;
		//this->panTime = duration;
	}

	void execute(unsigned int deltaTime)
	{
		if (!started)
		{
			sv->pan2Ch(startVal);
			sv->play();
			TimerManager::Add(TimeEventID::PlayAndPan, *this, 10u);
			started = true;
		}
		else
		{
			remainingTime -= deltaTime;
			currentPan += deltaPan;
			sv->pan2Ch(currentPan);
			loopCount++;
			if (remainingTime > 0) {
				TimerManager::Add(TimeEventID::PlayAndPan, *this, 10u);
			}
			else
			{
				Trace::out("LoopCount from Playpan %i", loopCount);
			}
		}
	}
};

class PlayVolSlideCMD : public Command
{
	SourceVoice* sv;
	bool started;
	unsigned int remainingTime;//, deltaTime;
	int loopCount;
	float startVal, TargetVal, currentVal, deltaVol;

public:

	//std::chrono::seconds panTime;

	PlayVolSlideCMD(SourceVoice* sv, float startVol, float targetVal, unsigned int durationInMs)
	{
		started = false;
		this->sv = sv;
		this->startVal = startVol;
		this->TargetVal = targetVal;
		remainingTime = durationInMs;
		deltaVol = -((startVal - TargetVal) / (durationInMs / 10u));

		loopCount = 0;
	}

	void execute(unsigned int deltaTime)
	{
		if (!started)
		{
			//sv->pan2Ch(startVal);
			sv->setVolume(startVal);
			sv->play();
			TimerManager::Add(TimeEventID::PlayAndVol, *this, 10u);
			started = true;
		}
		else
		{
			remainingTime -= deltaTime;
			float curvol;
			sv->getVolume(curvol);
			sv->setVolume(curvol + deltaVol);
			loopCount++;
			if (remainingTime > 0) {
				TimerManager::Add(TimeEventID::PlayAndVol, *this, 10u);
			}
			else
			{
				Trace::out("LoopCount from PlayVol %i \n", loopCount);
			}
		}
	}
};

class StopVoiceCMD : public Command
{
public:
	SourceVoice* sv;

	StopVoiceCMD(SourceVoice* sv)
	{
		this->sv = sv;
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		sv->stop();
	}
};

class getPlayTimeCMD : public Command
{
	SourceVoice* sv;
public:
	getPlayTimeCMD(SourceVoice* sv)
	{
		this->sv = sv;
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		Trace::out("PlayTime for %p = %i ms\n", sv, sv->getPlayTime());
	}

};

class demo5CMD : public Command
{
	AudioDataBuffer* async;
	SndId toPlay;
	SourceVoice* sv;

public:

	demo5CMD(SourceVoice* sv, SndId toPlay) : sv(sv), toPlay(toPlay), async(nullptr)
	{
	}

	void execute(unsigned int)
	{
		async = AudioBufferManager::Find(toPlay);
		assert(nullptr != async);
		sv->SubmitAudio(*async);
		sv->play();

	}
};

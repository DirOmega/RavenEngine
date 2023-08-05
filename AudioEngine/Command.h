#pragma once
//#include "Common.h"
//#include "WAV.h"
//#include "SourceVoice.h"

#include "XAudio2Wrapper.h"
#include "Trace.h"

class Command
{
public:
	virtual void execute(unsigned int deltaTime = 0) = 0;
	//virtual void execute() = 0;
};

class NullCMD : public Command
{
public:
	static NullCMD* pInstance;

	NullCMD()
	{
	}

	//static NullCMD* getInstance()
	//{
	//	if (pInstance == nullptr)
	//	{
	//		pInstance = new NullCMD();
	//	}

	//	return pInstance;
	//}

	void execute(unsigned int deltaTime = 0)
	{
		deltaTime;
		//OutputDebugStringW(L"NullCMD Executed!");
		//Trace::out("NullCMD Executed!\n");
	}
	//void execute()
	//{
	//	//OutputDebugStringW(L"NullCMD Executed!");
	//	Trace::out("NullCMD Executed!");

	//}

	void dClean()
	{

	}
};

//I want to build these out into lists of things that the command broker can have
//or make them call back to script objects or something so the work can be offloaded
//to another thread
class CommandBroker
{
	Command* OnBufferEndCmdHead;
	Command* OnBufferStartCmdHead;
	Command* OnLoopEndCmdHead;
	Command* OnStreamEndCmdHead;
	Command* OnVoiceErrorCmdHead;
	//Command* OnVoiceProcessingPassEndCmdHead;
	//Command* OnVoiceProcessingPassStartCmdHead;

public:

	HRESULT setBufferEndCMD(Command& cmd)
	{
		OnBufferEndCmdHead = &cmd;
		return 0;
	}

	HRESULT setStreamEndCMD(Command& cmd)
	{
		OnStreamEndCmdHead = &cmd;
		return 0;
	}

	CommandBroker()
	{
		OnBufferEndCmdHead = new NullCMD();
		OnBufferStartCmdHead = new NullCMD();
		OnLoopEndCmdHead = new NullCMD();
		OnStreamEndCmdHead = new NullCMD();
		OnVoiceErrorCmdHead = new NullCMD();
		//OnVoiceProcessingPassEndCmdHead = NullCMD::getInstance();
		//OnVoiceProcessingPassStartCmdHead = NullCMD::getInstance();
	}

	void BufferEndEvent()
	{
		//OutputDebugStringW(L"CommandCenter says BufferEnd\n");
		//Trace::out("CommandCenter says BufferEnd\n");
		OnBufferEndCmdHead->execute();
	}

	void BufferStartEvent()
	{
		//OutputDebugStringW(L"CommandCenter says BufferStart\n");
		//	Trace::out("CommandCenter says BufferStart\n");
		OnBufferStartCmdHead->execute();
	}

	void LoopEndEvent()
	{
		OnLoopEndCmdHead->execute();
		//OutputDebugStringW(L"Command Center says LoopEnd\n");
		//Trace::out("Command Center says LoopEnd\n");
	}

	void StreamEndEvent()
	{
		OnStreamEndCmdHead->execute();
		//OutputDebugStringW(L"Command Center says StreamEnd\n");
		//	Trace::out("Command Center says StreamEnd\n");
	}

	void VoiceErrorEvent()
	{
		OnVoiceErrorCmdHead->execute();
		//OutputDebugStringW(L"Command Center says VoiceError\n");
		//Trace::out("Command Center says VoiceError\n");
	}
};


//class PlayNextWAV : public Command
//{
//
//	WAVQueue* pQ;
//public:
//
//	PlayNextWAV()
//	{
//		pQ = nullptr;
//	}
//
//	void set(WAVQueue* Q)
//	{
//		pQ = Q;
//	}
//
//	void execute(float deltaTime)
//	{
//		if (pQ->QueueStarted == false)
//		{
//			pQ->playList();
//		}
//		else
//		{
//			pQ->playNext();
//		}
//		if (pQ->QueueFinished != true)
//		{
//			TimerManager::Add(TimeEventID::undef, *this, deltaTime);
//		}
//	}

//	void execute()
//	{
//		if (pQ->QueueStarted == false)
//		{
//			pQ->playList();
//		}
//		else
//		{
//			pQ->playNext();
//		}
//		if (pQ->QueueFinished != true)
//		{
//			//			TimerManager::Add(TimeEventID::undef, *this, deltaTime);
//		}
//	}
//
//};
//
//class IncrPanLeft : public Command
//{
//	WAVQueue* pQ;
//	float panval;
//public:
//	
//
//	IncrPanLeft()
//	{
//		pQ = nullptr;
//		panval = 0;
//	}
//
//	void set(WAVQueue* Q)
//	{
//		pQ = Q;
//	}
//
//	void execute(float deltaTime)
//	{
//		panval += 0.01f;
//			pQ->pan2Ch(panval);
//			if (panval < 1.0f) {
////				TimerManager::Add(TimeEventID::undef, *this, 2.0f);
//			}
//	}
//
//	void execute()
//	{
//
//	}
//
//};
//
//class PlayWavOnce : Command
//{
//	baseWAV* pWAV;
//
//public:
//	PlayWavOnce()
//	{
//		pWAV = nullptr;
//	}
//
//	void Attach(baseWAV* pWav)
//	{
//		pWAV = pWav;
//	}
//
//	void execute(float deltaTime)
//	{	
//		pWAV->singlePlay();
//		//TimerManager.Add(TimeEventID.Anim, this, deltaTime);
//	}
//
//	void execute()
//	{
//		pWAV->singlePlay();
//	}
//};

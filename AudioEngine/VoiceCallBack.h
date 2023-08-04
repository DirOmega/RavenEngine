#pragma once

//#include "AudioEngine.h"
#include "XAudio2Wrapper.h"
#include "CircularData.h"

#include <assert.h>
#include <stdio.h>
#include "Command.h"

class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	bool finished = false;
	CommandBroker* commandCenter;
	CircularData *pOut;


	VoiceCallback()
	{
		commandCenter = new CommandBroker();
	}

	void setpOut(CircularData *_pOut)
	{
		pOut = _pOut;
	}
	virtual ~VoiceCallback()
	{
		delete commandCenter;
	}

	virtual void __stdcall OnBufferEnd(void * context)
	{
		//playNext();

		//	OnBufferEndCmd->execute();

		//OutputDebugStringW(L"Manager says BufferEnd\n");
		context;
		//Trace::out("Manager says BufferEnd\n");
		commandCenter->BufferEndEvent();
	}

	virtual void __stdcall OnBufferStart(void * context)
	{
		context;
		//OutputDebugStringW(L"MSays BufferStart\n");
		//Trace::out("Manager says BufferStart\n");
		commandCenter->BufferStartEvent();
	}

	virtual void __stdcall OnLoopEnd(void * context)
	{
		context;
		commandCenter->LoopEndEvent();
		//Trace::out("Manager says LoopEnd\n");
		//OutputDebugStringW(L"LoopEnd\n");
	}

	virtual void __stdcall OnStreamEnd()
	{
		commandCenter->StreamEndEvent();
		//Trace::out("Manager says StreamEnd\n");

		pOut->PushBack(0x1234);
		finished = true;
		//OutputDebugStringW(L"StreamEnd\n");
	}

	virtual void __stdcall OnVoiceError(void * context, HRESULT Error)
	{
		context;
		//Error;
		commandCenter->VoiceErrorEvent();
		Trace::out("Manager says VoiceError %x\n", Error);
		//OutputDebugStringW(L"VoiceError\n");
	}

	//I honestly don't want the users to even have the last two they happen soooo often I don't want to allow a user to muck up the whole system
	//I'll stub out what it would need to bring them back if I have too.
	virtual void __stdcall OnVoiceProcessingPassEnd()
	{
		//commandCenter->ProcessingPassEndEvent();
		//OutputDebugStringW(L"VoiceProcessingPassEnd\n");
	}

	void __stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired)
	{
		BytesRequired;
		//commandCenter->ProcessingPassStartEvent();
		//OutputDebugStringW(L"VoiceProcessingPassStart\n");
	}
};
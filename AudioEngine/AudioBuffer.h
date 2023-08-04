#pragma once
//#include "Common.h"
//#include <xaudio2.h>
//#include "MD5.h"
#include "XAudio2Wrapper.h"
#include "DLink.h"
#include "Snd.h"

class AudioDataBuffer :public DLink//, public VoiceCallback
{
public:

	AudioDataBuffer() : wfx({ 0 }), Xbuffer({ 0 }), Name(SndId::Undef), MD5ReducedHash(0)
	{
		audioData = nullptr;
	}
	~AudioDataBuffer()
	{
		if (audioData != nullptr)
		{
			delete audioData;
		}
	}

	//void set(enum Name, BYTE* buff)
	//{
	//}

	void dClean()
	{
		MD5ReducedHash = 0;
		Name = SndId::Undef;
		wfx = { 0 };
		Xbuffer = { 0 };
		delete audioData;
	}

	void set(SndId name, const BYTE* dataBuff, unsigned int MD5rHash = 0)
	{
		Name = name;
		MD5ReducedHash = MD5rHash;
		audioData = dataBuff;
	}


	void set(WAVEFORMATEX inwfx, XAUDIO2_BUFFER buff)
	{
		this->wfx = inwfx;
		Xbuffer = buff;
	}

	WAVEFORMATEX* getWFX()
	{
		return &wfx;
	}

	const BYTE* getBYTEBuff()
	{
		return audioData;
	}

	XAUDIO2_BUFFER* getXBuff()
	{
		return &Xbuffer;
	}

	unsigned int getHash()
	{
		return MD5ReducedHash;
	}

	void setHash(unsigned int hash)
	{
		MD5ReducedHash = hash;
	}

	void clean()
	{
		if (audioData != nullptr)
		{
			delete audioData;
			audioData = nullptr;
		}
		MD5ReducedHash = 0;
	}

	//void DLink::dClean()
	//{
	//	wfx = { 0 };
	//	buffer = { 0 };
	//	delete audioData;
	//}

	//XAUDIO2_BUFFER CONTROLS
	HRESULT setStreamEnd()
	{
		Xbuffer.Flags = XAUDIO2_END_OF_STREAM;
		return 0;
	}
	HRESULT setStartPoint(UINT32 startSample)
	{
		HRESULT hr = 0;

		Xbuffer.PlayBegin = startSample;

		return hr;
	}

	HRESULT setLength(UINT32 length)
	{
		HRESULT hr = 0;

		if (0 == length)
		{
			setStartPoint(0);
		}
		Xbuffer.PlayLength = 0;

		return hr;
	}

	HRESULT setContext(void * context)
	{
		Xbuffer.pContext = context;
	}

	HRESULT setLoopRegion(UINT32 loopStart, UINT32 loopLength, UINT32 loopCount)
	{
		if (0 == loopCount) { loopStart = 0; loopLength = 0; }
		if (0 != loopCount) {
			if (Xbuffer.PlayBegin + Xbuffer.PlayLength > loopStart) {
				if (loopStart + loopLength > Xbuffer.PlayBegin && loopStart + loopLength < Xbuffer.PlayBegin + Xbuffer.PlayLength)
				{
					Xbuffer.LoopBegin = loopStart;
					Xbuffer.LoopLength = loopLength;
					Xbuffer.LoopCount = loopCount;
					//sv->FlushSourceBuffers();
					//sv->SubmitSourceBuffer(&buffer);
				}
			}
		}
	}

	SndId getName()
	{
		return Name;
	}

	void setName(SndId name)
	{
		Name = name;
	}

	//SourceVoice Controls
	//public for debugging

	//TO ED!!!! I never understood why you put the vairables at the bottom until now,
	//I always do public, protected, private in order from top to bottom which means all the
	//variables get pushed to the end of the code.
private:

	SndId Name;
	unsigned int MD5ReducedHash;
	WAVEFORMATEX  wfx;
	XAUDIO2_BUFFER  Xbuffer;
	const BYTE * audioData;
};

//class AudioDataBuffer : public DLink
//{
//public:
//	AudioDataBuffer()
//	{
//		pActual = new AudioDataBufferPIMPL();
//	}
//	~AudioDataBuffer()
//	{
//		delete pActual;
//	}
//	
//	AudioDataBufferPIMPL* operator ->() 
//	{
//		return pActual;
//	}
//
//	//void set(LPCWSTR fp,enum n)
//	//{
//	//	//pActual->set(fp, n);
//	//}
//
//	void set(BufferID name, BYTE* dataBuff, unsigned int MD5rHash)
//	{
//		pActual->set(name, dataBuff, MD5rHash);
//	}
//
//
//	void set(WAVEFORMATEX wfx, XAUDIO2_BUFFER buff)
//	{
//		pActual->set(wfx, buff);
//	}
//
//	unsigned int getHash()
//	{
//		return pActual->getHash();
//	}
//
//	//toLookup by hash.
//	void setHash(unsigned int hash)
//	{
//		pActual->setHash(hash);
//	}
//
//	void dClean()
//	{
//		pActual->clean();
//	}
//
//private:
//
//	AudioDataBufferPIMPL* pActual;
//	//BYTE * audioData;	
//};
//
//AudioDataBuffer::AudioDataBuffer()
//{
//}
//
//AudioDataBuffer::~AudioDataBuffer()
//{
//}

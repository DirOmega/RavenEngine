#pragma once

#include <assert.h>
#include"baseManager.h"
#include "AudioBuffer.h"
#include "AudioMain.h"
#include "LoadBuffer.h"
#include "SndCallRegistry.h"

class AudioBufferManager : public baseManager
{
private:

	static AudioBufferManager* pInstance;
	static AudioDataBuffer* poRefTE;

	//static Command* pNullCMD;

	AudioBufferManager(int numStart = 5, int deltaAdd = 3)
		: baseManager(deltaAdd)
	{
		generateReserves(numStart);
		poRefTE = new AudioDataBuffer();
	}

	AudioBufferManager()
	{

	}

public:

	static void Create(int numStart = 5, int deltaAdd = 3)
	{
		assert(numStart > 0);
		assert(deltaAdd > 0);
		assert(pInstance == nullptr);

		if (pInstance == nullptr)
		{
			pInstance = new AudioBufferManager(numStart, deltaAdd);
		}
	}

	//static AudioDataBuffer* Add(SndId id)
	//{
	//	//if(

	//}

	//TODO: URGENT!!! modify this to spitout an error code and change the pointer to a data buffer
	//currently if the file isn't found it completes with bad values.
	static AudioDataBuffer* Add(const char* filepath, SndId Name)
	{
		WAVEFORMATEX wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

		AudioBufferManager* pBMan = AudioBufferManager::getInstance();
		assert(pBMan != nullptr);

		helper::LoadBuffer(filepath, wfx, buffer);

		//TimeEvent pNode = (TimeEvent)pTMan.baseAdd();
		AudioDataBuffer* pNode = (AudioDataBuffer*)pBMan->baseAdd();
		assert(pNode != nullptr);

		pNode->set(Name, buffer.pAudioData);
		pNode->set(wfx, buffer);

		return pNode;
	}

	static AudioBufferManager* getInstance()
	{
		if (pInstance == nullptr)
		{
			AudioBufferManager::Create();
		}
		return pInstance;
	}


	static AudioDataBuffer* Find(SndId bufferID)
	{
		AudioBufferManager* mrT = AudioBufferManager::getInstance();
		assert(mrT != nullptr);
		AudioDataBuffer* target = mrT->toFind(bufferID);
		return (AudioDataBuffer*)mrT->baseFind(*target);
	}

	virtual DLink* dCreateNode() override
	{
		DLink* newNode = new AudioDataBuffer();
		assert(newNode != nullptr);

		return newNode;
	}
protected:

	void dClearNode(DLink* pLink)
	{
		AudioDataBuffer* p = (AudioDataBuffer*)pLink;
		p->dClean();
	}

	//void dClearNode(DLink* pLink)
	//{
	//	TimeEvent* p = (TimeEvent*)pLink;
	//	p->dClean();
	//}


	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		//assert(pLinkA != nullptr);
		//assert(pLinkB != nullptr);

		AudioDataBuffer& left = (AudioDataBuffer&)pLinkA;
		AudioDataBuffer& right = (AudioDataBuffer&)pLinkB;

		if (left.getName() == right.getName())
		{
			return true;
		}
		return false;
	}

	//for AltFind
	AudioDataBuffer* toFind(SndId name)
	{
		poRefTE->setName(name);
		return poRefTE;
	}

	//for find
	AudioDataBuffer* toFind(unsigned int hash)
	{
		poRefTE->setHash(hash);
		return poRefTE;
	}
};

//#include "StarterHelperFunctions.h"
//#include "baseManager.h"
//#include "AudioBuffer.h"
//

//
//	//TimeEvent* InSort(TimeEvent& te)
//	//{
//	//	baseManager::baseAddSort(te);
//	//	return &te;
//	//}
//
//


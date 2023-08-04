//#pragma once
//
//#include <assert.h>
//#include "baseManager.h"
//#include "GameCallback.h"
//
//
//class GameCallBackNode
//{
//
//
//
//};
//
//class GameCallbackManager : public baseManager
//{
//private:
//
//	static GameCallbackManager* pInstance;
//	static GameCallBackNode* poRefTE;
//
//	//static Command* pNullCMD;
//
//	GameCallbackManager(int numStart = 5, int deltaAdd = 3)
//		: baseManager(deltaAdd)
//	{
//		generateReserves(numStart);
//		poRefTE = new GameCallBackNode();
//	}
//
//	GameCallbackManager()
//	{
//
//	}
//
//public:
//
//	static void Create(int numStart = 5, int deltaAdd = 3)
//	{
//		assert(numStart > 0);
//		assert(deltaAdd > 0);
//		assert(pInstance == nullptr);
//
//		if (pInstance == nullptr)
//		{
//			pInstance = new GameCallbackManager(numStart, deltaAdd);
//		}
//	}
//
//	static GameCallBackNode* Add(GameCallback* gcb)
//	{
//		GameCallbackManager* pBMan = GameCallbackManager::getInstance();
//		
//		GameCallBackNode* pNode = (GameCallBackNode*)pBMan->baseAdd();
//		assert(pNode != nullptr);
//
//		//pNode->set(Name, buffer.pAudioData);
//		//pNode->set(wfx, buffer);
//
//		return pNode;
//	}
//
//	static AudioBufferManager* getInstance()
//	{
//		if (pInstance == nullptr)
//		{
//			AudioBufferManager::Create();
//		}
//		return pInstance;
//	}
//
//
//	static AudioDataBuffer* Find(SndId bufferID)
//	{
//		AudioBufferManager* mrT = AudioBufferManager::getInstance();
//		assert(mrT != nullptr);
//		AudioDataBuffer* target = mrT->toFind(bufferID);
//		return (AudioDataBuffer*)mrT->baseFind(*target);
//	}
//
//	virtual DLink* dCreateNode() override
//	{
//		DLink* newNode = new AudioDataBuffer();
//		assert(newNode != nullptr);
//
//		return newNode;
//	}
//protected:
//
//	void dClearNode(DLink* pLink)
//	{
//		AudioDataBuffer* p = (AudioDataBuffer*)pLink;
//		p->dClean();
//	}
//
//	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
//	{
//		//assert(pLinkA != nullptr);
//		//assert(pLinkB != nullptr);
//
//		AudioDataBuffer& left = (AudioDataBuffer&)pLinkA;
//		AudioDataBuffer& right = (AudioDataBuffer&)pLinkB;
//
//		if (left.getName() == right.getName())
//		{
//			return true;
//		}
//		return false;
//	}
//
//	//for AltFind
//	AudioDataBuffer* toFind(SndId name)
//	{
//		poRefTE->setName(name);
//		return poRefTE;
//	}
//
//	//for find
//	AudioDataBuffer* toFind(unsigned int hash)
//	{
//		poRefTE->setHash(hash);
//		return poRefTE;
//	}
//};
//

#include <assert.h>

#include "Trace.h"
#include "LoadManager.h"
#include "TimerEventManager.h"


LoadManager *LoadManager::pSndMan = 0;

LoadManager::LoadManager()
{
	this->pG2LBuff = new CircularData();
	assert(pG2LBuff);

	this->pL2GBuff = new CircularData();
	assert(pL2GBuff);
}


CircularData * LoadManager::GetG2LBuff()
{
	LoadManager *pMan = privGetInstance();
	return pMan->pG2LBuff;
}

CircularData * LoadManager::GetL2GBuff()
{
	LoadManager *pMan = privGetInstance();
	return pMan->pL2GBuff;
}

void LoadManager::SendData(int data)
{
	LoadManager *pMan = privGetInstance();

	Trace::out("Game->Audio: %x \n", data);
	pMan->pG2LBuff->PushBack(data);
}

int LoadManager::Update()
{
	LoadManager *pMan = privGetInstance();

	int val = 0;

	if (pMan->pL2GBuff->PopFront(val) == true)
	{
		if (val == 0xD04E)
		{
			//Trace::out("File->Game: %x \n", val);
		}
	}

	return val;
}

void LoadManager::Create()
{
	LoadManager::privCreate();
}

void LoadManager::privCreate()
{
	// Should only be created once
	assert(pSndMan == 0);

	pSndMan = new LoadManager();
	assert(pSndMan);
}

LoadManager *LoadManager::privGetInstance()
{
	assert(pSndMan);
	return pSndMan;
}
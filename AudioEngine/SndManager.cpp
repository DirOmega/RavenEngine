#include <assert.h>

#include "Trace.h"
#include "SndManager.h"

SndManager *SndManager::pSndMan = 0;

SndManager::SndManager()
{
	this->pG2ABuff = new CircularData();
	assert(pG2ABuff);

	this->pA2GBuff = new CircularData();
	assert(pA2GBuff);
}


CircularData * SndManager::GetG2ABuff()
{
	SndManager *pMan = privGetInstance();
	return pMan->pG2ABuff;
}

CircularData * SndManager::GetA2GBuff()
{
	SndManager *pMan = privGetInstance();
	return pMan->pA2GBuff;
}

void SndManager::SendData(int data)
{
	SndManager *pMan = privGetInstance();

	Trace::out("Game->Audio: %x \n", data);
	pMan->pG2ABuff->PushBack(data);
}

void SndManager::Update()
{
	SndManager *pMan = privGetInstance();

	int val;

	if (pMan->pA2GBuff->PopFront(val) == true)
	{
		if (val == 0x1234)
		{
			Trace::out("Audio->Game: %x \n", val);
		}
	}
}

void SndManager::Create()
{
	SndManager::privCreate();
}

void SndManager::privCreate()
{
	// Should only be created once
	assert(pSndMan == 0);

	pSndMan = new SndManager();
	assert(pSndMan);
}

SndManager *SndManager::privGetInstance()
{
	assert(pSndMan);
	return pSndMan;
}
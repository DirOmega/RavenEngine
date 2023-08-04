#pragma once

#include "baseManager.h"

#include"DLink.h"
#include "Trace.h"
#include <assert.h>

	int baseManager::getActive()
	{
		return mNumActive;
	}

	int baseManager::getReserve()
	{
		return mNumReserved;
	}

	int baseManager::getTotal()
	{
		return mNumActive + mNumReserved;
	}


	bool baseManager::Remove(DLink& toRemove)
	{
		if (&toRemove != nullptr)
		{
			//assert(&toRemove != nullptr);

			removeFromActive(toRemove);
			toRemove.dClean();
			recycleToReserve(toRemove);
		}
		return true;
	}

	int baseManager::debugCountActive()
	{
		int numActive = 0;
		DLink* cur = pActiveHead;
		while (nullptr != cur)
		{
			numActive++;
			cur = cur->pNext;
		}
		return numActive;
	}

	int baseManager::debugCountReserve()
	{
		int numReserve = 0;
		DLink* cur = pReserveHead;
		while (nullptr != cur)
		{
			numReserve++;
			cur = cur->pNext;
		}
		return numReserve;
	}

	int baseManager::debugCountAll()
	{
		return debugCountActive() + debugCountReserve();
	}

	DLink* baseManager::getActiveHead()
	{
		return pActiveHead;
	}

	DLink* baseManager::getReserveHead()
	{
		return pReserveHead;
	}

	DLink* baseManager::baseAdd()
	{
		return addToActive(*getFromReserve());
	}

	DLink* baseManager::baseAddSort(DLink& toAdd)
	{
		//DLink.addToFront(ref pActiveHead, toAdd);
		DLink::baseSortAdd(pActiveHead, &toAdd);
		return &toAdd;
	}

	//baseManager(int numStart = 5, int deltaAdd = 3)
	baseManager::baseManager(int deltaAdd)
	{

		mNumActive = 0;

		mNumReserved = 0;

		mDeltaAdd = deltaAdd;
		pReserveHead = nullptr;
		pActiveHead = nullptr;
	}

	baseManager::~baseManager()
	{
		//OutputDebugStringW(L"A manager has been deleted!");
		DLink* toDelete;
		while ((toDelete = getActiveHead()) != nullptr)
		{
			DLink::RemoveNode(pActiveHead, *toDelete);
			delete toDelete;
		}
		while ((toDelete = getReserveHead()) != nullptr)
		{
			DLink::RemoveNode(pReserveHead, *toDelete);
			delete toDelete;
		}
		Trace::out("A manager has been deleted!\n");
	}

	DLink* baseManager::baseFind(DLink& pTarget)
	{
		DLink *cur = pActiveHead;
		while (cur != nullptr)
		{
			if (dCompareNodes(*cur, pTarget))
			{
				break;
			}
			cur = cur->pNext;
		}
		return cur;
	}


	void baseManager::generateReserves(int numToAdd)
	{
		assert(numToAdd > 0);
		for (int i = 0; i<numToAdd; i++)
		{
			generateReserve();
		}
	}

	void baseManager::generateReserve()
	{
		mNumReserved++;
		DLink* newLink;
		newLink = dCreateNode();
		assert(&newLink != nullptr);

		DLink::addToFront(pReserveHead, newLink);

	}


	DLink* baseManager::getFromReserve()
	{

		mNumReserved--;
		//DLink ret = pReserveHead;
		//pReserveHead = pReserveHead.pNext;
		if (nullptr == pReserveHead)
		{
			generateReserves(3);
		}
		//pReserveHead.pPrev = null;
		return DLink::PullFromFront(pReserveHead);
	}

	//TODO: update this. 
	DLink* baseManager::addToActive(DLink& newLink)
	{

		mNumActive++;
		DLink::addToFront(pActiveHead, &newLink);

		return &newLink;
	}

	void baseManager::removeFromActive(DLink& toRemove)
	{
		DLink::RemoveNode(pActiveHead, toRemove);
		mNumActive--;
	}

	void baseManager::recycleToReserve(DLink& toRemove)
	{

		DLink::addToFront(pReserveHead, &toRemove);
		mNumReserved++;
	}
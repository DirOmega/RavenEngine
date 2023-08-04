#pragma once
//#include "Common.h"

#include "DLink.h"
#include "Trace.h"
#include <assert.h>

class baseManager //: iManager
{
public:

	DLink *pReserveHead;
	DLink *pActiveHead;
	int mnumStart;
	int mDeltaAdd;
	int mNumActive;
	int mNumReserved;


	int getActive()
	{
		return mNumActive;
	}

	int getReserve()
	{
		return mNumReserved;
	}

	int getTotal()
	{
		return mNumActive + mNumReserved;
	}


	bool Remove(DLink& toRemove)
	{
		assert(&toRemove != nullptr);


		removeFromActive(toRemove);
		recycleToReserve(toRemove);


		return true;
	}

	int debugCountActive()
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

	int debugCountReserve()
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

	int debugCountAll()
	{
		return debugCountActive() + debugCountReserve();
	}

protected:

	virtual DLink* dCreateNode() = 0;
	/*{
	OutputDebugStringW(L"A pure virtual function has been called!");
	return nullptr;
	}*/

	virtual bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) = 0;

	virtual void dClearNode(DLink* pLink) = 0;


	DLink* getActiveHead()
	{
		return pActiveHead;
	}

	DLink* baseAdd()
	{
		return addToActive(*getFromReserve());
	}

	DLink* baseAddSort(DLink& toAdd)
	{
		//DLink.addToFront(ref pActiveHead, toAdd);
		DLink::baseSortAdd(pActiveHead, &toAdd);
		return &toAdd;
	}

	//baseManager(int numStart = 5, int deltaAdd = 3)
	baseManager(int deltaAdd = 3)
	{
		//numStart;
		mNumActive = 0;
		mNumReserved = 0;

		mDeltaAdd = deltaAdd;
		pReserveHead = nullptr;
		pActiveHead = nullptr;
		//	generateReserves(numStart);
	}


	virtual ~baseManager()
	{
		//OutputDebugStringW(L"A manager has been deleted!");
		Trace::out("A manager has been deleted!");
	}



	DLink* baseFind(DLink& pTarget)
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


private:

	//private copyconstructor
	baseManager(baseManager &x)
	{
		x;
	}

	baseManager(baseManager && x) = default;

	baseManager& operator=(const baseManager& x) & = default;

	baseManager& operator=(baseManager&& x) & = default;

protected:
	void generateReserves(int numToAdd)
	{
		assert(numToAdd > 0);
		for (int i = 0; i<numToAdd; i++)
		{
			generateReserve();
		}
	}

	void generateReserve()
	{
		mNumReserved++;
		DLink* newLink;
		newLink = dCreateNode();
		assert(&newLink != nullptr);

		DLink::addToFront(pReserveHead, newLink);

	}


	DLink* getFromReserve()
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
	DLink* addToActive(DLink& newLink)
	{

		mNumActive++;
		DLink::addToFront(pActiveHead, &newLink);

		return &newLink;
	}

	void removeFromActive(DLink& toRemove)
	{
		DLink::RemoveNode(pActiveHead, toRemove);
		mNumActive--;
	}

	void recycleToReserve(DLink& toRemove)
	{

		DLink::addToFront(pReserveHead, &toRemove);
		mNumReserved++;
	}

};

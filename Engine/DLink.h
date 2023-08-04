#pragma once

//#include "Common.h"
#include <assert.h>

class DLink
{
public:

	DLink *pNext;
	DLink *pPrev;

	DLink();

	virtual ~DLink();

	virtual void dClean() = 0;

	static DLink* addToFront(DLink*& pHead, DLink* pNode);

	static DLink* PullFromFront(DLink*& pHead);

	virtual bool greaterThan(DLink*);

	static DLink* baseSortAdd(DLink*& pActiveHead, DLink* toAdd);

	static void RemoveNode(DLink*&pHead, DLink& pNode);


protected:

	void Clear();
};
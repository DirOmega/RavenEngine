#pragma once

#include "MemDLink.h"
#include "HeapLink.h"

//TODO:: change file name
class MemGDLink : public MemHeapLink
{

public:

	MemGDLink* pGNext;
	MemGDLink* pGPrev;

	MemGDLink()
	{
		pGNext = nullptr;
		pGPrev = nullptr;
	}

	void Clear()
	{
		pGNext = nullptr;
		pGPrev = nullptr;
	}


	static void addToFront(MemGDLink*& pHead, MemGDLink* pNode)
	{

		assert(pNode != nullptr);

		pNode->pGPrev = nullptr;
		if (pHead != nullptr)
		{
			pHead->pGPrev = pNode;
		}

		pNode->pGNext = pHead;
		pHead = pNode;

		assert(pHead != nullptr);
	}

	static void RemoveNode(MemGDLink*& pHead, MemGDLink* pNode)
	{
		// protection
		assert(pNode != nullptr);

		// 4 different conditions... 
		if (pNode->pGPrev != nullptr)
		{	// middle or last node
			pNode->pGPrev->pGNext = pNode->pGNext;
		}
		else
		{  // first
			pHead = pNode->pGNext;
		}

		if (pNode->pGNext != nullptr)
		{	// middle node
			pNode->pGNext->pGPrev = pNode->pGPrev;
		}
	}
};

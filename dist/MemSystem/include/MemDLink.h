#pragma once

#include <assert.h>

class MemDLink 
{

public:

	MemDLink* pNext;
	MemDLink* pPrev;

	MemDLink()
	{
		pNext = nullptr;
		pPrev = nullptr;
	}
	
	void Clear()
	{
		pNext = nullptr;
		pPrev = nullptr;
	}

	static void addToFront(MemDLink*& pHead, MemDLink* pNode)
	{

		assert(pNode != nullptr);

		pNode->pPrev = nullptr;
		if (pHead != nullptr)
		{
			pHead->pPrev = pNode;
		}

		pNode->pNext = pHead;
		pHead = pNode;

		assert(pHead != nullptr);
	}

	static void RemoveNode( MemDLink*& pHead, MemDLink* pNode)
	{
		// protection
		assert(pNode != nullptr);

		// 4 different conditions... 
		if (pNode->pPrev != nullptr)
		{	// middle or last node
			pNode->pPrev->pNext = pNode->pNext;
		}
		else
		{  // first
			pHead = pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{	// middle node
			pNode->pNext->pPrev = pNode->pPrev;
		}
	}
};



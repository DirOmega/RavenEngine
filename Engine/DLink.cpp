#pragma once

#include <assert.h>
#include "DLink.h"

DLink::DLink()
{
	//Clear();
	pNext = nullptr;
	pPrev = nullptr;
}

DLink::~DLink()
{
}


DLink* DLink::addToFront(DLink*& pHead, DLink* pNode)
{

	//Debug.Assert(pNode != null);
	assert(&pNode != nullptr);

	if (pHead != nullptr)
	{
		pHead->pPrev = pNode;

	}

	pNode->pNext = pHead;
	pHead = pNode;
	assert(pHead != nullptr);

	return pHead;
}

DLink* DLink::PullFromFront(DLink*& pHead)
{
	// There should always be something on list
	assert(&pHead != nullptr);

	// return node
	DLink* pNode = pHead;

	// Update head (OK if it points to NULL)
	pHead = pHead->pNext;
	if (pHead != nullptr)
	{
		pHead->pPrev = nullptr;
	}

	// remove any lingering links
	pNode->Clear();

	return pNode;
}

bool DLink::greaterThan(DLink*)
{
	//	//if this is greater than that return true;
	//	//woah woah you have to implement this before use.
	assert(false);
	return false;
}

DLink* DLink::baseSortAdd(DLink*& pActiveHead, DLink* toAdd)
{

	//add to front OR add only
	//just hoping for that early out
	if (nullptr == pActiveHead || pActiveHead->greaterThan(toAdd))
	{
		addToFront(pActiveHead, toAdd);
	}
	else
	{
		DLink* cur = pActiveHead;
		while (nullptr != cur)
		{
			//add end or middle
			if (cur->pNext == nullptr || cur->pNext->greaterThan(toAdd))
			{
				if (nullptr != cur->pNext)
				{
					cur->pNext->pPrev = toAdd;
				}
				toAdd->pNext = cur->pNext;
				cur->pNext = toAdd;
				toAdd->pPrev = cur;
				break;
			}
			cur = cur->pNext;
		}
	}
	return toAdd;
}

void DLink::RemoveNode(DLink*&pHead, DLink& pNode)
{
	// protection
	assert(&pNode != nullptr);

	// 4 different conditions... 
	if (pNode.pPrev != nullptr)
	{	// middle or last node
		pNode.pPrev->pNext = pNode.pNext;
	}
	else
	{  // first
		pHead = pNode.pNext;
	}

	if (pNode.pNext != nullptr)
	{	// middle node
		pNode.pNext->pPrev = pNode.pPrev;
	}
}



void DLink::Clear()
{
	pNext = nullptr;
	pPrev = nullptr;
	dClean();
}

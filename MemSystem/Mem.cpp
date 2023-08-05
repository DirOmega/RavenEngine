//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#pragma warning (push)
#pragma warning (disable : 4820)
#include <new>
#pragma warning (pop)

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Mem.h"
#include "Heap.h"
#include "MemGlobalLink.h"
#include <heapapi.h>

#define STUB_PLEASE_REPLACE(x) (x)

// Create the heap.
Mem::Code Mem::HeapCreate(Heap *&newHeap, unsigned int heapSize, const char * const Name)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;

	if (Instance->mInitialized)
	{
		//TODO: do the creation 
		HANDLE h =  _HEAPAPI_H_::HeapCreate(HEAP_NO_SERIALIZE, heapSize, heapSize);
		
		if (h != 0)
		{
			newHeap = new (HeapAlloc(h, 0, sizeof(Heap))) Heap(heapSize,Name,Instance,h);
			Instance->HeapAddToFront(newHeap);
			retCode = Code::OK;
		}
		else
		{
			retCode = Code::ERROR_Heap_Create;
		}
	}

	return retCode;
}

Mem::Code Mem::HeapCreate(Heap *&newHeap, unsigned int numBlocks, int sizePerBlock, const char * const Name)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;

	if (Instance->mInitialized)
	{
		HANDLE h = _HEAPAPI_H_::HeapCreate(HEAP_NO_SERIALIZE, numBlocks * sizePerBlock, numBlocks * sizePerBlock);
		//_HEAPAPI_H_::FillMemory(h, numBlocks*sizePerBlock,0xaa);
		if (h != 0)
		{
			newHeap = new (HeapAlloc(h, 0, sizeof(Heap))) Heap(numBlocks, sizePerBlock, Name, Instance, h);
			Instance->HeapAddToFront(newHeap);
			retCode = Code::OK;
		}
		else
		{
			retCode = Code::ERROR_Heap_Create;
		}
	}
	return retCode;
}

// Initialize the memory system  (Public method)
Mem::Code Mem::Initialize(void)
{
	Code retCode = Code::Dword;

	if (Instance->mInitialized == false)
	{
		Instance->mInitialized = true;
		retCode = Code::OK;
	}
	else
	{ //this is just to clean between tests if the test fails it still destroys it
		Destroy();
		Initialize();
	}

	return retCode;
}

Mem::Code Mem::Destroy(void)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;

	if (Instance->mInitialized)
	{
		//TODO:: properly destroy	
		
		while (Instance->poHead != nullptr)
		{
			HeapDestroy(Instance->poHead);
		}

		assert(Instance->pGlobalHead == 0); //If I just destroyed all the heaps the Global Tracking list will be empty
		
		Instance->mInitialized = false;
		Instance->mInfo = { 0 };
		retCode = Code::OK;
	}

	return retCode;
}

Mem::Code Mem::GetInfo(Mem::Info &retInfo)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;

	if (Instance->mInitialized)
	{
		retInfo = Instance->mInfo;
		retCode = Code::OK;
	}

	return retCode;
}


Heap *Mem::DebugGetHeapHead()
{
	return Instance->poHead;
}


Block *Mem::DebugGetGlobalTrackingHead()
{
	return Instance->pGlobalHead;
}

Mem::Code Mem::HeapDestroy(Heap *inHeap)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;
	
	if (Instance->mInitialized)
	{
		Instance->HeapRemove(inHeap);
		retCode = Code::OK;
	}

	return retCode;
}


Mem::Code Mem::GetHeapByAddr(Heap *&pHeap, void *p)
{
	Code retCode = Code::ERROR_Mem_Not_Initialized;
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(p);

	if (Instance->mInitialized)
	{
		retCode = Code::OK;
	}

	return retCode;
}


Mem::Code Mem::HeapAddToFront(Heap* h)
{
	//This is a calculated risk, it's private and only called after init has been checked.
	Instance->mInfo.AddHeap();
	MemDLink::addToFront((MemDLink*&)Instance->poHead, h);

	return Code::OK;
}

Mem::Code Mem::HeapRemove(Heap* h)
{
	Instance->mInfo.RemoveHeap();
	MemDLink::RemoveNode((MemDLink*&)Instance->poHead, h);
	
	HANDLE handle = h->getHandle();
	h->~Heap();
	_HEAPAPI_H_::HeapDestroy(handle);

	return Code::OK;
}


Mem::Mem() : poHead(0), pGlobalHead(0), mInfo({ 0 }), mInitialized(false), pad0(0), pad1(0), pad2(0)
{ }

Mem::~Mem()
{
	while (poHead != nullptr)
	{
		HeapDestroy(poHead);
	}
}

unsigned int Mem::AlignEnumToPadVal(Mem::Align code)
{
	unsigned int pad = 0;
	//this should be compiled as a jump table so it shouldn't need optimization
	switch (code)
	{
	case Mem::Byte_4:
		pad = 4;
		break;
	case Mem::Byte_8:
		pad = 8;
		break;
	case Mem::Byte_16:
		pad = 16;
		break;
	case Mem::Byte_32:
		pad = 32;
		break;
	case Mem::Byte_64:
		pad = 64;
		break;
	case Mem::Byte_128:
		pad = 128;
		break;
	case Mem::Byte_256:
		pad = 256;
		break;
	default:
		assert(false); //if default crash
	}
	return pad;
}

void* Mem::Alloc(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum)
{
	void* retPtr = nullptr;
	unsigned int pad = AlignEnumToPadVal(align);
	if (pHeap != nullptr)
	{
		Instance->mInfo.AddAlloc();
		Instance->mInfo.AddBytes(inSize);
		retPtr = pHeap->privAlloc(inSize, pad, inName, lineNum,Instance->mInfo.getCurAllocIndex());

#if _DEBUG
		Heap::Info curInfo;

		pHeap->GetInfo(curInfo);
		unsigned int testAddr = (unsigned int) retPtr;
		assert(testAddr >  curInfo.StartAddr && testAddr < curInfo.EndAddr);
		assert(testAddr + inSize > curInfo.StartAddr && testAddr + inSize < curInfo.EndAddr);

#endif

	}
	return retPtr;
}

//TODO: whatever I need to do to differentiate between array allocations, it currently is identical.
//void* Mem::ArrayAlloc(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum)
//{
//	void* retPtr = nullptr;
//	unsigned int pad = AlignEnumToPadVal(align);
//	if (pHeap != nullptr)
//	{
//		Instance->mInfo.AddAlloc();
//		Instance->mInfo.AddBytes(inSize);
//		retPtr = pHeap->privAlloc(inSize, pad, inName, lineNum, Instance->mInfo.getCurAllocIndex());
//	}
//	return retPtr;
//
//}

bool Mem::FreeFromMyMem(void* p)
{
	bool retVal = false;

	if (p != nullptr)
	{
		if (Instance->poHead != nullptr)
		{
			Heap* cur = Instance->poHead;
			Heap::Info curInfo;
			while (cur != nullptr)
			{
				cur->GetInfo(curInfo);
				if (curInfo.StartAddr < (unsigned int)p && curInfo.EndAddr >= (unsigned int)p)
				{
					if (p == (void*)curInfo.StartAddr)
					{
						assert(false);//probably a better way to check for this, probably if p = cur; I don't want to be allowed to destroy a heap this way.
					}

					Instance->mInfo.removeAlloc();
					Instance->mInfo.RemoveBytes(cur->privfreeAlloc(p));
					retVal = true; //sets the return val to true so the global delete knows it's been found in this memory management system.
					break;
				}
				else
				{
					cur = (Heap*)cur->pNext;
				}
			}
		}
	}
	return retVal;
}

bool Mem::FreeFromMyMemA(void* p)
{
	bool retVal = false;

	if (p != nullptr)
	{
		if (Instance->poHead != nullptr)
		{
			Heap* cur = Instance->poHead;
			Heap::Info curInfo;
			while (cur != nullptr)
			{
				cur->GetInfo(curInfo);
				if (curInfo.StartAddr < (unsigned int)p && curInfo.EndAddr >= (unsigned int)p)
				{
					if (p == (void*)curInfo.StartAddr)
					{
						assert(false);//probably a better way to check for this, probably if p = cur; I don't want to be allowed to destroy a heap this way.
					}
					
					Instance->mInfo.removeAlloc();
					Instance->mInfo.RemoveBytes(cur->privfreeAllocA(p));
					retVal = true; //sets the return val to true so the global delete knows it's been found in this memory management system.
					break;
				}
				else
				{
					cur = (Heap*) cur->pNext;
				}
			}
		}
	}
	return retVal;
}

void Mem::privRemoveAlloc()
{
	mInfo.removeAlloc();
}

void Mem::privRemoveBytes(unsigned int bytes)
{
	mInfo.RemoveBytes(bytes);
}

void Mem::AddBlockForTracking(Block* toAdd)
{
	MemGDLink::addToFront((MemGDLink*&)pGlobalHead,(MemGDLink*) toAdd);
}

void Mem::RemoveBlockForTracking(Block* toRemove)
{
	MemGDLink::RemoveNode((MemGDLink*&)pGlobalHead, (MemGDLink*)toRemove);
}

Mem* Mem::Instance = new Mem();
// ---  End of File ---------------

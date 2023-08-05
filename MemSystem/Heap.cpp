#pragma warning (push)
#pragma warning (disable : 4820)
#include <assert.h>
#include <new.h>  // for placement new
#pragma warning (pop)

#include "Heap.h"
#include "Block.h"

#define STUB_PLEASE_REPLACE(x) (x)

void Heap::GetInfo( Info &retInfo)
{
	retInfo = mInfo;
};

Heap *Heap::DebugGetNext() const
{
	return 	(Heap*) pNext;
};

Heap *Heap::DebugGetPrev() const
{
	return (Heap*) pPrev;
};

Block *Heap::GetTrackingHead()
{
   return pBlkHead;
}

void Heap::nameCpy(const char* const pInName)
{
	for (int i = 0; i < Heap::NameLength; i++)
	{
		mInfo.Name[i] = pInName[i];
		if (i == Heap::NameLength - 1) { mInfo.Name[i] = '\0'; }
	}
}

Heap::Heap(unsigned int heapSize, const char * const Name, Mem * const _pMem, HANDLE heapHandle) : mInfo({ 0 }), pMem(_pMem)
{

	nameCpy(Name);
	mInfo.StartAddr = (unsigned int)this;
	mInfo.EndAddr = heapSize + (unsigned int)this;
	mInfo.TotalSize = heapSize;
	mWinHeapHandle = heapHandle;

}

Heap::Heap(unsigned int numBlocks, int sizePerBlock, const char* const Name, Mem * const _pMem, HANDLE heapHandle) : mInfo({ 0 }), pMem(_pMem)
{

	nameCpy(Name);
	mInfo.StartAddr = (unsigned int)this;
	mInfo.TotalSize = numBlocks * sizePerBlock;
	mInfo.EndAddr = mInfo.TotalSize + (unsigned int)this;
	mWinHeapHandle = heapHandle;
}

HANDLE Heap::getHandle()
{
	return mWinHeapHandle;
}

void* Heap::privAlloc(size_t inSize, unsigned int align, char *inName, int lineNum,unsigned int allocIndex)
{
	//TODO: Optimize this

	//create the allocation and put the block header at it's start address
	unsigned char * rawptr = (unsigned char*) HeapAlloc(mWinHeapHandle, 0, inSize + sizeof(Block) + align + sizeof(void*));
	Block* allocBlock = new(rawptr) Block(this, inName, lineNum, allocIndex, inSize);

	//book keeping
	this->mInfo.AddAlloc(inSize);
	MemHeapLink::addToFront((MemDLink*&)pBlkHead, allocBlock);
	pMem->AddBlockForTracking(allocBlock);
	//book keeping

	//increment the pointer from the block header and leave room for the secret ptr
	rawptr += sizeof(Block) + sizeof(void*); //sizeof void leaves a space for the secret pointer even if corrected pointer falls on it's alignment (I need to confrim this it seems to leave 4 bytes of padding)
	//correct the pointer for alignment
	unsigned char *p_corrected = (unsigned char *)(((unsigned int)rawptr + (align - 1)) & ~(align - 1));

	//jump back one ptr and hide the secret pointer
	unsigned int * secretptr = (unsigned int*) p_corrected - 1;
	*secretptr = (unsigned int)allocBlock;

	//finally return the pointer corrected for alignment
	return p_corrected;
}

unsigned int Heap::privfreeAlloc(void* p)
{
	unsigned int retVal = 0;
	
	if (p)
	{
		unsigned int* scrtPtr = (unsigned int*)p;
		scrtPtr--;
		
		Block* trackingblock = (Block*)*scrtPtr;
		
		this->RemoveNode((MemDLink*&) pBlkHead, (Block*) trackingblock);
		pMem->RemoveBlockForTracking(trackingblock);
		retVal = trackingblock->GetAllocSize();

		this->mInfo.removeAlloc(retVal);

		HeapFree(mWinHeapHandle, 0, trackingblock);
	}

	return retVal;
}

unsigned int Heap::privfreeAllocA(void* p)
{
	unsigned int retVal = 0;

	if (p)
	{
		unsigned int* scrtPtr = (unsigned int*)p;
		scrtPtr--;
		//scrtPtr--; //have to double back in order to jump over the size of the array

		Block* trackingblock = (Block*)*scrtPtr;

		this->RemoveNode((MemDLink*&)pBlkHead, (Block*)trackingblock);
		pMem->RemoveBlockForTracking(trackingblock);
		retVal = trackingblock->GetAllocSize();

		this->mInfo.removeAlloc(retVal);

		HeapFree(mWinHeapHandle, 0, trackingblock);
	}

	return retVal;
}

Heap::~Heap()
{
	Block* cur;
	while (pBlkHead != nullptr)
	{
		//need to create helper function for removal
		cur = pBlkHead;
		this->RemoveNode((MemDLink*&)pBlkHead,(MemDLink*)pBlkHead);
		pMem->RemoveBlockForTracking(cur);
		pMem->privRemoveAlloc();
		pMem->privRemoveBytes(cur->GetAllocSize());
		_HEAPAPI_H_::HeapFree(mWinHeapHandle,0, cur);
	}
}

// ---  End of File ---------------

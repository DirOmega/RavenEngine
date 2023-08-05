#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Block.h"

#define STUB_PLEASE_REPLACE(x) (x)

Block *Block::GetHeapNext()
{
	return (Block*)MemHeapLink::pNext;
}

Block *Block::GetHeapPrev()
{
	return (Block*)MemHeapLink::pPrev;
}

Block *Block::GetGlobalNext()
{
	return (Block*)MemGDLink::pGNext;
}

Block *Block::GetGlobalPrev()
{
	return (Block*)MemGDLink::pGPrev;
}

char *Block::GetFileName()
{
	return mFileName;
}

int Block::GetLineNum()
{
	return mLineNum;
}

unsigned int Block::GetAllocIndex()
{
	return mAllocIndex;
}

unsigned int Block::GetAllocSize()
{
	return mAllocSize;
}

Block::Block(Heap* pHeap, char * _fileName,/* bool _mArrayAllocation,*/ int _mLineNum, unsigned int _mAllocIndex, unsigned int _ASize)
	: pContainingHeap(pHeap), /*mArrayAllocation(_mArrayAllocation), pad0('ª'), pad1('ª'), pad2('ª'),*/ mLineNum(_mLineNum), mAllocIndex(_mAllocIndex), mAllocSize(_ASize)
{
	if (_fileName != nullptr)
	{
		char* fixedFileName = strrchr(_fileName, '\\');
		if (fixedFileName == nullptr)
		{
			fixedFileName = _fileName; //for if full fP isn't being captured becuase that can apparently happen based on compiler flags
		}
		else
		{
			fixedFileName += 1;
		}
		for (int i = 0; i < Heap::NameLength; i++)
		{
			mFileName[i] = fixedFileName[i];
			if (i == Heap::NameLength - 1) { mFileName[i] = '\0'; }
		}
	}
}

Block::~Block()
{
	//No dynamic storage to free
}

// ---  End of File ---------------

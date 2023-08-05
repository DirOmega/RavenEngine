#ifndef BLOCK_H
#define BLOCK_H

#include "Heap.h"
#include "MemGlobalLink.h"

class Block : public MemGDLink //public HeapLink //,public GlobalLink
{
public:
	// defines for the heap name length
	static const unsigned int NameLength = 32;
	static const unsigned int NameNumChar = (Heap::NameLength - 1);

public:
	// Public methods
	Block *GetHeapNext();
	Block *GetHeapPrev();
	Block *GetGlobalNext();
	Block *GetGlobalPrev();
	char *GetFileName();
	int GetLineNum();
	unsigned int GetAllocIndex();
	unsigned int GetAllocSize();

	Block(Heap* pHeap, char * _fileName, /*bool _mArrayAllocation,*/ int _mLineNum, unsigned int _mAllocIndex, unsigned int _ASize);
	~Block();
	
private:

	// Total Size of this class:
	// 64 Bytes - add appropriate padding

	// Note: need to add Tracking Block links for Heap and Global lists
	//       Several ways to do it... I don't care
	//       No Templates

	// Total Size of this class:
	// 64 Bytes - add appropriate padding

   // Required
	//DLink GlobalLink;
	//DLink HeapLink;
	Heap*		   pContainingHeap;
	char           mFileName[Block::NameLength]; 
	//bool		   mArrayAllocation;
	//char		   pad0, pad1, pad2;//for future use
	int            mLineNum;
	unsigned int   mAllocIndex;
	unsigned int   mAllocSize;

	//todo: add a ptr to the heap that this exists in.
	//add padding soon

	//Block          *pSecret;   // back pointer to the top of the trackingblock
};

#endif

// ---  End of File ---------------

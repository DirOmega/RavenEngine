#ifndef HEAP_H
#define HEAP_H

#include "Mem.h"
#include "MemDLink.h"

#pragma warning( push )
#pragma warning( disable : 4820 )
#pragma warning( disable : 4668 )
#include <Windows.h>
#pragma warning( pop ) 

// Forward declaration
class Block;


// Heap class
class Heap : public MemDLink
{
public:
	// Constants 
	static const unsigned int NameLength = 32;
	static const unsigned int NameNumChar = (NameLength - 1);

	// Heap Types
	enum Type
	{
		Normal,
		Fixed,
		Dword = 0x7fffffff
	};

	// Info structure
	struct Info
	{
		char Name[Heap::NameLength];
		unsigned int StartAddr;
		unsigned int EndAddr;
		unsigned int TotalSize;
		int PeakNumAlloc;
		int CurrNumAlloc;
		int PeakBytesUsed;
		int CurrBytesUsed;

		void AddAlloc(unsigned int bytes)
		{
			this->CurrBytesUsed += bytes;

			if (this->CurrBytesUsed > this->PeakBytesUsed)
			{
				this->PeakBytesUsed = this->CurrBytesUsed;
			}

			this->CurrNumAlloc++;

			if (this->CurrNumAlloc > this->PeakNumAlloc)
			{
				this->PeakNumAlloc = this->CurrNumAlloc;
			}

		}

		void removeAlloc(unsigned int bytes)
		{
			this->CurrBytesUsed -= bytes;
			this->CurrNumAlloc--;
		}
	};

public:
	// Public Interface (For Customers)
	void GetInfo(Info &info);

	// Tracking block 
	Block *GetTrackingHead();

	// Needed for Testings (Not for Customers)
	Heap *DebugGetNext() const;
	Heap *DebugGetPrev() const;

	//Heap();
	Heap(unsigned int heapSize, const char * const Name, Mem * const _pMem, HANDLE heapHandle);
	Heap(unsigned int numBlocks, int sizePerBlock, const char* const Name, Mem * const _pMem, HANDLE heapHandle);

	~Heap();

	HANDLE getHandle();
	//make theese private later
    unsigned int privfreeAlloc(void* p);
	unsigned int privfreeAllocA(void* p);
	void* privAlloc(size_t inSize, unsigned int align, char *inName, int lineNum,unsigned int allocIndex);


private:
	// data -----------------------------------------------------------------------

	void nameCpy(const char* const pInName);

	Block	*pBlkHead;
	Type	mType;
	HANDLE  mWinHeapHandle;
	Info	mInfo;

	// Back link to the memory system
	Mem     *pMem;

};

#endif

// ---  End of File ---------------

#ifndef MEM_H
#define MEM_H

// forward declaration
class Heap;
class Block;

// Singleton
class Mem
{
public:

	// Constants for the library
	static const unsigned int MAJOR_VERSION = 2;
	static const unsigned int MINOR_VERSION = 0;

	// alignment 
	enum Align
	{
		Byte_4,
		Byte_8,
		Byte_16,
		Byte_32,
		Byte_64,
		Byte_128,
		Byte_256
	};

	// Return codes
	enum Code
	{
		OK,
		ERROR_Heap_Create,
		ERROR_Illegal_Heap,
		ERROR_Mem_Not_Initialized,
		ERROR_Invalid_Addr,
		Dword = 0x7fffffff
	};

	// info structure
	struct Info
	{
		unsigned int CurrAllocIndex;
		unsigned int PeakHeapCount;
		unsigned int CurrHeapCount;
		unsigned int PeakNumAlloc;
		unsigned int CurrNumAlloc;
		unsigned int PeakBytesUsed; //make theese unsigned int's none of them should ever be negative
		unsigned int CurrBytesUsed;

	public:


		void AddBytes(unsigned int bytes)
		{
			this->CurrBytesUsed += bytes;

			if (this->CurrBytesUsed > this->PeakBytesUsed)
			{
				this->PeakBytesUsed = this->CurrBytesUsed;
			}
		}

		void RemoveBytes(unsigned int bytes)
		{
			this->CurrBytesUsed -= bytes;
		}
		

		unsigned int getCurAllocIndex()
		{
			return CurrAllocIndex;
		}

		void incCurAllocIndex()
		{
			CurrAllocIndex++;
		}

		void resetCurallocIndex()
		{
			CurrAllocIndex = 0;
		}

		void AddAlloc()
		{
			this->CurrAllocIndex++;

			this->CurrNumAlloc++;

			if (this->CurrNumAlloc > this->PeakNumAlloc)
			{
				this->PeakNumAlloc = this->CurrNumAlloc;
			}
		}

		void removeAlloc()
		{
			this->CurrNumAlloc--;
		}

		void AddHeap()
		{
			this->CurrHeapCount++;

			if (this->CurrHeapCount > this->PeakHeapCount)
			{
				this->PeakHeapCount = this->CurrHeapCount;
			}

		}

		void RemoveHeap()
		{
			this->CurrHeapCount--;
		}
	};

public:
	// Public Interface ------------------------------   for the customers

	// Initialize the mem system
	static Code Initialize();
	static Code Destroy();

	// Create an Heap from the heap system
	static Code HeapCreate(Heap *&newHeap, unsigned int heapSize, const char * const Name);
	static Code HeapCreate(Heap *&newHeap, unsigned int numBlocks, int sizePerBlock, const char * const Name);

	// Create an Heap from the heap system
	static Code HeapDestroy(Heap *inHeap);

	// Information
	static Code GetInfo(Info &info);
	static Code GetHeapByAddr(Heap *&pHeap, void *p);
	static Block *DebugGetGlobalTrackingHead(void);

	// Needed for Testings (Not for Customers)
	static Heap *DebugGetHeapHead(void);

	static bool FreeFromMyMem(void* p);
	static bool FreeFromMyMemA(void* p);

	// -----------------------------------------------------------------------------------
	// Add extra data or methods below this line
	// -----------------------------------------------------------------------------------

	static void* Alloc(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum);
	//static void* ArrayAlloc(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum);
	void AddBlockForTracking(Block* toAdd);
	void RemoveBlockForTracking(Block* toRemove);

	void privRemoveAlloc();
	void privRemoveBytes(unsigned int bytes);

private:

	Mem();
	~Mem();

	static unsigned int AlignEnumToPadVal(Mem::Align code);

	Code HeapAddToFront(Heap* h);
	Code HeapRemove(Heap* h);
	// data -----------------------------------------------------------------------
	static Mem* Instance;
	Heap	*poHead;
	Block	*pGlobalHead;
	Info	mInfo;
	bool	mInitialized;
	char	pad0;
	char    pad1;
	char    pad2;
};

#endif 

// ---  End of File ---------------

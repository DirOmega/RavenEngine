#include "Mem.h"
#include "Heap.h"
#include "GlobalNew.h"

#define STUB_PLEASE_REPLACE(x) (x)

// operator new
void * operator new( size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	return Mem::Alloc(inSize,pHeap,align,inName,lineNum);
}

void * operator new( size_t inSize )
{
	void *p = malloc( inSize );
	return p;
}

// operator placement delete
void operator delete( void *p, Heap *pHeap, Mem::Align align, char *inName, int lineNum)
{
	STUB_PLEASE_REPLACE(p);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
}

// operator delete
void operator delete( void *p )
{
	if (!Mem::FreeFromMyMem(p))//finds the heap if the pointer exists in my heap and then frees it, it returns true if the pointer was in my memory space.
	{
		free(p);
	}
}

void * operator new[]( size_t inSize ,Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	return Mem::Alloc(inSize, pHeap, align, inName, lineNum);
}

void operator delete[]( void *p,Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	STUB_PLEASE_REPLACE(p);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
}

void * operator new[]( size_t inSize )
{
	void *p = malloc( inSize );
	return p;
}

void operator delete[]( void *p )
{
	if (!Mem::FreeFromMyMemA(p))//finds the heap if the pointer exists in my heap and then frees it, it returns true if the pointer was in my memory space.
	{
		free(p);
	}
}

// ---  End of File ---------------

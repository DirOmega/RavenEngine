#include <assert.h>
#include "PCSTreeForwardIterator.h"

#define STUB_PLEASE_REPLACE(x) (x)

PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode *rootNode) : root(rootNode), current(rootNode)
{
}

PCSNode *PCSTreeForwardIterator::First()
{
	current = root;
	return  current;
}

PCSNode *PCSTreeForwardIterator::Next()
{
	//if (current != nullptr)
	if(current != root->GetNextSibling())
	{
		current = current->GetForward();
	}
	return current;
}

bool PCSTreeForwardIterator::IsDone()
{
	return (current == root->GetNextSibling());
}

PCSNode *PCSTreeForwardIterator::CurrentItem()
{
	return current;
}

//+		_m	{m128_f32=0x004ff8b0 {1.82809544, -0.0361938477, -5.73240471, 1.00000000} m128_u64=0x004ff8b0 {13624585142519070472, ...} ...}	__m128
//{_v0={_m={m128_f32=0x004ff750 {-7.56247520, 0.981169224, -2.41790962, 0.000000000} m128_u64=0x004ff750 {...} ...} ...} ...}

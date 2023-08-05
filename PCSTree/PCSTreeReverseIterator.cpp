#include <assert.h>
#include "PCSTreeReverseIterator.h"

#define STUB_PLEASE_REPLACE(x) (x)

PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode *rootNode) : root(rootNode), current(nullptr), prevNode(nullptr)
{
	if (root != nullptr) //if someone passes a null root
	{
		current = root->GetReverse();
	}
}

PCSNode *PCSTreeReverseIterator::First()
{
	PCSNode* retval = nullptr;
	
	if (root != nullptr) //defend against someone removing a node while the iterator is on it
	{
		prevNode = nullptr;
		retval = root->GetReverse();
		current = retval;

	}
	return retval;
}

PCSNode *PCSTreeReverseIterator::Next()
{
	if (current != nullptr) //just to defend against someone removing a node while the iterator is on it
	{
		prevNode = current;
		if (prevNode == root)
		{
			current = nullptr;
		}
		else
		{
			current = current->GetReverse();
		}
	}
	return current;
}

bool PCSTreeReverseIterator::IsDone()
{
	return (prevNode == root || current == nullptr); //just in case you remove the node while it's in a list
}

PCSNode *PCSTreeReverseIterator::CurrentItem()
{
	return current;
}

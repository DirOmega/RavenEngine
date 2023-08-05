#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "Trace.h"

#include "PCSTree.h"
#include "PCSNode.h"

#define STUB_PLEASE_REPLACE(x) (x)

void PCSNode::nameCpy(const char* const pInName)
{
	for (int i = 0; i < PCSNode::NAME_SIZE; i++)
	{
		pName[i] = pInName[i];
		if (i == PCSNode::NAME_SIZE - 1) { pName[i] = '\0'; }
	}
}

// constructor
PCSNode::PCSNode() :
	pParent(nullptr), pChild(nullptr),
	pNextSibling(nullptr), pPrevSibling(nullptr),
	pForward(nullptr), pReverse(nullptr), pName{ 0 }
{ }

// copy constructor
PCSNode::PCSNode(const PCSNode &in) :
	pParent(in.pParent), pChild(in.pChild),
	pNextSibling(in.pNextSibling), pPrevSibling(in.pPrevSibling),
	pForward(in.pForward), pReverse(in.pReverse)
{
	nameCpy(in.pName);
}

// Specialize Constructor
PCSNode::PCSNode(PCSNode * const pInParent, PCSNode * const pInChild, PCSNode * const pInNextSibling, PCSNode * const pInPrevSibling, const char * const pInName):
	pParent(pInParent), pChild(pInChild),
	pNextSibling(pInNextSibling), pPrevSibling(pInPrevSibling),
	pForward(nullptr), pReverse(nullptr)
{
	nameCpy(pInName);
}


PCSNode::PCSNode(const char * const pInName) :
	pParent(nullptr), pChild(nullptr),
	pNextSibling(nullptr), pPrevSibling(nullptr),
	pForward(nullptr), pReverse(nullptr)
{
	nameCpy(pInName);
}

// destructor
PCSNode::~PCSNode()
{	}

// assignment operator
PCSNode &PCSNode::operator = (const PCSNode &in)
{
	pParent = in.pParent;
	pChild = in.pChild;
	pNextSibling = in.pNextSibling;
	pPrevSibling = in.pPrevSibling;
	pForward = in.pForward;
	pReverse = in.pReverse;
	nameCpy(in.pName);
	return *this;
}

void PCSNode::SetParent(PCSNode * const pIn)
{
	pParent = pIn;
}

void PCSNode::SetChild(PCSNode * const pIn)
{
	pChild = pIn;
}

void PCSNode::SetForward(PCSNode * const pIn)
{
	pForward = pIn;
}

void PCSNode::SetReverse(PCSNode * const pIn)
{
	pReverse = pIn;
}

void PCSNode::SetNextSibling(PCSNode * const pIn)
{
	pNextSibling = pIn;
}

void PCSNode::SetPrevSibling(PCSNode * const pIn)
{
	pPrevSibling = pIn;
}

PCSNode *PCSNode::GetParent(void) const
{
	return pParent;
}

PCSNode *PCSNode::GetChild(void) const
{
	return pChild;
}

PCSNode *PCSNode::GetNextSibling(void) const
{
	return pNextSibling;
}

PCSNode *PCSNode::GetPrevSibling(void) const
{
	return pPrevSibling;
}

PCSNode *PCSNode::GetForward(void) const
{
	return pForward;
}

PCSNode *PCSNode::GetReverse(void) const
{
	return pReverse;
}

PCSNode::Code PCSNode::SetName(const char * const pInName)
{
	PCSNode::Code retval = PCSNode::Code::DWORD;
	if (pInName == nullptr)
	{
		retval = PCSNode::Code::FAIL_NULL_PTR;
	}
	else
	{
		nameCpy(pInName);
		retval = PCSNode::Code::SUCCESS;
	}
	
	return retval;
}

PCSNode::Code PCSNode::GetName(char * const pOutBuffer, unsigned int sizeOutBuffer) const
{
	Code retval = Code::SUCCESS;
	if (pOutBuffer == nullptr)
	{
		retval = Code::FAIL_NULL_PTR;
	}
	else
	{
		for (unsigned int i = 0; i < PCSNode::NAME_SIZE; i++) //
		{
			if (i < sizeOutBuffer)
			{
				pOutBuffer[i] = pName[i];
				//if (pName[i] == '\0') { break; }
			}
			else
			{
				pOutBuffer[i - 1] = '\0';
				break;
			}
		}
	}

	return retval;
}

void PCSNode::PrintNode() const
{
	Trace::out("%s\n", pName);
}

void PCSNode::PrintChildren() const
{
	PCSNode const * cursor = this->GetChild();
	Trace::out("\nPrinting Children () -------------------------------\n");

	if (cursor != nullptr)
	{
		cursor->PrintSiblings();
		cursor = cursor->GetChild();
	}

	Trace::out("\nEnd Print Children --------------------------------\n\n\n");
}


void PCSNode::PrintSiblings() const
{
	Trace::out("\nPrinting Siblings() ----------------------------------\n");

	PCSNode* cursor = this->GetParent();
	if (cursor != nullptr)
	{
		cursor = cursor->GetChild();

		while (cursor != nullptr)
		{

			cursor->PrintNode();
			cursor = cursor->GetNextSibling();
		}
	}
	else
	{
		//this is just in case someone does something strange like make a test that has nodes in a linked list config
		cursor = this->GetNextSibling();
		while (cursor != nullptr)
		{
			cursor->PrintNode();
			cursor = cursor->GetNextSibling();
		}
		cursor = this->GetPrevSibling();
		while (cursor != nullptr)
		{
			cursor->PrintNode();
			cursor = cursor->GetPrevSibling();
		}
	}

	Trace::out("\nEnd Printing Siblings --------------------------------\n");
}



int PCSNode::GetNumSiblings() const
{
	PCSNode* cursor = this->GetNextSibling();
	int count = 1;
	while (cursor != nullptr)
	{
		count++;
		cursor = cursor->GetNextSibling();
	}
	cursor = this->GetPrevSibling();
	while (cursor != nullptr)
	{
		count++;
		cursor = cursor->GetPrevSibling();
	}

	return count;
}

int PCSNode::GetNumChildren() const
{
	PCSNode const * cursor = this->GetChild();
	int count = 0;

	if (cursor != nullptr)
	{
		count += cursor->GetNumSiblings();
		cursor = cursor->GetChild();
	}
	return count;
}

// ---  End of File ---------------

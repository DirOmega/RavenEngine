#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Trace.h"
#include "PCSTree.h"
#include "PCSNode.h"

#define STUB_PLEASE_REPLACE(x) (x)

// constructor
PCSTree::PCSTree() : mInfo{ 0 }, pRoot(nullptr)
{
};

// destructor
PCSTree::~PCSTree()
{
};


// get Root
PCSNode *PCSTree::GetRoot() const
{
	return pRoot;
}


// insert
void PCSTree::Insert(PCSNode * const pInNode, PCSNode * const pParent)
{
	assert(pInNode != nullptr);

	if (nullptr == pParent)
	{ //insert as root.
		this->pRoot = pInNode;
		//this->pRoot->SetReverse(pRoot);//odd I know but it may change
		//this->mInfo.InfoAddNode();
	}
	else // insert inside tree
	{
		if (pParent->GetChild() == nullptr)
		{ //first child add directly
			pParent->SetChild(pInNode);
			pInNode->SetParent(pParent);

			pInNode->SetForward(pParent->GetForward());
			pParent->SetForward(pInNode);

			pInNode->SetReverse(pParent);
			if (pInNode->GetForward() != nullptr)
			{
				pInNode->GetForward()->SetReverse(pInNode);
			}
		}
		else
		{//add as sibling

			//Get the first sibling on level
			PCSNode* first = pParent->GetChild();
			first->SetPrevSibling(pInNode);

			//update the pInNode PCS pointers
			pInNode->SetParent(pParent);
			pInNode->SetChild(nullptr); //may want to change this if I want to be allowed to add/detach subtrees to an existing tree
			pInNode->SetNextSibling(first);
			pInNode->SetPrevSibling(nullptr);

			//update the pInNode Iterator points
			pInNode->SetForward(pParent->GetForward());
			pParent->SetForward(pInNode);
			//if I'm here there is a next node
			pInNode->SetReverse(pParent);
			pInNode->GetForward()->SetReverse(pInNode);

			//pInNode->SetReverse(pInNode->GetForward()->);
			//pInNode->SetReverse(pParent->GetForward()->GetReverse());

			//first->SetForward(pInNode);

			pParent->SetChild(pInNode);


			//this->mInfo.InfoAddNode();
		}
		//if the node is the last in the list update proot to point to it to start the reverse


	}

	if (pInNode->GetForward() == nullptr)
	{
		this->pRoot->SetReverse(pInNode);
	}

	this->mInfo.InfoAddNode();
}


// Remove
void PCSTree::Remove(PCSNode * const pInNode)
{
	assert(pInNode != nullptr);

	if (pInNode->GetChild() == nullptr)
	{
		//leaf node

		PCSNode* pParent = pInNode->GetParent();

		// special case root
		if (pParent == nullptr)
		{
			this->pRoot = nullptr;
		}
		else
		{

			//Linked list update
			//pParent->SetForward(pInNode->GetForward());
			if (pInNode->GetForward() != nullptr) //can be null because the foreward list runs off into null
			{
				pInNode->GetForward()->SetReverse(pInNode->GetReverse());
			}
			else
			{
				pRoot->SetReverse(pInNode->GetReverse());
			}

			pInNode->GetReverse()->SetForward(pInNode->GetForward()); //famous last words: get reverse here can't be null because the reverse iterator is circular even just the root points to itself
			//end of linked list update

			if (pInNode->GetPrevSibling() == nullptr)
			{
				//child of parent need to move foreward.
				pParent->SetChild(pInNode->GetNextSibling());

			}
			else
			{
				pInNode->GetPrevSibling()->SetNextSibling(pInNode->GetNextSibling());
			}

			if (pInNode->GetNextSibling() == nullptr)
			{
				//pInNode->get
			}
			else
			{
				pInNode->GetNextSibling()->SetPrevSibling(pInNode->GetPrevSibling());
				pInNode->SetNextSibling(nullptr);
			}
		}

		pInNode->SetParent(nullptr);
		pInNode->SetPrevSibling(nullptr);
		pInNode->SetNextSibling(nullptr);
		pInNode->SetForward(nullptr);
		pInNode->SetReverse(nullptr);
		mInfo.InfoRemoveNode();
	}
	else
	{
		// If we are here, recursively call
		PCSNode* pTmp = pInNode->GetChild();
		assert(pTmp != nullptr);

		this->Remove(pTmp);
		this->Remove(pInNode);
	}
}

// get tree info
void PCSTree::GetInfo(Info &info)
{
	int i = 0;
	int max = 0;
	PCSNode* cur = this->pRoot;

	if (cur != nullptr)
	{
		privGetLayers(cur, i, max);
	}

	mInfo.InfoSetCurrentLayer(max);


	info = this->mInfo;
}

void PCSTree::Print() const
{
	Trace::out("\ndumpTree () -------------------------------\n");
	this->privDumpTreeDepthFirst(this->pRoot);
	Trace::out("dumpTree END -------------------------------\n\n\n");
}

void PCSTree::privDumpTreeDepthFirst(PCSNode* pNode) const
{
	PCSNode* pChild = nullptr;

	// dump
	pNode->PrintNode();

	// iterate through all of the active children 
	if (pNode->GetChild() != nullptr)
	{
		pChild = pNode->GetChild();
		// make sure that allocation is not a leaf node 
		while (pChild != nullptr)
		{
			privDumpTreeDepthFirst(pChild);
			// goto next sibling
			pChild = pChild->GetNextSibling();
		}
	}
	else
	{
		// bye bye exit condition
	}
}

void PCSTree::privGetLayers(PCSNode* pCur, int& layers, int& max)
{
	layers++;
	if (pCur->GetChild() != nullptr)
	{
		PCSNode* pChild = pCur->GetChild();

		while (pChild != nullptr)
		{

			privGetLayers(pChild, layers, max);
			pChild = pChild->GetNextSibling();
		}
	}
	else
	{
		if (layers > max)
		{
			max = layers;
		}

	}
	layers--;
}
// ---  End of File ---------------

#ifndef PCSTREE_H
#define PCSTREE_H

// forward declare
class PCSNode;

// PCSTree class 
class PCSTree
{
public:
	// Simple data structure
	struct Info
	{

		int currNumNodes;
		int maxNumNodes;
		int currNumLevels;
		int maxNumLevels;

		void InfoAddNode()
		{
			this->currNumNodes += 1;

			if (this->currNumNodes > this->maxNumNodes)
			{
				this->maxNumNodes += 1;
			}
		}

		void InfoRemoveNode()
		{
			currNumNodes -= 1;
		}

		void InfoSetCurrentLayer(int layercount)
		{
			currNumLevels = layercount;
			if (currNumLevels > maxNumLevels)
			{
				this->maxNumLevels = currNumLevels;
			}
		}
	};

public:

	// constructor
	PCSTree();

	// copy constructor 
	PCSTree(const PCSTree &in) = delete;

	// assignment operator
	PCSTree & operator = (const PCSTree &in) = delete;

	// destructor
	~PCSTree();

	// get Root
	PCSNode *GetRoot() const;

	// insert
	void Insert(PCSNode * const pInNode, PCSNode * const pParent);

	// remove
	void Remove(PCSNode * const pInNode);

	// get info
	void GetInfo(Info &info);
	void Print() const;

private:

	// Data
	Info	mInfo;
	PCSNode *pRoot;
	//PCSNode *pReverseStart;

	void privDumpTreeDepthFirst(PCSNode* pNode) const;
	void privGetLayers(PCSNode* counter, int& layers, int& max);
};

#endif

// ---  End of File ---------------

#pragma once
//#include "Common.h"

#include"DLink.h"
#include "Trace.h"
#include <assert.h>

class baseManager //: iManager
{
public:

	int getActive();

	int getReserve();

	int getTotal();

	bool Remove(DLink& toRemove);

	int debugCountActive();
	int debugCountReserve();

	int debugCountAll();

	DLink* getActiveHead();

protected:

	//implmementation contract
	virtual DLink* dCreateNode() = 0;

	virtual bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) = 0;

	virtual void dClearNode(DLink* pLink) = 0;

	DLink* getReserveHead();

	DLink* baseAdd();

	DLink* baseAddSort(DLink& toAdd);

	baseManager(int deltaAdd = 3);

	virtual ~baseManager();

	DLink* baseFind(DLink& pTarget);


private:

	//private copy constructor
	baseManager(baseManager &) = delete;
	//private move constructor
	baseManager(baseManager && x) = default;
	//private assignment operator
	baseManager& operator=(const baseManager& x) & = default;
	//private move assignment operator
	baseManager& operator=(baseManager&& x) & = default;

	void removeFromActive(DLink& toRemove);

	void recycleToReserve(DLink& toRemove);


	void generateReserve();


protected:

	DLink *pReserveHead;
	DLink *pActiveHead;
	int mnumStart;
	int mDeltaAdd;
	int mNumActive;
	int mNumReserved;

	void generateReserves(int numToAdd);

	DLink* getFromReserve();

	//TODO: update this. 
	DLink* addToActive(DLink& newLink);


	

};

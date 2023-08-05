#pragma once

#include <assert.h>
#include "baseManager.h"
#include "TimeEvent.h"
#include "Command.h"

//enum TimeEventID
//{
//	undef, Anim, exp, disp, create,
//	GridMove
//};

class TimerManager : baseManager
{

private:
	static unsigned int currTime;
	static TimerManager* pInstance;
	static TimeEvent* poRefTE;
	static Command* pNullCMD;
	TimerManager(int numStart = 5, int deltaAdd = 3)
		: baseManager(deltaAdd)
	{
		numStart;
		currTime = 0;
		FlagQuit = false;
		pNullCMD = new NullCMD();
		//	 pNullCMD = new NullCMD();

		poRefTE = new TimeEvent();
	}

public:
	//just so I have some way to end the demo
	bool FlagQuit;
	void quit()
	{
		FlagQuit = true;
	}

	void resetQuitFlag()
	{
		//I'm restting everything for the next demo.
		FlagQuit = false;
		currTime = 0;
	}

	//TODO: clean this up it's horrid. I don't think on remove a node is cleaned so I should be able to eleminate the if/else
	//but currently when I try to factor it out it breaks the list.
	void Update(unsigned int gameTime)
	{
		currTime += gameTime;
		TimeEvent* e = (TimeEvent*)getActiveHead();
		while (e != nullptr)
		{

			if (currTime >= e->triggerTime)
			{
				e->process();
				if (e->pNext != nullptr)
				{
					e = (TimeEvent*)e->pNext;
					Remove(*e->pPrev);
				}
				else
				{
					Remove(*e);
					e = nullptr;
				}
			}
			else
			{
				break;
			}
		}
	}

	static void Create(int numStart = 5, int deltaAdd = 3)
	{
		assert(numStart > 0);
		assert(deltaAdd > 0);
		assert(pInstance == nullptr);

		if (pInstance == nullptr)
		{
			pInstance = new TimerManager(numStart, deltaAdd);
		}
	}

	static TimeEvent* Add(TimeEventID Name, Command& cmd, unsigned int DTime)
	{
		assert(DTime >= 0.0f);

		TimerManager* pTMan = TimerManager::getInstance();
		assert(pTMan != nullptr);

		//TimeEvent pNode = (TimeEvent)pTMan.baseAdd();
		TimeEvent* pNode = (TimeEvent*)pTMan->getFromReserve();
		assert(pNode != nullptr);
		//pNode.set(Name,DTime,TTime);
		pNode->set(Name, DTime, currTime + DTime, cmd);
		//InsertionSort
		pTMan->InSort(*pNode);

		return pNode;
	}

	TimeEvent* InSort(TimeEvent& te)
	{
		baseManager::baseAddSort(te);
		return &te;
	}

	static int getCurrentTime()
	{
		return currTime;
	}

	static TimerManager* getInstance()
	{
		if (pInstance == nullptr)
		{
			TimerManager::Create();
		}
		return pInstance;
	}

	TimeEvent* Find(TimeEventID eventID)
	{
		TimerManager* mrT = TimerManager::getInstance();
		assert(mrT != nullptr);
		TimeEvent* target = mrT->toFind(eventID);
		return (TimeEvent*)mrT->baseFind(*target);
	}

protected:

	void dClearNode(DLink* pLink)
	{
		TimeEvent* p = (TimeEvent*)pLink;
		p->dClean();
	}

	//void dClearNode(DLink* pLink)
	//{
	//	TimeEvent* p = (TimeEvent*)pLink;
	//	p->dClean();
	//}

	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		//assert(pLinkA != nullptr);
		//assert(pLinkB != nullptr);

		TimeEvent& left = (TimeEvent&)pLinkA;
		TimeEvent& right = (TimeEvent&)pLinkB;

		if (left.Name == right.Name)
		{
			return true;
		}
		return false;
	}

	DLink* dCreateNode()
	{
		DLink* newNode = new TimeEvent();
		assert(newNode != nullptr);

		return newNode;
	}

	//for find
	TimeEvent* toFind(TimeEventID id)
	{
		poRefTE->Name = id;
		return poRefTE;
	}
};

class CMDQuit : public Command
{
public:
	CMDQuit()
	{
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		TimerManager* tm = TimerManager::getInstance();
		OutputDebugStringW(L"QUIT COMMAND CALLED!!!!!!!");
		tm->quit();
	}
};

#pragma once

#include "SourceVoice.h"
#include "SourceVoiceTopManager.h"
const int MaxSounds = 6;

class PriorityManager
{
private:
	SourceVoice* Active[MaxSounds] = { nullptr };
	bool SlotActive[MaxSounds] = { false };
	static PriorityManager* pInstance;
	PriorityManager()
	{
	}

public:
	static PriorityManager* getInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new PriorityManager();
		}
		return pInstance;
	}

	static void printActive()
	{
		PriorityManager* pMan = PriorityManager::getInstance();
		Trace::out("------- Active Table ------------------\n");
		SourceVoice* cur;
		for (int i = 0; i < MaxSounds; i++)
		{
			if (pMan->SlotActive[i])
			{
				cur = pMan->Active[i];

				//Trace::out("TableLine\n");
				//Trace::out("Handle : %08X  ", cur->handle);
				Trace::out("Priority %i   ", cur->getPriority());
				Trace::out("ActiveTime %i ms \n", cur->getPlayTime());

				//Trace::out("Handle : %08X Priority: %i ActiveTime %i\n", cur->handle, cur->getPriority(), cur->getPlayTime());
			}
		}
	}

	//TODO: I know this is horribbly ineffecient 
	//prototype code.
	static void play(SourceVoice* sv)
	{
		PriorityManager* pMan = PriorityManager::getInstance();
		//check for free
		int MaxPri = -1;
		int MaxPriSlot = -1;
		bool Added = false;
		for (int i = 0;i < MaxSounds;i++)
		{
			if (!pMan->SlotActive[i])
			{
				Added = true;
				pMan->SlotActive[i] = true;
				pMan->Active[i] = sv;
				sv->play();
				break;
			}
			else
			{
				if (pMan->Active[i]->getPriority() > MaxPri)
				{
					MaxPriSlot = i;
					MaxPri = pMan->Active[i]->getPriority();
				}
			}
		}

		if (!Added && MaxPri >= sv->getPriority())
		{
			if (MaxPri == sv->getPriority())
			{
				unsigned int maxTime = 0;
				int oldestSlot = -1;
				for (int i = 0;i < MaxSounds;i++)
				{
					if (pMan->Active[i]->getPlayTime() > maxTime && pMan->Active[i]->getPriority() == sv->getPriority())
					{
						maxTime = pMan->Active[i]->getPlayTime();
						oldestSlot = i;
					}

				}
				
				pMan->Active[oldestSlot]->stop();
				//SourceVoiceManager::returnSourceVoice(*pMan->Active[oldestSlot]);
				pMan->Active[oldestSlot] = sv;
				sv->play();
			}
			else
			{
				pMan->Active[MaxPriSlot]->stop();
				//SourceVoiceManager::returnSourceVoice(*pMan->Active[MaxPriSlot]);
				pMan->Active[MaxPriSlot] = sv;
				sv->play();
			}
		}
		printActive();
	}
};

class printActiveCMD : public Command
{
public:
	printActiveCMD()
	{
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		PriorityManager::printActive();
	}
};

class TriggerVoiceCMDpt : public Command
{
public:
	SourceVoice* sv;

	TriggerVoiceCMDpt(SourceVoice* sv)
	{
		this->sv = sv;
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		PriorityManager::play(sv);

	}
};

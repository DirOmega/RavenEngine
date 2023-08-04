#pragma once
//#include "Common.h"
#include "DLink.h"
#include "Command.h"

enum TimeEventID
{
	undef, PlayOnce, PlayAndKeep,
	Loop, Quit, User, TriggerVoice,
	StopVoice, PlayAndPan, PlayAndVol,
	PrintPlayTime//, PrintPriTable
};

class TimeEvent : public DLink
{
public:

	unsigned int deltaTime;
	//delta passed to manager, trigger passed to time event.
	unsigned int triggerTime;
	TimeEventID Name;
	Command* CMD;

	TimeEvent()
	{
		triggerTime = 0;
		Name = TimeEventID::undef;
		//temp need to create a singleton NullCMD to refrence
		CMD = new NullCMD();
		//	CMD = new NullCMD();
	}

	void process()
	{
		//System.Diagnostics.WriteLine("triggerTime = {0}", triggerTime);
		CMD->execute(deltaTime);
	}

	void set(TimeEventID name, unsigned int TTime)
	{
		Name = name;
		triggerTime = TTime;
	}

	void set(TimeEventID name, unsigned int DTime, unsigned int TTime)
	{
		deltaTime = DTime;
		triggerTime = TTime;
		Name = name;
	}

	void set(TimeEventID name, unsigned int DTime, unsigned int TTime, Command& cmd)
	{
		Name = name;
		deltaTime = DTime;
		triggerTime = TTime;
		CMD = &cmd;
	}

	void set(Command& cmd)
	{
		CMD = &cmd;
	}

	//Bad smell Not sure how I like the greaterThan system, I think I want to do the pass up value for comparison.
	//I always want the earliest insertion so I am using greaterthan or equals to.
	bool greaterThan(DLink* that)
	{
		assert(that != nullptr);

		TimeEvent* TE = (TimeEvent*)that;
		return this->triggerTime >= TE->triggerTime;
	}

	void dClean()
	{
		//   deltaTime = 0f;
		triggerTime = 0;
		Name = TimeEventID::undef;
	}
};

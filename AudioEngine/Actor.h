#pragma once
#include "AudioBufferManager.h"
#include "SourceVoiceTopManager.h"


//Example simple client object
//and handle/Pimpl example.
class ActorPimpl;
class Script;
class Line;

class Actor
{
private:
	ActorPimpl* RealActor;
	//SndHandle handle;
	Handle handle;

public:

	Actor();
	~Actor();

	Actor(const Actor &r);
	const Actor & operator = (const Actor &r);

	HandleStatus destroy();

	ActorPimpl* operator->();

	HandleStatus fromTheTop();

	HandleStatus giveVoice(VoiceTypeID svt, Command* LineEndCommand, CircularData* pOut);
	HandleStatus giveScript(Script* script);

	HandleStatus setPartner(Actor& b);

	HandleStatus sayLine();
	HandleStatus lineEnd();

	HandleStatus setVolume(float volLevl);

};

class Script
{
	DLink* pLineHead;
	Line* pcurLine;

public:
	Script();

	void addLine(AudioDataBuffer& buff, char* name);

	void fromTheTop();

	AudioDataBuffer* sayLine();
};

//class Scene
//{
//
//};
//
//class Dialog : public Scene
//{
//	Actor* a;
//	Actor* b;
//
//
//
//};


//Example user created commands.
class LineEndCMD : public Command
{
	Actor* actor;
public:
	LineEndCMD(Actor* actor) :actor(actor)
	{
	}

	void execute()
	{
		actor->lineEnd();
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		execute();
	}

};

class SayLineCMD : public Command
{
	Actor* actor;

public:
	SayLineCMD(Actor* actor) :actor(actor)
	{

	}

	void execute()
	{
		OutputDebugStringW(L"LineEnd!\n");
		actor->lineEnd();
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		execute();
	}

};

class TriggerNextActorCmd : public Command
{
	Actor* toTrigger;

public:

	TriggerNextActorCmd(Actor* toTrigger) :toTrigger(toTrigger)
	{
	}

	void execute(unsigned int deltaTime)
	{
		deltaTime;
		execute();
	}

	void execute()
	{
		//OutputDebugStringW(L"Actor Triggered\n");
		toTrigger->sayLine();
	}
};

class CleanupActorCMD : public Command
{
	Actor* a;
public: 
	CleanupActorCMD(Actor* a) : a(a)
	{

	}

	void execute(unsigned int)
	{
		delete a;
	}


};
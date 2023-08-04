#include "Actor.h"
//#include "SndHandle.h"
#include "Trace.h"

class Line : public DLink
{
	AudioDataBuffer* line;
	char* lineName;
	
public:
	Line(AudioDataBuffer& buff, char* lineName) : line(&buff), lineName(lineName)
	{
	}

	AudioDataBuffer* getLine()
	{
		return line;
	}

	char* getLineName()
	{
		return lineName;
	}

	~Line()
	{
		delete line;
	}
	void dClean()
	{
		delete line;
	}
};

Script::Script()
{
	pLineHead = nullptr;
	pcurLine = nullptr;
}

void Script::addLine(AudioDataBuffer& buff, char* name)
{
	buff.setStreamEnd();
	DLink* tmp = new Line(buff, name);

	DLink::addToFront(pLineHead, tmp);
}

void Script::fromTheTop()
{
	pcurLine = (Line*)pLineHead;
}

AudioDataBuffer* Script::sayLine()
{
	if (pcurLine != nullptr) {
		Line* toSay = pcurLine;
		pcurLine = (Line*)pcurLine->pNext;
		//can optionally unload completed lines.
		Trace::out("Stiching %s \n", toSay->getLineName());
		return toSay->getLine();
	}
	else
	{
		return nullptr;
	}
}


class ActorPimpl
{
	SourceVoice* sv;
	Script* pScript;
	Actor* partner;
	//SndHandle handle;

public:
	ActorPimpl()
	{
		sv = nullptr;
		pScript = nullptr;
	}

	void setVol(float vol)
	{
		sv->setVolume(vol);
	}

	float getVol()
	{
		float ret;
		sv->getVolume(ret);
		return ret;
	}


	virtual ~ActorPimpl()
	{
		if (pScript != nullptr) {

			delete pScript;
		}
		if (sv != nullptr) {
			SourceVoiceManager::returnSourceVoice(*sv);
		}
	}

	const ActorPimpl& operator = (const ActorPimpl &r)
	{
		this->sv = r.sv;
		this->pScript = r.pScript;
		this->partner = r.partner;
	}


	void fromTheTop()
	{
		pScript->fromTheTop();
	}

	void giveVoice(VoiceTypeID svt, Command* LineEndCommand, CircularData* pOut)
	{
		//this->sv = sv;
		//Command* LECMD = new LineEndCMD(this);
		SourceVoiceManager::getSourceVoice(svt, sv);
		sv->setpOut(pOut);
		sv->setStreamEndCMD(*LineEndCommand);
	}

	void giveScript(Script* script)
	{
		pScript = script;
	}

	void setPartner(Actor& b)
	{
		partner = &b;
	}

	void sayLine()
	{
		//OutputDebugStringW(L"Actor Recived Trigger\n");
		Trace::out("Actor Recieved Trigger  ");
		AudioDataBuffer* l = pScript->sayLine();
		if (l != nullptr) {

			//OutputDebugStringW(L"Actor submitting non null audio\n");
			//Trace::out("Stiching %s \n", pScript->);
			sv->SubmitAudio(*l);
			sv->play();
		}
		if (l == nullptr)
		{
			SourceVoiceManager::returnSourceVoice(*sv);
			sv = nullptr;
		}
	}

	void lineEnd()
	{
		sv->stop();
		if (partner != nullptr) {
			partner->sayLine();
		}
	}
};



HandleStatus Actor::setVolume(float volLevl)
{
	HANDLE_CHECK(this->handle)

	Trace::out("   Actor(0x%p):() handle(%x): 0x%X \n");
		Trace::out("      volume Set to %f\n", volLevl);

		this->RealActor->setVol(volLevl);

	HANDLE_CHECK_END
}

Actor::Actor() :RealActor(new ActorPimpl()), handle()
{

	Trace::out("   Actor(0x%p):() handle(%x): 0x%X \n",
		this, handle.GetIndex(), handle.GetID());

	//RealActor = new ActorPimpl();
}

Actor::~Actor()
{
	if (RealActor != nullptr)
	{
		delete RealActor;
	}
}

Actor::Actor(const Actor &r) :RealActor(r.RealActor), handle()
{
}

const Actor & Actor::operator = (const Actor &r)
{
	//Don't know if this is a good idea yet.
	if (RealActor != nullptr) { delete this->RealActor; }

	this->RealActor = r.RealActor;

	Trace::out("   Actor(0x%p)operator=:\n", this);
	Trace::out("           ->r   : Actor(0x%p):0x%p  handle(%x): 0x%X \n", &r, this->RealActor, r.handle.GetIndex(), r.handle.GetID());
	Trace::out("           ->this: Actor(0x%p):0x%p handle(%x): 0x%X \n", this, this->RealActor, this->handle.GetIndex(), this->handle.GetID());

	return *this;
}


HandleStatus Actor::destroy()
{
	HANDLE_CHECK(this->handle)

		// Demo to showcase a method that invalidates the resources
		HANDLE_INVALIDATE(this->handle);

	HANDLE_CHECK_END
}

ActorPimpl* Actor::operator->()
{
	//I don't know if this breaks the point of the handles.
	return RealActor;
}

HandleStatus Actor::fromTheTop()
{
	HANDLE_CHECK(this->handle)

		RealActor->fromTheTop();

	HANDLE_CHECK_END

}

HandleStatus Actor::giveVoice(VoiceTypeID svt, Command* LineEndcommand, CircularData* pOut)
{
	HANDLE_CHECK(this->handle)

		RealActor->giveVoice(svt, LineEndcommand, pOut);

	HANDLE_CHECK_END

}


HandleStatus Actor::giveScript(Script* script)
{
	HANDLE_CHECK(this->handle)

		RealActor->giveScript(script);

	HANDLE_CHECK_END

}

HandleStatus Actor::setPartner(Actor& b)
{
	HANDLE_CHECK(this->handle)

		RealActor->setPartner(b);

	HANDLE_CHECK_END

}

HandleStatus Actor::sayLine()
{
	HANDLE_CHECK(this->handle)

		RealActor->sayLine();

	HANDLE_CHECK_END

}

HandleStatus Actor::lineEnd()
{
	HANDLE_CHECK(this->handle)

		RealActor->lineEnd();

	HANDLE_CHECK_END

}



//
//class Line : public DLink
//{
//	AudioDataBuffer* line;
//public:
//	Line(AudioDataBuffer& buff)
//	{
//		line = &buff;
//	}
//
//	AudioDataBuffer* getLine()
//	{
//		return line;
//	}
//
//	~Line()
//	{
//		delete line;
//	}
//	void dClean()
//	{
//		delete line;
//	}
//};
//class Script
//{
//	DLink* pLineHead;
//	Line* pcurLine;
//
//public:
//	Script()
//	{
//		pLineHead = nullptr;
//		pcurLine = nullptr;
//	}
//
//	void addLine(AudioDataBuffer& buff)
//	{
//		buff.setStreamEnd();
//		DLink* tmp = new Line(buff);
//
//		DLink::addToFront(pLineHead, tmp);
//	}
//
//	void fromTheTop()
//	{
//		pcurLine = (Line*)pLineHead;
//	}
//
//	AudioDataBuffer* sayLine()
//	{
//		if (pcurLine != nullptr) {
//			Line* toSay = pcurLine;
//			pcurLine = (Line*)pcurLine->pNext;
//			//can optionally unload completed lines.
//			return toSay->getLine();
//		}
//		else
//		{
//			return nullptr;
//		}
//	}
//
//};
//class Actor
//{
//	SourceVoice* sv;
//	Script* pScript;
//	Actor* partner;
//	SndHandle handle;
//public:
//	Actor()
//	{
//		sv = nullptr;
//		pScript = nullptr;
//	}
//
//	virtual ~Actor()
//	{
//		if (pScript != nullptr) {
//
//			delete pScript;
//		}
//		if (sv != nullptr) {
//			SourceVoiceManager::returnSourceVoice(*sv);
//		}
//	}
//
//	void fromTheTop()
//	{
//		pScript->fromTheTop();
//	}
//
//	void giveVoice(VoiceTypeID svt, Command* LineEndCommand)
//	{
//		//this->sv = sv;
//		//Command* LECMD = new LineEndCMD(this);
//		SourceVoiceManager::getSourceVoice(svt, sv);
//		sv->setStreamEndCMD(*LineEndCommand);
//	}
//
//	void giveScript(Script* script)
//	{
//		pScript = script;
//	}
//
//	void setPartner(Actor& b)
//	{
//		partner = &b;
//	}
//
//	void sayLine()
//	{
//		OutputDebugStringW(L"Actor Recived Trigger\n");
//		AudioDataBuffer* l = pScript->sayLine();
//		if (l != nullptr) {
//
//			OutputDebugStringW(L"Actor submitting non null audio\n");
//			sv->SubmitAudio(*l);
//			sv->play();
//		}
//	}
//
//	void lineEnd()
//	{
//		sv->stop();
//		if (partner != nullptr) {
//			partner->sayLine();
//		}
//	}
//};
//
//
////Example user created commands.
//class LineEndCMD : public Command
//{
//	Actor* actor;
//public:
//	LineEndCMD(Actor* actor) :actor(actor)
//	{
//	}
//
//	void execute()
//	{
//		actor->lineEnd();
//	}
//
//	void execute(unsigned int deltaTime)
//	{
//		execute();
//	}
//
//};
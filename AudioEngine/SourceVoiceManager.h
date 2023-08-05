#pragma once

#include <assert.h>
//#include "StarterHelperFunctions.h"
#include "baseManager.h"
#include "SourceVoice.h"
#include "LoadBuffer.h"

//enum TimeEventID
//{
//	undef, Anim, exp, disp, create,
//	GridMove
//};

//enum VoiceTypeID
//{
//	error, mono44100, stereo44100
//};
//

//I want to clean all classes in this file up to be a factory and pool wrapped into one.
//I want the Sourcevoice manager to have a list of nodes that contain source voice type managers
//each type manager manages the resources of a specfic type of source voice.
//Since the source voice is basically locked in once created it doesn't need to do any cleaning or setting
//after creation which would make this a simpler pool than the other managers.
//but the multi dimensions allow me to manage multiple types of voices with different WFX stats;
//I also want to hide everything that isnt the top level manager in a cpp file and make it a PIMPL.
class SourceVoiceTypeManager : public baseManager
{
private:
	//VoiceTypeID Name;
	WAVEFORMATEX refWFX;
	//CircularData* pOut;

public:

	SourceVoiceTypeManager(WAVEFORMATEX wfx, int numStart = 5, int deltaAdd = 3)
		: baseManager(deltaAdd)
	{
		refWFX = wfx;
		generateReserves(numStart);
		//currTime = 0.0f;
		//poRefTE = new SourceVoice();
	}

	SourceVoice* getSourceVoice()
	{
		SourceVoice* pNode = (SourceVoice*)this->baseAdd();
		assert(pNode != nullptr);
		pNode->sv->FlushSourceBuffers();
		pNode->pan2Ch(0.0f);
		pNode->setVolume(1.0f);
		return pNode;
	}

	HRESULT returnSourceVoice(SourceVoice& sv)
	{
		sv.stop();
		sv.sv->FlushSourceBuffers();
		sv.pan2Ch(0.0f);
		sv.setVolume(1.0f);
		Remove(sv);
		OutputDebugStringW(L"VoiceReturned Sucessfully\n");
		return 0;
	}


	/*SourceVoice* Find(VoiceTypeID eventID)
	{
	SourceVoiceTypeManager* mrT = SourceVoiceTypeManager::getInstance();
	assert(mrT != nullptr);
	SourceVoice* target = mrT->toFind(eventID);
	return (SourceVoice*)mrT->baseFind(*target);
	}*/

protected:

	void dClearNode(DLink* pLink)
	{
		SourceVoice* p = (SourceVoice*)pLink;
		p->dClean();
	}

	//bad smell I don't have a FIND function defined and I don't want source voices in the pool to be searchable.
	//but base class requires this functionality and this breaks the contract.
	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		assert(false);
		//assert(pLinkA != nullptr);
		//assert(pLinkB != nullptr);

		SourceVoice& left = (SourceVoice&)pLinkA;
		SourceVoice& right = (SourceVoice&)pLinkB;

		if (left.getName() == right.getName())
		{
			return true;
		}
		return false;
	}

	DLink* dCreateNode()
	{
		SourceVoice* newNode = new SourceVoice();
		assert(newNode != nullptr);
		newNode->init(refWFX);
		return newNode;
	}

	////for find
	//TimeEvent* toFind(TimeEventID id)
	//{
	//	poRefTE->Name = id;
	//	return poRefTE;
	//}

};

//this allows the searching of the type managers without creating an extra manager
class SourceVoiceType : public DLink
{
public:

	VoiceTypeID Name;
	SourceVoiceTypeManager* poNodeMan;

	SourceVoiceType()
	{
		Name = VoiceTypeID::error;
		//poNodeMan = new SourceVoiceTypeManager();
	}

	void Set(VoiceTypeID name, WAVEFORMATEX wfx)
	{
		Name = name;
		poNodeMan = new SourceVoiceTypeManager(wfx);
	}

	void dClean()
	{
		Name = VoiceTypeID::error;
	}

	SourceVoice* getSourceVoice()
	{
		SourceVoice* toReturn = poNodeMan->getSourceVoice();
		toReturn->Name = Name;
		return toReturn;
	}

	HRESULT returnSourceVoice(SourceVoice& sv)
	{
		return poNodeMan->returnSourceVoice(sv);
	}
};

class SourceVoiceManager : public baseManager
{

private:
	//static float currTime;
	static SourceVoiceManager* pInstance;
	static SourceVoiceType* poRefTE;
	static CircularData* pOut;

	SourceVoiceManager(int numStart = 5, int deltaAdd = 3)
		: baseManager(deltaAdd)
	{
		generateReserves(numStart);
		poRefTE = new SourceVoiceType();
	}

	//theese just create waveformatex's for voices, it doesn't actually keep the audio loaded, the idea is to get a one second sample of a type of data
	//so I can create all the wavfomatex's filled properly.
	void loadDefaultTypes()
	{
		SourceVoiceManager* pSvMan = SourceVoiceManager::getInstance();
		pSvMan;
		WAVEFORMATEX tmp = { 0 };
		XAUDIO2_BUFFER tmpbuff = { 0 };
		//HANDLE mono16b44100 = getFileHandle("heli.wav");
		//HANDLE stereo16b44100 = getFileHandle("applause.wav");

		//getWFX(mono16b44100, tmp);
		helper::LoadBuffer("heli.wav", tmp, tmpbuff);
		pSvMan->Add(VoiceTypeID::mono16b44100, tmp);

		//getWFX(stereo16b44100, tmp);
		helper::LoadBuffer("applause.wav", tmp, tmpbuff);
		pSvMan->Add(VoiceTypeID::stereo16b44100, tmp);

		helper::LoadBuffer(".\\Demo1\\Oboe_mono.wav", tmp, tmpbuff);
		pSvMan->Add(VoiceTypeID::mono32b48000, tmp);


		helper::LoadBuffer(".\\Demo5\\s32b48000.wav", tmp, tmpbuff);
		pSvMan->Add(VoiceTypeID::stereo32b48000, tmp);
	}

public:

	SourceVoiceManager(const SourceVoiceManager &) = delete;
	const SourceVoiceManager & operator = (SourceVoiceManager &) = delete;

	//add HRESULT or custom error coding.
	static HRESULT getSourceVoice(VoiceTypeID Name, SourceVoice*& out)
	{
		//search active nodes if type isn't initlized create.

		SourceVoiceManager* mrT = SourceVoiceManager::getInstance();
		assert(mrT != nullptr);

		SourceVoiceType* VTN = mrT->Find(Name);
		out = VTN->getSourceVoice();

		return 0;
	}

	static HRESULT returnSourceVoice(SourceVoice& sv)
	{
		SourceVoiceManager* pSvMan = SourceVoiceManager::getInstance();

		SourceVoiceType* pSVTMan = pSvMan->Find(sv.getName());

		pSVTMan->returnSourceVoice(sv);

		return 0;
	}

	static void Create(int numStart = 5, int deltaAdd = 3)
	{
		assert(numStart > 0);
		assert(deltaAdd > 0);
		assert(pInstance == nullptr);

		if (pInstance == nullptr)
		{
			pInstance = new SourceVoiceManager(numStart, deltaAdd);
			pInstance->loadDefaultTypes();
		}
	}

	static SourceVoiceType* Add(VoiceTypeID name, WAVEFORMATEX wfx)
	{
		SourceVoiceManager* pSvMan = SourceVoiceManager::getInstance();
		assert(nullptr != pSvMan);
		SourceVoiceType* pNode = (SourceVoiceType*)pSvMan->baseAdd();
		assert(nullptr != pNode);

		pNode->Set(name, wfx);

		return pNode;
	}

	static SourceVoiceManager* getInstance()
	{
		if (pInstance == nullptr)
		{
			SourceVoiceManager::Create();
		}
		return pInstance;
	}


	SourceVoiceType* Find(VoiceTypeID eventID)
	{
		SourceVoiceManager* mrT = SourceVoiceManager::getInstance();
		assert(mrT != nullptr);
		SourceVoiceType* target = mrT->toFind(eventID);
		return (SourceVoiceType*)mrT->baseFind(*target);
	}

protected:

	void dClearNode(DLink* pLink)
	{
		SourceVoiceType* p = (SourceVoiceType*)pLink;
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

		SourceVoiceType& left = (SourceVoiceType&)pLinkA;
		SourceVoiceType& right = (SourceVoiceType&)pLinkB;

		if (left.Name == right.Name)
		{
			return true;
		}
		return false;
	}

	DLink* dCreateNode()
	{
		DLink* newNode = new SourceVoiceType();
		assert(newNode != nullptr);

		return newNode;
	}

	//for find
	SourceVoiceType* toFind(VoiceTypeID id)
	{
		poRefTE->Name = id;
		return poRefTE;
	}
};

class StopVoiceAndReleaseCMD : public Command
{
	SourceVoice* sv;
public:
	StopVoiceAndReleaseCMD(SourceVoice* sv) : sv(sv)
	{
	}

	void execute(unsigned int)
	{
		SourceVoiceManager::returnSourceVoice(*sv);
	}
};

class ReleaseVoiceCMD : public Command
{
	SourceVoice* sv;
public:
	ReleaseVoiceCMD(SourceVoice* sv) : sv(sv)
	{
	}

	void execute(unsigned int)
	{
		SourceVoiceManager::returnSourceVoice(*sv);
	}
};

class demo4CMD : public Command
{
	SourceVoice* sv;
	char* name;

public:
	demo4CMD(SourceVoice* sv, char* name) : sv(sv), name(name)
	{
	}

	void execute(unsigned int)
	{
		Trace::out("GameCallback %p EndofStream \n", &sv->handle);
		Trace::out("Wave %s ", name);
		Trace::out("   PanValue %f", sv->getPanVal());
		Trace::out("   Duration %u", sv->getPlayTime());

		SourceVoiceManager::returnSourceVoice(*sv);
	}
};

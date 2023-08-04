#include <assert.h>

#include "CircularData.h"
#include "SndManager.h"
#include "LoadManager.h"
#include "Snd.h"


Snd::Snd(SndId snd)
	: sndID(snd),
	handle()
{
}

Snd::Snd(const Snd &tmp)
	: sndID(tmp.sndID),
	handle()
{
}

const Snd &Snd::operator=(const Snd &tmp)
{
	this->sndID = tmp.sndID;

	return *this;
}

Snd::~Snd()
{
	//HANDLE_INVALIDATE(handle);

	// todo: is this it, I think there is more
}


HandleStatus Snd::Play()
{
	HANDLE_CHECK(handle)

		// Send a play command to the Audio thread
		CircularData *pOut = SndManager::GetG2ABuff();
	assert(pOut);

	pOut->PushBack((int)this->sndID);

	HANDLE_CHECK_END
}

HandleStatus Snd::Load()
{
	HANDLE_CHECK(handle)

		CircularData *pOut = LoadManager::GetG2LBuff();
	assert(pOut);

	pOut->PushBack((int)this->sndID);

	HANDLE_CHECK_END
}
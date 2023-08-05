#include <assert.h>
#include "HandleMan.h"

const HandleID invalidHandle = HANDLE_INVALID_STATE;
const HandleID startID = HANDLE_STARTING_ID;

HandleMan::HandleMan()
{
	this->srcID = startID;

	for (int i = 0; i < HANDLE_TABLE_SIZE; i++)
	{
		this->table[i].ID = invalidHandle;
	}
}

HandleMan::~HandleMan()
{
	// safety - nuke it again
	this->srcID = startID;

	for (int i = 0; i < HANDLE_TABLE_SIZE; i++)
	{
		this->table[i].ID = invalidHandle;
	}
}

HandleID HandleMan::privGetNewID()
{
	// Increment
	this->srcID++;

	return this->srcID;
}


HandleStatus HandleMan::InvalidateHandle(Handle &handle)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	HandleStatus status = HandleStatus::HANDLE_ERROR;

	// before nuking it, is it valid?
	status = HandleMan::ValidateHandle(handle);
	assert(status == HandleStatus::SUCCESS);

	// nuke it
	pMan->table[handle.GetIndex()].ID = invalidHandle;

	return status;
}

HandleStatus HandleMan::ValidateHandle(Handle &handle)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	HandleStatus status = HandleStatus::HANDLE_ERROR;

	if (handle.GetIndex() >= 0 && handle.GetIndex() < HANDLE_TABLE_SIZE)
	{
		if (pMan->table[handle.GetIndex()].ID == handle.GetID())
		{
			status = HandleStatus::SUCCESS;
		}
		else
		{
			status = HandleStatus::Invalid_Handle;
		}
	}

	return status;
}

HandleStatus HandleMan::ActivateHandle(HandleID &id, HandleIndex &index)
{
	HandleMan *pMan = HandleMan::privInstance();
	assert(pMan);

	HandleStatus status;

	if (pMan->privFindNextAvailable(index))
	{
		status = HandleStatus::SUCCESS;

		// Set it
		id = pMan->privGetNewID();
		pMan->table[index].ID = id;
	}
	else
	{
		status = HandleStatus::Insuffient_Space;
	}

	return status;
}

bool HandleMan::privFindNextAvailable(HandleIndex &index)
{
	bool status = false;

	for (HandleIndex i = 0; i < HANDLE_TABLE_SIZE; i++)
	{
		if (this->table[i].ID == invalidHandle)
		{
			// found one
			index = i;
			status = true;
			break;
		}
	}

	return status;
}

HandleMan *HandleMan::privInstance()
{
	// Here's the singleton
	static HandleMan instance;

	return &instance;
}

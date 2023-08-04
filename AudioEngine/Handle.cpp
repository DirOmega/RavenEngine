#include <assert.h>
#include "Trace.h"

#include "HandleMan.h"
#include "Handle.h"

Handle::Handle()
{
	//Activate
	HandleStatus status = HandleMan::ActivateHandle(this->ID, this->index);
	assert(status == HandleStatus::SUCCESS);

	Trace::out("Handle(): new handle(%x):0x%X\n", this->index, this->ID);
}


Handle::~Handle()
{
	// Invalidate handle;
	HandleMan::InvalidateHandle(*this);

	Trace::out("~Handle(): invalidate handle(%x):0x%X\n", this->index, this->ID);
}

HandleID Handle::GetID() const
{
	return this->ID;
}

HandleIndex Handle::GetIndex() const
{
	return this->index;
}
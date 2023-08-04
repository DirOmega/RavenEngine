#ifndef HANDLE_H
#define HANDLE_H

// for types
#include "HandleTypes.h"

class Handle
{
public:
	Handle();
	~Handle();

	//-----------------------------------------------------
	// You cannot copy handles
	//-----------------------------------------------------
	Handle(const Handle &) = delete;
	const Handle & operator = (const Handle &) = delete;

	//-----------------------------------------------------
	// You can peek at it, but you can't set it 
	//-----------------------------------------------------
	HandleID GetID() const;
	HandleIndex GetIndex() const;

private:
	//-----------------------------------------------------
	// Data must remain private
	//-----------------------------------------------------
	HandleID	ID;
	HandleIndex	index;
};


#endif

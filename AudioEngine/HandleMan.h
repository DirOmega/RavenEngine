#ifndef HANDLE_MAN_H
#define HANDLE_MAN_H

#include "HandleTable.h"
#include "HandleTypes.h"
#include "HandleStatus.h"
#include "Handle.h"


#define HANDLE_TABLE_SIZE		64
#define HANDLE_INVALID_STATE	0x0
#define HANDLE_STARTING_ID      0xAAAA0000

#define HANDLE_CHECK(handle)	HandleStatus status = HandleMan::ValidateHandle(handle);\
								if (status == HandleStatus::SUCCESS) { 
#define HANDLE_CHECK_END		}return status;

#define HANDLE_INVALIDATE(handle) HandleMan::InvalidateHandle(handle);

class HandleMan
{
public:
	HandleMan();
	~HandleMan();

	//-----------------------------------------------------
	// You cannot copy the singleton
	//-----------------------------------------------------
	HandleMan(const HandleMan &) = delete;
	const HandleMan & operator = (const HandleMan &) = delete;

	//-----------------------------------------------------
	// Being strategic on what is expose or not
	//-----------------------------------------------------
	static HandleStatus ActivateHandle(HandleID &id, HandleIndex &index);
	static HandleStatus InvalidateHandle(Handle &handle);
	static HandleStatus ValidateHandle(Handle &handle);

private:
	//-----------------------------------------------------
	// Keeping internals... private
	//-----------------------------------------------------
	static HandleMan *privInstance();
	HandleID privGetNewID();
	bool privFindNextAvailable(HandleIndex &index);

	//-----------------------------------------------------
	// Raw data
	//-----------------------------------------------------
	HandleTable table[HANDLE_TABLE_SIZE];
	HandleID srcID;
};

#endif

#ifndef HANDLE_STATUS_H
#define HANDLE_STATUS_H


enum class HandleStatus
{
	SUCCESS = 0x7EEE0000,

	Insuffient_Space,
	Invalid_Handle,

	HANDLE_ERROR
};

#endif

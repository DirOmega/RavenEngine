#include <assert.h>

// Windows.h include
// many warnings - need to wrap for Wall warnings
#pragma warning( push )
#pragma warning( disable : 4820 )
#pragma warning( disable : 4668 )
#include <Windows.h>
#pragma warning( pop ) 

#include "File.h"

#define STUB_PLEASE_REPLACE(x) (x);

File::Error File::SearchDirectory(File::Handle& fh, char* fileName, size_t& fileSize, char *& FileName)
{
	File::Error retval = File::Error::SUCCESS;
	WIN32_FIND_DATA data;
	fh = FindFirstFile(fileName, &data);
	if (fh != INVALID_HANDLE_VALUE)
	{
		FileName = data.cFileName;
		fileSize = data.nFileSizeLow;
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			retval = File::Error::NO_FILES_FOUND;
		}
		else
		{
			retval = File::Error::FIND_FAIL;
		}
	}

	return retval;
}

File::Error File::NextFile(File::Handle&fh, size_t& fileSize, char *& FileName)
{
	File::Error retval = File::Error::SUCCESS;
	WIN32_FIND_DATA data;
	if (FindNextFile(fh, &data))
	{
		FileName = data.cFileName;
		fileSize = data.nFileSizeLow;
	}
	else
	{
		if (GetLastError() == ERROR_NO_MORE_FILES)
		{
			retval = File::Error::NO_MORE_FILES;
		}
		else
		{
			retval = File::Error::FIND_FAIL;
		}
	}

	return retval;
}

File::Error File::CloseSearch(File::Handle& fh)
{
	File::Error retval = File::Error::SUCCESS;
	if (!FindClose(fh))
	{
		retval = File::Error::CLOSE_FAIL;
	}
	return retval;
}

File::Error File::Open(File::Handle &fh, const char * const fileName, File::Mode mode)
{

	fh = CreateFile(fileName, mode, 0, 0, 4, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD err = GetLastError();
	File::Error retval = File::Error::SUCCESS;
	if (err != 0 && err != ERROR_ALREADY_EXISTS)
	{
		retval = OPEN_FAIL;
	}

	return retval;
}


File::Error File::Close(File::Handle& fh)
{

	File::Error retval = File::Error::SUCCESS;

	if (!CloseHandle(fh))
	{
		retval = File::Error::CLOSE_FAIL;
		//GetLastError();
	}

	return 	retval;
}


File::Error File::Write(File::Handle& fh, const void * const buffer, const size_t inSize)
{
	LPDWORD out = 0;

	bool ret = WriteFile(fh, buffer, inSize, out, 0);

	File::Error retval = File::Error::SUCCESS;
	if (!ret)
	{
		retval = File::Error::WRITE_FAIL;
		/*if (inSize != *out)
		{
		retval = File::Error::WRITE_FAIL;
		}*/
	}

	return 	retval;
}

File::Error File::Read(File::Handle& fh, void * const buffer, const size_t inSize)
{
	//LPDWORD read; //could check to see that the bytes that were supposed to be read were all read

	File::Error retval = File::Error::SUCCESS;
	//if (!ReadFile(fh, buffer, inSize, read, 0))
	if (!ReadFile(fh, buffer, inSize, 0, 0))
	{
		retval = File::Error::READ_FAIL;
	}

	return 	retval;
}

File::Error File::Seek(File::Handle& fh, File::Location location, int offset)
{
	File::Error retval = File::Error::SUCCESS;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(fh, offset, 0, location))
	{
		retval = File::Error::SEEK_FAIL;
	}

	return 	retval;
}

File::Error File::Tell(File::Handle& fh, unsigned int &offset)
{
	File::Error retval = File::Error::TELL_FAIL;

	offset = SetFilePointer(fh, 0, 0, File::CURRENT);

	if (offset != INVALID_SET_FILE_POINTER)
	{
		retval = File::Error::SUCCESS;
	}

	return 	retval;
}

File::Error File::Flush(File::Handle& fh)
{
	File::Error retval = File::Error::FLUSH_FAIL;

	if (FlushFileBuffers(fh))
	{
		retval = File::Error::SUCCESS;
	}

	return 	retval;
}

// ---  End of File ---------------

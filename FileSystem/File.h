#ifndef FILE_H
#define FILE_H

// Make the assumption of c-char strings, not UNICODE
// 32 bit files, not supporting 64 bits yet

class File
{
public:

	typedef void *Handle;

	enum Mode
	{
		READ = GENERIC_READ,
		WRITE = GENERIC_WRITE,
		READ_WRITE = GENERIC_READ | GENERIC_WRITE
		/*READ = 0x7A000000,
		WRITE,
		READ_WRITE*/
	};

	enum Location
	{
		BEGIN = FILE_BEGIN,
		CURRENT = FILE_CURRENT,
		END = FILE_END
		/*BEGIN = 0x7B000000,
		CURRENT,
		END*/
	};

	enum Error
	{
		SUCCESS = 0x7C000000,
		OPEN_FAIL,
		CLOSE_FAIL,
		WRITE_FAIL,
		READ_FAIL,
		SEEK_FAIL,
		TELL_FAIL,
		FLUSH_FAIL,
		FIND_FAIL,
		NO_MORE_FILES,
		NO_FILES_FOUND
	};

public:

	static File::Error Open(File::Handle &fh, const char * const fileName, File::Mode mode);
	static File::Error Close(File::Handle& fh);
	static File::Error Write(File::Handle& fh, const void * const buffer, const size_t inSize);
	static File::Error Read(File::Handle& fh, void * const _buffer, const size_t _size);
	static File::Error Seek(File::Handle& fh, File::Location location, int offset);
	static File::Error Tell(File::Handle& fh, unsigned int &offset);
	static File::Error Flush(File::Handle& fh);

	static File::Error SearchDirectory(File::Handle& fh, char* fileName, size_t& fileSize, char *& FileName);
	static File::Error NextFile(File::Handle&fh, size_t& fileSize, char *& FileName);
	static File::Error CloseSearch(File::Handle& fh);

};

#endif

// ---  End of File ---------------

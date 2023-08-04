#ifndef XAUDIO2_CHUNK_H
#define XAUDIO2_CHUNK_H

#include "File.h"

enum ChunkError
{
	CHUNK_SUCCESS = 0xC0000000,
	CHUNK_FAIL,
};

ChunkError FindChunk2(File::Handle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
ChunkError ReadChunkData2(File::Handle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);


#endif

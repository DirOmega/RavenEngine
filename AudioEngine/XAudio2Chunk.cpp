#include "XAudio2Wrapper.h"
#include "XAudio2Chunk.h"


// ----------------------------------------------------------------
// Find wave audio data chunk
// ----------------------------------------------------------------

ChunkError FindChunk2(File::Handle hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwOffset = 0;

	// Set file pointer to the beginning
	if (File::Error::SUCCESS != File::Seek(hFile, File::Location::BEGIN, 0))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// loop until you find it
	while (cerror == ChunkError::CHUNK_SUCCESS)
	{
		// Read data Type
		if (File::Error::SUCCESS != File::Read(hFile, &dwChunkType, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		// Read data size
		if (File::Error::SUCCESS != File::Read(hFile, &dwChunkDataSize, sizeof(DWORD)))
		{
			cerror = ChunkError::CHUNK_FAIL;
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;

			// Read data size, special format for RIFF header
			if (File::Error::SUCCESS != File::Read(hFile, &dwFileType, sizeof(DWORD)))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
			break;

		default:
			// Move file pointer to the next Chunk, continue
			if (File::Error::SUCCESS != File::Seek(hFile, File::Location::CURRENT, dwChunkDataSize))
			{
				cerror = ChunkError::CHUNK_FAIL;
			}
		}

		// Since the headers are only 2 words long... 
		// Keep a running total
		dwOffset += sizeof(DWORD) * 2;

		// Found type
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			break;
		}

		dwOffset += dwChunkDataSize;

	}  // while()

	return cerror;
}

// ---------------------------------------------------------------------
// Read wave data chunk, using our FILE system
// ---------------------------------------------------------------------

ChunkError ReadChunkData2(File::Handle hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	ChunkError cerror = ChunkError::CHUNK_SUCCESS;

	// Seek to the offset to chunk
	if (File::Error::SUCCESS != File::Seek(hFile, File::Location::BEGIN, bufferoffset))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	// Read data chunk 
	if (File::Error::SUCCESS != File::Read(hFile, buffer, buffersize))
	{
		cerror = ChunkError::CHUNK_FAIL;
	}

	return cerror;
}

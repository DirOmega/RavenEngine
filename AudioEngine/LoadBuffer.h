#pragma once

#include "File.h"
#include "XAudio2Chunk.h"

namespace helper
{
	static void LoadBuffer(const char *pFileName, WAVEFORMATEX &wfx, XAUDIO2_BUFFER &buffer)
	{
		// -----------------------------------------------------------------
		// Audio: Load Files
		//------------------------------------------------------------------

		HANDLE FileHandle;
		File::Error ferror;

		// Open file

		// "Bass.wav";
		// "LRMonoPhase4.wav";
		const char *srcFile = pFileName;

		ferror = File::Open(FileHandle, srcFile, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		// Set file to beginning
		ferror = File::Seek(FileHandle, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		// -------------------------------------------------------
		// Audio Format - wfx
		// Audio Buffer - how to play it back
		// -------------------------------------------------------
		//WAVEFORMATEX

		//wfx = { 0 };
		//XAUDIO2_BUFFER buffer = { 0 };

		wfx = { 0 };
		buffer = { 0 };


		// -------------------------------------------------------
		// Find and load specific Chunks
		// -------------------------------------------------------

		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		DWORD filetype;
		ChunkError cerror;

		// Scan to the RIFF and load file type
		cerror = FindChunk2(FileHandle, fourccRIFF, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData2(FileHandle, &filetype, sizeof(DWORD), dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Make sure its not in the compressed format, WAVE format is uncompressed
		if (filetype != fourccWAVE)
		{
			assert(false);
		}

		// Read the FMT: format
		cerror = FindChunk2(FileHandle, fourccFMT, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);
		cerror = ReadChunkData2(FileHandle, &wfx, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		// Scan to the DATA chunk, read the size, allocate buffer of that size
		cerror = FindChunk2(FileHandle, fourccDATA, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		BYTE * pDataBuffer = new BYTE[dwChunkSize];
		assert(pDataBuffer != 0);

		// Fill the data...
		cerror = ReadChunkData2(FileHandle, pDataBuffer, dwChunkSize, dwChunkPosition);
		assert(cerror == ChunkError::CHUNK_SUCCESS);

		buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
		buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
		buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		buffer.LoopCount = 0;				  // looping...

		ferror = File::Close(FileHandle);
		assert(ferror == File::Error::SUCCESS);
	};
};
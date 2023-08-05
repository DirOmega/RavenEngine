#include "Trace.h"
#include "File.h"
#include <cstring>

#include "PackageHeader.h"
#include "ChunkHeader.h"
#include "eat.h"

#include "ScratchSpace.h"

//---------------------------------------------------------------------------
// Leave this signature - do your EAT magic here
//---------------------------------------------------------------------------

bool eat(const char * const inFileName,
	ChunkType type,
	const char * const chunkName,
	unsigned char *&chunkBuff,
	unsigned int &chunkSize)
{
	bool retval = false;
	unsigned int offset = 0;
	File::Handle ph;
	File::Open(ph, inFileName, File::Mode::READ);

	printf("offset = %i\n", offset);

	PackageHeader phdr;
	File::Read(ph, &phdr, sizeof(PackageHeader));
	File::Tell(ph, offset);

	printf("offset = %i sizeof phdr = %i\n", offset, sizeof(PackageHeader));

	//File::Seek(ph, File::Location::BEGIN, sizeof(PackageHeader));
	ChunkHeader chdr;

	for (unsigned int i = 0; i < phdr.numChunks; i++)
	{
		File::Read(ph, &chdr, sizeof(ChunkHeader));
		if (chdr.type == type)
		{
			if (strcmp(chdr.chunkName, chunkName) == 0)
			{
				retval = true;
				chunkSize = chdr.chunkSize;
				//chunkBuff = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) unsigned char[chunkSize];
				chunkBuff = new unsigned char[chunkSize];
				File::Read(ph, chunkBuff, chunkSize);
				File::Close(ph);
				//delete chunkBuff;
				break;
			}
			else
			{
				File::Seek(ph, File::Location::CURRENT, (int)chdr.chunkSize);
			}
		}
		else
		{
			File::Seek(ph, File::Location::CURRENT, (int)chdr.chunkSize);
		}
	}

	return retval;
};

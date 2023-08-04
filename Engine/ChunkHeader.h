#ifndef CHUNK_HEADER_H
#define CHUNK_HEADER_H

enum ChunkType
{
	ERROR_TYPE,
	MODEL_TYPE,
	TEXTURE_TYPE,
	VERTS_TYPE = 0xA0000000,
	NORMS_TYPE,
	ANIM_TYPE,
	UV_TYPE
};

struct ChunkHeader
{
	ChunkHeader() {}

	//not for the realtime system
	/*ChunkHeader(char* _chunkType, char* _chunkName, unsigned int _chunkSize, unsigned int _hashNum)
	{

		type = getTypeFromString(_chunkType);

		if (_chunkName != nullptr)
			for (int i = 0; i < CHUNK_NAME_SIZE; i++)
			{
				chunkName[i] = _chunkName[i];
				if (_chunkName[i] == '\0')
				{
					break;
				}
				if (i == CHUNK_NAME_SIZE - 1) { chunkName[i] = '\0'; }
			}

		chunkSize = _chunkSize;
		hashNum = _hashNum;
	}*/

	static const unsigned int CHUNK_NAME_SIZE = 20;

	// data:
	ChunkType      type;
	char           chunkName[CHUNK_NAME_SIZE];
	unsigned int   chunkSize;
	unsigned int   hashNum;

//private:
	//not for the realtime system
	/*ChunkType getTypeFromString(char* chunkType)
	{
		ChunkType retval = ERROR_TYPE;
		if (strcmp(chunkType, "MODEL_TYPE") == 0)
		{
			retval = MODEL_TYPE;
		}
		else if (strcmp(chunkType, "TEXTURE_TYPE") == 0)
		{
			retval = VERTS_TYPE;
		}
		else if (strcmp(chunkType, "NORMS_TYPE") == 0)
		{
			retval = NORMS_TYPE;
		}
		else if (strcmp(chunkType, "ANIM_TYPE") == 0)
		{
			retval = ANIM_TYPE;
		}
		else if (strcmp(chunkType, "VERTS_TYPE") == 0)
		{
			retval = TEXTURE_TYPE;
		}
		else if (strcmp(chunkType, "UV_TYPE") == 0)
		{
			retval = UV_TYPE;
		}
		else
		{
			retval = ERROR_TYPE;
		}
		return retval;
	}*/
};

#endif
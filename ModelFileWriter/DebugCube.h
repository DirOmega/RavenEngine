#pragma once

#include "ModelStructs.h"

class DebugCube
{

	static Vert_xyzuvn vertList[];
	static Tri_index triList[];
	static const char * fn;

public: 

	static const char* getFileName();

	static unsigned int getVertCount();
	static unsigned int getVertArraySize();

	static unsigned int getTriCount();
	static unsigned int getTriArraySize();

	static Vert_xyzuvn* getVertList()
	{
		return vertList;
	}

	static Tri_index* getTriIndexList()
	{
		return triList;
	}

};
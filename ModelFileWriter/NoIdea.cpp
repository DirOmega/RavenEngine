#include "NoIdea.h"


const char* NoIdea::fn = "NoIdea.mdl";

Vert_xyzuvn NoIdea::vertList[] =
{
	{ 0.250000f, 0.250000f, 0.250000f, 0.0f, 0.0f,-0.0000f, 0.9181f, -0.3964f},

	{ 0.000000f, 0.000000f, - 0.328964f, 0.5f, 1.0f, -0.9181f, 0.0000f, - 0.3964f},

	{ -0.250000f, 0.250000f, 0.250000f,1.0f, 0.0f, 0.0000f, -0.9181f, -0.3964f},

	{ 0.250000f, -0.250000f, 0.250000f, 1.0f, 0.0f, 0.9181f, 0.0000f, -0.3964f},

	{ -0.250000f, - 0.250000f, 0.250000f, 0.0f, 0.0f,0.0000f, - 0.0000f, 1.0000f},

	{ 0.250000f, 0.250000f, -0.250000f, 0.0f, 0.0f, 0.9181f, 0.3964f, 0.0000f},

	{ 0.000000f, 0.828964f,  0.000000f, 0.5f, 1.0f, -0.0000f, 0.3964f, 0.9181f},

	{ 0.250000f, 0.250000f, 0.250000f, 1.0f, 0.0f, -0.9181f, 0.3964f, -0.0000f},

	{ -0.250000f, 0.250000f, - 0.250000f, 1.0f, 0.0f, 0.0000f, 0.3964f, - 0.9181f},
	{ -0.250000f, 0.250000f, 0.250000f, 0.0f,0.0f,  0.0000f, -1.0000f, 0.0000f},
	{ 0.250000f, - 0.250000f, - 0.250000f, 0.0f,0.0f, 0.3964f, - 0.9181f, 0.0000f},

	{ 0.828964f, -0.000000f, 0.000000f, 0.5f, 1.0f,  0.3964f, 0.0000f, 0.9181f},
	{ 0.250000f, - 0.250000f, 0.250000f, 1.0f, 0.0f,  0.3964f, 0.9181f, - 0.0000f},
	{ 0.250000f, 0.250000f, -0.250000f,  1.0f, 0.0f, 0.3964f, 0.0000f, -0.9181f},
	{ 0.250000f, 0.250000f, 0.250000f, 0.0f, 0.0f, -1.0000f, 0.0000f, 0.0000f},
	{ -0.250000f,   0.250000f, -0.250000f, 0.0f, 0.0f, -0.3964f, 0.9181f, -0.0000f},
	{ -0.828964f, -0.000000f,   0.000000f, .50f, 1.0f, -0.3964f, 0.0000f, 0.9181f},
	{ -0.250000f,   0.250000f,   0.250000f, 1.0f, 0.0f, -0.3964f, -0.9181f, -0.0000f},
	{ -0.250000f, -0.250000f, -0.250000f, 1.0f, 0.0f, -0.3964f, 0.0000f, -0.9181f},
	{ -0.250000f, -0.250000f,   0.250000f, 0.0f, 0.0f, 1.0000f, 0.0000f, 0.0000f},
	{ -0.250000f, -0.250000f, -0.250000f, .50f, 1.0f, -0.9181f, -0.3964f, 0.0000f},
	{ 0.000000f, -0.828964f,   0.000000f, 1.0f, 0.0f, 0.0000f, -0.3964f, 0.9181f},
	{ - 0.250000f, - 0.250000f,   0.250000f, 1.0f, 0.0f, 0.9181f, - 0.3964f,  0.0000f},
	{ 0.250000f, -   0.250000f, - 0.250000f, 0.0f, 0.0f, 0.0000f, - 0.3964f,- 0.9181f},
	{ 0.250000f,   - 0.250000f,   0.250000f, 0.0f, 0.0f, -0.0000f, 1.0000f,   0.0000f}
	
};

Tri_index NoIdea::triList[] =
{
	{0,1,2},
	{2,1,4},
	{4,1,3},
	{3,1,0},
	{4,0,2},
	{ 5,6,7 },
	{ 7,6,9 },
	{ 9,6,8 },
	{ 8,6,5 },
	{ 7,8,5 },
	{ 10,11,12 },
	{ 12,11,14 },
	{ 14,11,13 },
	{ 13,11,10 },


	{ 14,10,12 },
	{ 15,16,17 },

	{ 17,16,19 },
	{ 19,16,18 },

	{ 18,16,15 },

	{ 19,15,17 },

	{ 20,21,22 },

	{ 22,21,24 },

	{ 24,21,23 },

	{ 23,21,20 },

	{ 24,20,22 },

	{ 4,3,0 },

	{ 7,9,8 },

	{ 14,13,10 },

	{ 19,18,15},
	{24,23,20}

};


unsigned int NoIdea::getVertCount()
{
	return sizeof(vertList) / sizeof(Vert_xyzuvn);
}

unsigned int NoIdea::getTriArraySize()
{
	return sizeof(triList);
}

unsigned int NoIdea::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int NoIdea::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

const char* NoIdea::getFileName()
{
	return fn;
}
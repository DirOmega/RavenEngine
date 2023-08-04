#include "diamond.h"

const char* Diamond::fn = "diamond.mdl";

Vert_xyzuvn Diamond::vertList[] = 
{
	{- 0.250000f, - 0.000000f, - 0.250000f, 0.0f, 0.0f,  - 0.9181f, - 0.3964f, 0.0000f },
	{  0.000000f, - 0.578964f,   0.000000f, 0.5f, 1.0f,    0.0000f, - 0.3964f, 0.9181f },
	{- 0.250000f, - 0.000000f,   0.250000f, 1.0f, 0.0f,    0.9181f, - 0.3964f, 0.0000f },
	{  0.250000f,   0.000000f, - 0.250000f, 1.0f, 0.0f,    0.0000f, -  0.3964f, - 0.9181f },
	{  0.250000f,   0.000000f,   0.250000f, 0.0f, 0.0f,  - 0.0000f,     1.0000f,   0.0000f },

	{ 0.250000f, 0.000000f, -0.250000f, 0.0f, 0.0f, 0.9181f, 0.3964f, 0.0000f},
	{ 0.000000f, 0.578964f, 0.000000f, 0.5f, 1.0f,  0.0000f, 0.3964f, 0.9181f},
	{ 0.250000f, 0.000000f, 0.250000f, 1.0f, 0.0f,-0.9181f, 0.3964f, 0.0000f},

	{-0.250000f, 0.000000f, -0.250000f, 1.0f, 0.0f,  0.0000f, 0.3964f, - 0.9181f,},
	{-0.250000f, 0.000000f,   0.250000f, 0.0f, 0.0f,  0.0000f, - 1.0000f, 0.0000f},


};


Tri_index Diamond::triList[] =
{
	{0,1,2},
	{2,1,4},
	{4,1,3},
	{3,1,0},
	{4,0,2},
	{4,3,0},

	{5, 6, 7},
	{7,6,9},
	{9, 6, 8},
	{8, 6, 5},
	{9, 5, 7},
	{9, 8, 5}

};


unsigned int Diamond::getVertCount()
{
	return sizeof(vertList) / sizeof(Vert_xyzuvn);
}

unsigned int Diamond::getTriArraySize()
{
	return sizeof(triList);
}

unsigned int Diamond::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int Diamond::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

const char* Diamond::getFileName()
{
	return fn;
}
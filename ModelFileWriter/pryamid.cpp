#include "Pryamid.h"

const char* Pryamid::fn = "Pryamid.mdl";

Vert_xyzuvn Pryamid::vertList[] =
{
	{  0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 0.9181f, 0.3964f, 0.0f },
	{  0.0f, 0.328964f, 0.0f, 0.50f, 1.0f, 0.0f, 0.3964f, 0.9181f },
	{  0.25f, -0.25f, 0.25f, 1.0f, 0.0f, -0.9181f, 0.3964f, 0.0f },
	{ -0.25f, -0.25f, -0.25f, 1.0f, 0.0f, -0.0f, 0.3964f, -0.9181f },
	{ -0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f }
};

//I'm pretty sure it exported ccw
Tri_index Pryamid::triList[] =
{
	{ 0,1,2 }, //uv 0 0 , .5 1 , 1 0
	{ 2,1,4 }, //uv 1 0 , .5 1,  1 0
	{ 4,1,3 },
	{ 3,1,0 },
	{ 4,0,2 },
	{ 4,3,0 }

	/*{ 3,2,1 },
	{ 5,2,3 },
	{ 4,2,5 },
	{ 1,2,4 },
	{ 3,1,5 },
	{ 1,4,5 }*/

};

unsigned int Pryamid::getVertCount()
{
	return sizeof(vertList) / sizeof(Vert_xyzuvn);
}

unsigned int Pryamid::getTriArraySize()
{
	return sizeof(triList);
}

unsigned int Pryamid::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int Pryamid::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

const char* Pryamid::getFileName()
{
	return fn;
}
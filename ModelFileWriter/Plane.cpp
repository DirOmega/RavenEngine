#include "Plane.h"

const char* Plane::fn = "Plane.mdl";

Vert_xyzuvn Plane::vertList[] =
{
{ -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
{ 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },
{ -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
{ 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }
};

//I'm pretty sure it exported ccw
Tri_index Plane::triList[] =
{
	{2,3,1},
	{2,4,3}
};

unsigned int Plane::getVertCount()
{
	return sizeof(vertList) / sizeof(Vert_xyzuvn);
}

unsigned int Plane::getTriArraySize()
{
	return sizeof(triList);
}

unsigned int Plane::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int Plane::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

const char* Plane::getFileName()
{
	return fn;
}
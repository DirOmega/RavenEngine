#include "BSphere.h"

const char* BSphere::fn = "BSphere.mdl";

Vert_xyzuvn BSphere::pVerts[121];
Tri_index   BSphere::triList[200];


const char* BSphere::getFileName()
{
	return fn;
}

unsigned int BSphere::getVertCount()
{
	return sizeof(pVerts) / sizeof(Vert_xyzuvn);
}

unsigned int BSphere::getVertArraySize()
{
	return sizeof(pVerts);
}

unsigned int BSphere::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

unsigned int BSphere::getTriArraySize()
{
	return sizeof(triList);
}
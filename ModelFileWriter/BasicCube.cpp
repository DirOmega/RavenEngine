#include "BasicCube.h"

const char* BasicCube::fn = "BasicCube.mdl";

Tri_index BasicCube::triList[] =
{
	{ 0,1,2 },
	{ 2,3,0 },
	{ 4,5,6 },
	{ 5,7,6 },
	{ 8,9,10 },
	{ 9,11,10 },
	{ 12,13,14 },
	{ 13,15,14 },
	{ 16,17,18 },
	{ 18,19,16 },
	{ 20,21,22 },
	{ 22,23,20 }
};

Vert_xyzuvn  BasicCube::vertList[] =
{
	// Triangle 0
	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //0
	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },// 1
	{ 0.25f, -0.25f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //2

																// Triangle 1
	{ 0.25f,   0.25f, -0.25f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f },  //4  3


																  // Triangle 2
	{ 0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f }, //6
	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f }, //7
	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f }, //8

															   // Triangle 3
	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f }, //9 

															   // Triangle 4
	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f },  // 12
	{ -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f },   // 13
	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f },  // 14

																// Triangle 5
	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },  //15

																 // Triangle 6
	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //18
	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //19
	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, //20

																 // Triangle 7
	{ -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // 21

																 // Triangle 8
	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //24
	{ 0.25f, -0.25f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f }, //25
	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //26

																 // Triangle 9
	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // 27

																 // Triangle 10
	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }, //30
	{ 0.25f,  0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f }, //31
	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32

																// Triangle 11
	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f }, //33

};



const char* BasicCube::getFileName()
{
	return fn;
}

unsigned int BasicCube::getVertCount()
{
	return sizeof(vertList) / sizeof(Vert_xyzuvn);
}

unsigned int BasicCube::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int BasicCube::getTriCount()
{
	return sizeof(triList) / sizeof(Tri_index);
}

unsigned int BasicCube::getTriArraySize()
{
	return sizeof(triList);
}
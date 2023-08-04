//#include "CubeModel.h"
//#include <assert.h>
//
////struct Vert_xyzuv
////{
////	float x;
////	float y;
////	float z;
////	float u;
////	float v;
////};
//
//
////struct Vert_xyzuvn
////{
////	float x;
////	float y;
////	float z;
////	float u;
////	float v;
////	float nx;
////	float ny;
////	float nz;
////};
////
////struct Tri_index
////{
////	unsigned int v0;
////	unsigned int v1;
////	unsigned int v2;
////};
////
//
//
////Vert_xyzuv  cubeData[] =
////{
////	// Triangle 0
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f },
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f },
////
////	// Triangle 1
////	{ 0.25f,  -0.25f, -0.25f, 1.0f, 1.0f },
////	{ 0.25f,   0.25f, -0.25f, 1.0f, 0.0f },
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f },
////
////	// Triangle 2
////	{ 0.25f, -0.25f, -0.25f, 0.0f, 1.0f },
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f },
////	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f },
////
////	// Triangle 3
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f },
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f },
////	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f },
////
////	// Triangle 4
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f },
////	{ -0.25f, -0.25f, 0.25f, 0.0f, 1.0f },
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f },
////
////	// Triangle 5
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 1.0f },
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f },
////	{ 0.25f,   0.25f,  0.25f, 1.0f, 0.0f },
////
////	// Triangle 6
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f },
////	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f },
////
////	// Triangle 7
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f },
////	{ -0.25f,  0.25f, -0.25f, 1.0f, 1.0f },
////	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f },
////
////	// Triangle 8
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f },
////	{ 0.25f, -0.25f,   0.25f, 1.0f, 0.0f },
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f },
////
////	// Triangle 9
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f },
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f },
////
////	// Triangle 10
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f },
////	{ 0.25f,  0.25f,  -0.25f, 1.0f, 1.0f },
////	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f },
////
////	// Triangle 11
////	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f },
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f },
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f }
////};
//
////static Vert_xyz cubeData[] =
////{
////	// Triangle 0
////	{-0.25f,  0.25f, -0.25f},
////	{-0.25f, -0.25f, -0.25f},
////	{0.25f, -0.25f, -0.25f},
////	
////	// Triangle 1
////	{0.25f, -0.25f, -0.25f},
////	{0.25f,  0.25f, -0.25f},
////	{-0.25f,  0.25f, -0.25f},
////	
////	// Triangle 2
////	{0.25f, -0.25f, -0.25f},
////	{0.25f, -0.25f,  0.25f},
////	{0.25f,  0.25f, -0.25f},
////	
////	// Triangle 3
////	{0.25f, -0.25f,  0.25f},
////	{0.25f,  0.25f,  0.25f},
////	{0.25f,  0.25f, -0.25f},
////	
////	// Triangle 4
////	{0.25f, -0.25f,  0.25f},
////	{-0.25f, -0.25f,  0.25f},
////	{0.25f,  0.25f,  0.25f},
////	
////	// Triangle 5
////	{-0.25f, -0.25f,  0.25f},
////	{-0.25f,  0.25f,  0.25f},
////	{0.25f,  0.25f,  0.25f},
////	
////	// Triangle 6
////	{-0.25f, -0.25f,  0.25f},
////	{-0.25f, -0.25f, -0.25f},
////	{-0.25f,  0.25f,  0.25f},
////	
////	// Triangle 7
////	{-0.25f, -0.25f, -0.25f},
////	{-0.25f,  0.25f, -0.25f},
////	{-0.25f,  0.25f,  0.25f},
////	
////	// Triangle 8
////	{-0.25f, -0.25f,  0.25f},
////	{0.25f, -0.25f,  0.25f},
////	{0.25f, -0.25f, -0.25f},
////	
////	// Triangle 9
////	{0.25f, -0.25f, -0.25f},
////	{-0.25f, -0.25f, -0.25f},
////	{-0.25f, -0.25f,  0.25f},
////	
////	// Triangle 10
////	{-0.25f,  0.25f, -0.25f},
////	{0.25f,  0.25f, -0.25f},
////	{0.25f,  0.25f,  0.25f},
////	
////	// Triangle 11
////	{0.25f,  0.25f,  0.25f},
////	{-0.25f,  0.25f,  0.25f},
////	{-0.25f,  0.25f, -0.25f}
////};
//
//
////Tri_index triList[] =
////{
////	{ 0,1,2 },
////	{ 3,4,5 },
////	{ 6,7,8 },
////	{ 9,10,11 },
////	{ 12,13,14 },
////	{ 15,16,17 },
////	{ 18,19,20 },
////	{ 21,22,23 },
////	{ 24,25,26 },
////	{ 27,28,29 },
////	{ 30,31,32 },
////	{ 33,34,35 }
////};
////
////Tri_index triList[] =
////{
////	{ 0,1,2 },
////	{ 2,3,0 },//{3,4,5},
////
////	{ 6 - 2,7 - 2,8 - 2 },
////	{ 7 - 2,9 - 2,8 - 2 },
////
////	{ 12 - 4,13 - 4,14 - 4 },
////	{ 13 - 4,15 - 4,14 - 4 },
////
////
////	{ 18 - 6,19 - 6,20 - 6 },
////	{ 19 - 6,21 - 6,20 - 6 },
////
////	{ 24 - 8,25 - 8,26 - 8 },
////	{ 26 - 8,27 - 8,24 - 8 },
////
////
////	{ 30 - 10,31 - 10,32 - 10 },
////	{ 32 - 10,33 - 10,30 - 10 }
////};
//
////Vert_xyzuvn  cubeData[] =
////{
////	// Triangle 0
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f,  -0.6f, -0.6f },
////
////	// Triangle 1
////	{ 0.25f,  -0.25f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f },
////	{ 0.25f,   0.25f, -0.25f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f },
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f },
////
////	// Triangle 2
////	{ 0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f },
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f },
////	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f },
////
////	// Triangle 3
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f },
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f },
////	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f },
////
////	// Triangle 4
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f },
////	{ -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f },
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f },
////
////	// Triangle 5
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f },
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },
////	{ 0.25f,   0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f },
////
////	// Triangle 6
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },
////	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f },
////
////	// Triangle 7
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },
////	{ -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f },
////	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f },
////
////	// Triangle 8
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f },
////	{ 0.25f, -0.25f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f },
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f },
////
////	// Triangle 9
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f },
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f },
////
////	// Triangle 10
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f },
////	{ 0.25f,  0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f },
////	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f },
////
////	// Triangle 11
////	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f },
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f },
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }
////};
//
//
////Tri_index triList[] =
////{
////	{ 0,1,2 },
////	{ 2,3,0 },//{3,4,5},
////
////	{ 6 - 2,7 - 2,8 - 2 },
////	{ 7 - 2,9 - 2,8 - 2 },
////
////	{ 12 - 4,13 - 4,14 - 4 },
////	{ 13 - 4,15 - 4,14 - 4 },
////
////
////	{ 18 - 6,19 - 6,20 - 6 },
////	{ 19 - 6,21 - 6,20 - 6 },
////
////	{ 24 - 8,25 - 8,26 - 8 },
////	{ 26 - 8,27 - 8,24 - 8 },
////
////
////	{ 30 - 10,31 - 10,32 - 10 },
////	{ 32 - 10,33 - 10,30 - 10 }
////};
//
////Vert_xyzuvn  cubeData[] =
////{
////	// Triangle 0
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f }, //0
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f },// 1
////	{ 0.25f, -0.25f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //2
////
////																// Triangle 1
////																//{ 0.25f,  -0.25f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f  }, //3  2
////	{ 0.25f,   0.25f, -0.25f, 1.0f, 0.0f,  0.6f,  0.6f, -0.6f },  //4  3
////																  //{-0.25f,   0.25f, -0.25f, 0.0f, 0.0f, -0.6f,  0.6f, -0.6f  }, //5  0
////
////																  // Triangle 2
////	{ 0.25f, -0.25f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f }, //6
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f }, //7
////	{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f }, //8
////
////															   // Triangle 3
////															   //{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f, 0.6f, -0.6f,  0.6f  },   //7
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f, 0.6f,  0.6f,  0.6f }, //9 
////															   //{ 0.25f,  0.25f, -0.25f, 0.0f, 0.0f, 0.6f,  0.6f, -0.6f  },  //8
////
////															   // Triangle 4
////	{ 0.25f, -0.25f,  0.25f, 1.0f, 1.0f,  0.6f, -0.6f, 0.6f },  // 12
////	{ -0.25f, -0.25f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f },   // 13
////	{ 0.25f,  0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f },  // 14
////
////																// Triangle 5
////																//{ -0.25f, -0.25f,  0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f },  // 13
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f,  0.6f, 0.6f },  //15
////																 //	{ 0.25f,   0.25f,  0.25f, 1.0f, 0.0f,  0.6f,  0.6f, 0.6f }, // 14
////
////																 // Triangle 6
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //18
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, //19
////	{ -0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, //20
////
////																 // Triangle 7
////																 //{-0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // 19
////	{ -0.25f,  0.25f, -0.25f, 1.0f, 1.0f, -0.6f,  0.6f, -0.6f }, // 21
////																 //{-0.25f,  0.25f,  0.25f, 1.0f, 0.0f, -0.6f,  0.6f,  0.6f }, // 20
////
////																 // Triangle 8
////	{ -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f }, //24
////	{ 0.25f, -0.25f,   0.25f, 1.0f, 0.0f,  0.6f, -0.6f,  0.6f }, //25
////	{ 0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f }, //26
////
////																 // Triangle 9
////																 //{0.25f, -0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f}, // 26
////	{ -0.25f, -0.25f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f }, // 27
////																 //{-0.25f, -0.25f,  0.25f, 0.0f, 0.0f, -0.6f, -0.6f,  0.6f}, // 24
////
////																 // Triangle 10
////	{ -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f }, //30
////	{ 0.25f,  0.25f,  -0.25f, 1.0f, 1.0f,  0.6f, 0.6f, -0.6f }, //31
////	{ 0.25f,  0.25f,   0.25f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32
////
////																// Triangle 11
////																//{0.25f,  0.25f,   0.25f, 1.0f, 0.0f,  0.6f, 0.6f,  0.6f }, //32
////	{ -0.25f,  0.25f,  0.25f, 0.0f, 0.0f, -0.6f, 0.6f,  0.6f }, //33
////																//{-0.25f,  0.25f, -0.25f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f } //30
////};
//
//
////#define CUBE_DATA_NUM_VERTS (sizeof(cubeData)/sizeof(Vert_xyz))
//#define CUBE_DATA_NUM_VERTS (sizeof(cubeData)/sizeof(Vert_xyzuvn))
//#define TRILIST_NUM_TRIANGLES (sizeof(triList)/sizeof(Tri_index))
//#define UNUSED_VAR(x) (x);
//
//
//void CubeModel::privCreateVAO(const char * const pModelFileName)
//{
//	assert(pModelFileName);
//	UNUSED_VAR(pModelFileName);
//
//	this->numVerts = CUBE_DATA_NUM_VERTS;
//	this->numTris = TRILIST_NUM_TRIANGLES;
//
//	//Create a VAO
//	glGenVertexArrays(1, &this->vao);
//	assert(this->vao != 0);
//	glBindVertexArray(this->vao);
//
//	//Create a VBO
//	glGenBuffers(2, &this->vbo[0]);
//	assert(this->vbo != 0);
//
//	// Load the combined data
//	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
//	
//	//send all the verts to the GPU
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), cubeData, GL_STATIC_DRAW);
//
//	//define an array of generic vertex attribute data
//	// Vert data is 0
//	//TODO:: make an enum for pointer index so it is garuenteed consistent
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), 0);
//
//	glEnableVertexAttribArray(0);
//
//	void *offsetTex = (void *)((unsigned int)&cubeData[0].u - (unsigned int)cubeData);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
//	glEnableVertexAttribArray(1);
//
//	void *offsetNorm = (void *)((unsigned int)&cubeData[0].nx - (unsigned int)cubeData);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
//	glEnableVertexAttribArray(2);
//
//	 Load tri index data
//
//	//bind 2nd vbo
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
//	//copy tri indexes to buffer
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);
//
//}
//
//CubeModel::CubeModel(const char * const pModelFileName)
//	:Model()
//{
//	assert(pModelFileName);
//	this->privCreateVAO(pModelFileName);
//}
//
//CubeModel::~CubeModel()
//{
//	//delete the vao and vbo here
//	glDeleteVertexArrays(1, &vao);
//
//	glDeleteBuffers(2, &vbo[0]);
//	
//}
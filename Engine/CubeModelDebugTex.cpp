//#pragma once
//#include "CubeModelDebugTex.h"
//#include "File.h"
//#include <assert.h>
//
////struct Vert_xyz
////{
////	float x;
////	float y;
////	float z;
////};
//
////struct Vert_xyzuvn
////{
////	float x;
////	float y;
////	float z;
////	float u;
////	float v;
////};
//
//
////#define CUBE_DATA_NUM_VERTS (sizeof(cubeData)/sizeof(Vert_xyz))
////#define CUBE_DATA_NUM_VERTS (sizeof(cubeData2)/sizeof(Vert2_xyzuv))
////#define UNUSED_VAR(x) (x);
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
////Vert_xyzuvn  cubeData2[] =
////{
////	//8 verts
////	//front verts
////	//{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f} // D I O
////	//{-0.25f, 0.25f, 0.25f, 0.25f, 0.75f } // A H N
////	//{0.25f, -0.25f, 0.25f, 0.50f, 0.50f} // C J T
////	//{-.25f, -.25f, 0.25f, 0.25f, 0.5f} //B G Q
////	//rear verts
////	//{.25f, .25f, -.25f, 0.0f, 0.0f} //P L U
////	//{-.25f, .25f, -.25f, 0.0f,0.0f} //M E X
////	//{.25f , -.25f, -.25f, 0.0f,0.0f} // V K S
////	//{-.25f, -.25f, -.25f, 0.0f,0.0f} //F R W
////
////	//Front face
////	//Triangle 0 = A D C
////	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f, -0.6f, 0.6f, -0.6f },
////	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f, -0.6f, -0.6f, -0.6f },
////	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,-0.6f, -0.6f, -0.6f},
////										 
////	//Triangle 1 = A C B				
////	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f ,-0.6f, -0.6f, -0.6f},
////	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, -0.6f},
////										 
////	//Right face						
////	//Triangle 2 = I L K				
////	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,-0.6f, -0.6f, -0.6f},
////	{ .25f, .25f, -.25f, .75f, 0.75f     ,-0.6f, -0.6f, -0.6f},
////	{ .25f , -.25f, -.25f, .75f,0.50f    ,-0.6f, -0.6f, -0.6f},
////									
////	//Triangle 3 = I K J			
////	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,-0.6f, -0.6f, -0.6f},
////	{ .25f , -.25f, -.25f, .75f,0.50f    ,-0.6f, -0.6f, -0.6f},
////	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,-0.6f, -0.6f, -0.6f},
////										
////	//Left face							
////	//Triangle E H G					
////	{ -.25f, .25f, -.25f, 0.0f,0.75f     ,-0.6f, -0.6f, -0.6f},
////	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, -0.6f},
////	//Triangle E G F					 
////	{-.25f, .25f, -.25f, 0.0f,0.75f      ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, -.25f, 0.0f,0.50f    ,-0.6f, -0.6f, -0.6f},
////									
////	//Top Face						
////	//Triangle M P O					 
////	{ -.25f, .25f, -.25f, 0.25f,1.0f     ,-0.6f, -0.6f, -0.6f},
////	{ .25f, .25f, -.25f, 0.50f, 1.0f     ,-0.6f, -0.6f, -0.6f},
////	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,-0.6f, -0.6f, -0.6f},
////	//Triangle M O N					
////	{ -.25f, .25f, -.25f, 0.25f,1.0f     ,-0.6f, -0.6f, -0.6f},
////	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,-0.6f, -0.6f, -0.6f},
////	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f ,-0.6f, -0.6f, -0.6f},
////										
////	//Bottom Face 						 
////	//Triangle Q T S					
////	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, -0.6f},
////	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,-0.6f, -0.6f, -0.6f},
////	{ .25f , -.25f, -.25f, 0.50f,0.25f   ,-0.6f, -0.6f, -0.6f},
////	//Triangle Q S R					
////	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, -0.6f},
////	{ .25f , -.25f, -.25f, 0.50f,0.25f   ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, -.25f, 0.25f,0.25f   ,-0.6f, -0.6f, -0.6f},
////									
////	//Back Face							
////	//Triangle U X W					
////	{ .25f, .25f, -.25f, 0.75f, 0.75f    ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, .25f, -.25f, 1.0f,0.75f     ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, -.25f, 1.0f,0.50f    ,-0.6f, -0.6f, -0.6f},
////	//Triangle U W V					
////	{.25f, .25f, -.25f, 0.75f, 0.75f     ,-0.6f, -0.6f, -0.6f},
////	{ -.25f, -.25f, -.25f, 1.0f,0.50f    ,-0.6f, -0.6f, -0.6f},
////	{ .25f , -.25f, -.25f, 0.75f,0.50f   ,-0.6f, -0.6f, -0.6f}
////
////};
//
//
//void CubeModelDebugTex::privCreateVAO(const char * const pModelFileName)
//{
//	assert(pModelFileName);
//	/*UNUSED_VAR(pModelFileName);*/
//	File::Handle fh;
//
//	File::Open(fh, pModelFileName, File::Mode::READ);
//
//	int vertstoRead = 0;
//	int triIndextoRead = 0;
//	File::Read(fh, &vertstoRead, sizeof(int));
//	File::Read(fh, &triIndextoRead, sizeof(int));
//	assert(vertstoRead > 0);
//
//	Vert_xyzuvn* cubeData = new Vert_xyzuvn[(unsigned int)vertstoRead];
//	Tri_index* triList = new Tri_index[(unsigned int)triIndextoRead];
//	assert(cubeData != nullptr);
//
//	File::Read(fh , cubeData , vertstoRead    * sizeof(Vert_xyzuvn));
//	File::Read(fh , triList  , triIndextoRead * sizeof(Tri_index));
//
//	File::Close(fh);
//
//	this->numVerts = vertstoRead;
//	this->numTris = vertstoRead / 3;
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
//
//	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);
//
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), cubeData, GL_STATIC_DRAW);
//
//	//define an array of generic vertex attribute data
//	// Vert data is 0
//	// todo make a table or enum
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
//	// Load tri index data
//
//	//bind 2nd vbo
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
//	//copy tri indexes to buffer
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);
//
//}
//
//CubeModelDebugTex::CubeModelDebugTex(const char * const pModelFileName)
//	:Model()
//{
//	assert(pModelFileName);
//	this->privCreateVAO(pModelFileName);
//}
//
//CubeModelDebugTex::~CubeModelDebugTex()
//{
//	//delete the vao and vbo here
//	glDeleteVertexArrays(1, &vao);
//
//	glDeleteBuffers(2, &vbo[0]);
//}
#pragma once
#include "ModelFromFile.h"
#include "File.h"
#include <assert.h>

#include "ChunkHeader.h"
#include "eat.h"

#include "ScratchSpace.h"

void ModelFromFile::privCreateVAO(const char * const pModelFileName)
{
	assert(pModelFileName);
	File::Handle fh;

	File::Open(fh, pModelFileName, File::Mode::READ);

	int vertstoRead = 0;
	int triIndextoRead = 0;
	File::Read(fh, &vertstoRead, sizeof(int));
	File::Read(fh, &triIndextoRead, sizeof(int));
	assert(vertstoRead > 0);

	//Vert_xyzuvn* cubeData = new Vert_xyzuvn[(unsigned int)vertstoRead];
	//Tri_index* triList = new Tri_index[(unsigned int)triIndextoRead];

	//this->modelData = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Vert_xyzuvn[(unsigned int)vertstoRead];
	//this->triList = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Tri_index[(unsigned int)triIndextoRead];\

	this->modelData = new Vert_xyzuvn[(unsigned int)vertstoRead];
	this->triList = new Tri_index[(unsigned int)triIndextoRead];


	assert(modelData != nullptr);
	assert(triList != nullptr);

	File::Read(fh, modelData, vertstoRead * sizeof(Vert_xyzuvn));
	File::Read(fh, triList, triIndextoRead * sizeof(Tri_index));

	File::Close(fh);

	this->numVerts = vertstoRead;
	this->numTris = triIndextoRead;

	//Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	//Create a VBO
	glGenBuffers(2, &this->vbo[0]);
	assert(this->vbo != 0);


	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), modelData, GL_STATIC_DRAW);

	//define an array of generic vertex attribute data
	// Vert data is 0
	// todo make a table or enum
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), 0);

	glEnableVertexAttribArray(0);

	void *offsetTex = (void *)((unsigned int)&modelData[0].u - (unsigned int)modelData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	void *offsetNorm = (void *)((unsigned int)&modelData[0].nx - (unsigned int)modelData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load tri index data

	//bind 2nd vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	//copy tri indexes to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);

}

void ModelFromFile::privCreateVAO(const char * const pPackageFileName, const char * const pModelName)
{
	assert(pPackageFileName);
	assert(pModelName);

	unsigned char* buffer;
	unsigned int bufferSize;
	eat(pPackageFileName, ChunkType::MODEL_TYPE, pModelName, buffer, bufferSize);

	unsigned int vertstoRead = 0;
	unsigned int triIndextoRead = 0;

	unsigned int* cursor = (unsigned int*) buffer;

	vertstoRead = *cursor;
	cursor++;
	triIndextoRead = *cursor;
	cursor++;

	assert(vertstoRead > 0);
	assert(triIndextoRead > 0);

	unsigned char* cursor2 = (unsigned char*)cursor;

	//Vert_xyzuvn* modelData = new Vert_xyzuvn[vertstoRead];

	//this->modelData = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Vert_xyzuvn[vertstoRead];
	this->modelData = new Vert_xyzuvn[vertstoRead];

	assert(modelData != nullptr);

//	this->triList = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Tri_index[triIndextoRead];
	this->triList = new Tri_index[triIndextoRead];
	//Tri_index* triList = new Tri_index[triIndextoRead];

	assert(triList != nullptr);

	unsigned int bytes = vertstoRead * sizeof(Vert_xyzuvn);
	memcpy( modelData, cursor2, bytes );
	cursor2 += bytes;
	bytes = triIndextoRead * sizeof(Tri_index);
	memcpy(triList, cursor2, bytes);

	this->numVerts = (int) vertstoRead;
	this->numTris = (int) triIndextoRead;

	//Create a VAO
	glGenVertexArrays(1, &this->vao);
	assert(this->vao != 0);
	glBindVertexArray(this->vao);

	//Create a VBO
	glGenBuffers(2, &this->vbo[0]);
	assert(this->vbo != 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts),modelData, GL_STATIC_DRAW);

	//define an array of generic vertex attribute data
	// Vert data is 0
	// todo make a table or enum
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), 0);

	glEnableVertexAttribArray(0);

	void *offsetTex = (void *)((unsigned int)&modelData[0].u - (unsigned int)modelData);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	glEnableVertexAttribArray(1);

	void *offsetNorm = (void *)((unsigned int)&modelData[0].nx - (unsigned int)modelData);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	glEnableVertexAttribArray(2);

	// Load tri index data

	//bind 2nd vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	//copy tri indexes to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);
	//TODO: check if I can now delete the model data off main memory cuz I think it's on the GPU now entirely

}

ModelFromFile::ModelFromFile(const char * const pPackageName, const char * const pFileName) 
	: Model()
{
	assert(pPackageName);
	assert(pFileName);
	this->privCreateVAO(pPackageName, pFileName);

}

ModelFromFile::ModelFromFile(const char * const pModelFileName)
	: Model()
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}

ModelFromFile::~ModelFromFile()
{

	//delete the vao and vbo here
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(2, &vbo[0]);

	delete this->modelData;
	delete this->triList;
}
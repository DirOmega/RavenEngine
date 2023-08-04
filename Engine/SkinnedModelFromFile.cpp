#pragma once
#include "SkinnedModelFromFile.h"
#include "File.h"
#include <assert.h>

#include "ChunkHeader.h"
#include "eat.h"
#include "Trace.h"
#include "ScratchSpace.h"


void SkinnedModelFromFile::privCreateVAO(const char * const pModelFileName)
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
	this->triList =   new Tri_index[(unsigned int)triIndextoRead];

	//TODO::fix this, costly extra memory allocation and copy because the converter isn't setup to play nice with rigid bodies and skins
	//this is temporary till I separate the weights in the converter

	Vert_xyzuvnw* temp = new Vert_xyzuvnw[(unsigned int)vertstoRead]; 
	this->boneWeights  = new  Vert_Weights[(unsigned int)vertstoRead];
	this->boneIndices  = new  Bone_Index[(unsigned int)vertstoRead];

	assert(modelData != nullptr);
	assert(triList != nullptr);

	File::Read(fh, temp, vertstoRead * sizeof(Vert_xyzuvnw));
	File::Read(fh, boneIndices, vertstoRead * sizeof(Bone_Index));
	File::Read(fh, triList, triIndextoRead * sizeof(Tri_index));

	////HACK to fix the stuff
	//for (int i = 0; i < vertstoRead; i++)
	//{
	//	bool replaced = false;
	//	//LegD_R to Leg_R
	//	if (boneIndices[i].w0BoneIndex == 135)
	//	{
	//		boneIndices[i].w0BoneIndex = 133;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w1BoneIndex == 135)
	//	{
	//		boneIndices[i].w1BoneIndex = 133;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w2BoneIndex == 135)
	//	{
	//		boneIndices[i].w2BoneIndex = 133;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w3BoneIndex == 135)
	//	{
	//		boneIndices[i].w3BoneIndex = 133;
	//		replaced = true;
	//	}

	//	//KneeD_R to Knee_R
	//	if (boneIndices[i].w0BoneIndex == 136)
	//	{
	//		boneIndices[i].w0BoneIndex = 131;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w1BoneIndex == 136)
	//	{
	//		boneIndices[i].w1BoneIndex = 131;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w2BoneIndex == 136)
	//	{
	//		boneIndices[i].w2BoneIndex = 131;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w3BoneIndex == 136)
	//	{
	//		boneIndices[i].w3BoneIndex = 131;
	//		replaced = true;
	//	}


	//	//AnkleD_R to Ankle_R
	//	/*if (boneIndices[i].w0BoneIndex == 137)
	//	{
	//		boneIndices[i].w0BoneIndex = 130;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w1BoneIndex == 137)
	//	{
	//		boneIndices[i].w1BoneIndex = 130;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w2BoneIndex == 137)
	//	{
	//		boneIndices[i].w2BoneIndex = 130;
	//		replaced = true;
	//	}
	//	if (boneIndices[i].w3BoneIndex == 137)
	//	{
	//		boneIndices[i].w3BoneIndex = 130;
	//		replaced = true;
	//	}*/

	//	if (replaced)
	//	{
	//		Trace::out("ModelReplacing weight\n");
	//	}
	//}



	File::Close(fh);

	this->numVerts = vertstoRead;
	this->numTris = triIndextoRead;


	//TODO: FIX
	//temporary hack to separate weights form vert data need to push this into the converter to reduce load times
	//or don't separate the vert data
	for (int i = 0; i < vertstoRead; i++)
	{
		modelData[i].x = temp[i].x;
		modelData[i].y = temp[i].y;
		modelData[i].z = temp[i].z;

		modelData[i].u = temp[i].u;
		modelData[i].v = temp[i].v;

		modelData[i].nx = temp[i].nx;
		modelData[i].ny = temp[i].ny;
		modelData[i].nz = temp[i].nz;

		boneWeights[i].w0 = temp[i].w0;
		boneWeights[i].w1 = temp[i].w1;
		boneWeights[i].w2 = temp[i].w2;
		boneWeights[i].w3 = temp[i].w3;
	}

	delete[] temp;

	//Trace::out("Skinned Model data = \n");
	//for (int i = 0; i < numVerts; i++)
	//{
	//	Trace::out("Vert %i x y z = %f, %f, %f \n", i, modelData[i].x,modelData[i].y,modelData[i].z);
	//	//Trace::out("\tUV = %f, %f\n", modelData[i].u, modelData[i].v);
	//	//Trace::out("\tN = %f, %f, %f\n", modelData[i].nx, modelData[i].ny, modelData[i].nz);
	//	Trace::out("\t W = %f, %f, %f, %f Wtotal = %f", boneWeights[i].w0, boneWeights[i].w1, boneWeights[i].w2, boneWeights[i].w3, (boneWeights[i].w0 + boneWeights[i].w1 + boneWeights[i].w2 + boneWeights[i].w3));
	//	Trace::out("\t from bones %i , %i , %i , %i", boneIndices[i].w0BoneIndex, boneIndices[i].w1BoneIndex, boneIndices[i].w1BoneIndex, boneIndices[i].w2BoneIndex);
	//	Trace::out("\n");

	//}
	//Trace::out("");

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

	//skin data
	//Create vbo for bone weights
	glGenBuffers(1, &this->vWi);//bone weights handle
	assert(this->vWi != 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->vWi);

	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_Weights)*this->numVerts), boneWeights, GL_STATIC_DRAW);

	//todo rename bone weights if I swap between vert weights and bone weights
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &this->bIi);
	assert(this->bIi != 0);

	glBindBuffer(GL_ARRAY_BUFFER, this->bIi);

	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_Index)*this->numVerts), boneIndices, GL_STATIC_DRAW);

	glVertexAttribIPointer(4, 4, GL_INT, 0, 0);
	glEnableVertexAttribArray(4);

	//TODO: refactor out of this model and into the animation controller, the model shouldn't be responsible for managing animation data
	//bind buffers for bind pose array and bone matrix array

	/*bindArray = new Matrix[256];
	boneArray = new Matrix[256];


	glGenBuffers(1, &bindBuffer);
	
	glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
	glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 256), bindArray, GL_STATIC_DRAW);

	glGenBuffers(1, &boneBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 256, boneArray, GL_STATIC_DRAW);
*/

}

#define UNUSED_VAR(x) (x)
void SkinnedModelFromFile::privCreateVAO(const char * const pPackageFileName, const char * const pModelName)
{
	assert(pPackageFileName);
	assert(pModelName);
	UNUSED_VAR(pPackageFileName);
	UNUSED_VAR(pModelName);

	//TODO:: implement this later, need to fix .spu creater to include theese.
	assert(false);

	//unsigned char* buffer;
	//unsigned int bufferSize;
	//eat(pPackageFileName, ChunkType::MODEL_TYPE, pModelName, buffer, bufferSize);

	//unsigned int vertstoRead = 0;
	//unsigned int triIndextoRead = 0;

	//unsigned int* cursor = (unsigned int*)buffer;

	//vertstoRead = *cursor;
	//cursor++;
	//triIndextoRead = *cursor;
	//cursor++;

	//assert(vertstoRead > 0);
	//assert(triIndextoRead > 0);

	//unsigned char* cursor2 = (unsigned char*)cursor;

	////Vert_xyzuvn* modelData = new Vert_xyzuvn[vertstoRead];

	////this->modelData = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Vert_xyzuvn[vertstoRead];
	//this->modelData = new Vert_xyzuvn[vertstoRead];

	//assert(modelData != nullptr);

	//this->triList = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) Tri_index[triIndextoRead];
	////Tri_index* triList = new Tri_index[triIndextoRead];

	//assert(triList != nullptr);

	//unsigned int bytes = vertstoRead * sizeof(Vert_xyzuvn);
	//memcpy(modelData, cursor2, bytes);
	//cursor2 += bytes;
	//bytes = triIndextoRead * sizeof(Tri_index);
	//memcpy(triList, cursor2, bytes);

	//this->numVerts = (int)vertstoRead;
	//this->numTris = (int)triIndextoRead;

	////Create a VAO
	//glGenVertexArrays(1, &this->vao);
	//assert(this->vao != 0);
	//glBindVertexArray(this->vao);

	////Create a VBO
	//glGenBuffers(2, &this->vbo[0]);
	//assert(this->vbo != 0);

	//glBindBuffer(GL_ARRAY_BUFFER, this->vbo[0]);

	//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), modelData, GL_STATIC_DRAW);

	////define an array of generic vertex attribute data
	//// Vert data is 0
	//// todo make a table or enum
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), 0);

	//glEnableVertexAttribArray(0);

	//void *offsetTex = (void *)((unsigned int)&modelData[0].u - (unsigned int)modelData);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
	//glEnableVertexAttribArray(1);

	//void *offsetNorm = (void *)((unsigned int)&modelData[0].nx - (unsigned int)modelData);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
	//glEnableVertexAttribArray(2);

	//// Load tri index data

	////bind 2nd vbo
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	////copy tri indexes to buffer
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), triList, GL_STATIC_DRAW);
	////TODO: check if I can now delete the model data off main memory cuz I think it's on the GPU now entirely

}

SkinnedModelFromFile::SkinnedModelFromFile(const char * const pPackageName, const char * const pFileName)
	: Model()
{
	assert(pPackageName);
	assert(pFileName);
	this->privCreateVAO(pPackageName, pFileName);
}

SkinnedModelFromFile::SkinnedModelFromFile(const char * const pModelFileName)
	: Model()
{
	assert(pModelFileName);
	this->privCreateVAO(pModelFileName);
}

SkinnedModelFromFile::~SkinnedModelFromFile()
{

	//delete the vao and vbo here
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(2, &vbo[0]);

	glDeleteBuffers(1, &vWi);
	glDeleteBuffers(1, &bIi);

	//if (numShapes > 0)
	//{
	//	glDeleteBuffers(numShapes, &vboShapes[0]);
	//}
	

//	glDeleteBuffers(1, &boneBuffer);
//	glDeleteBuffers(1, &bindBuffer);

	delete this->modelData;
	delete this->triList;

	delete this->boneWeights;
	delete this->boneIndices;

//	delete[] this->bindArray;
//	delete[] this->boneArray;
}
#pragma once

#include "GLibSetup.h"
#include "MathEngine.h"

struct Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Vert_xyz
{
	float x;
	float y;
	float z;

	Vert_xyz& operator = (const Vert_xyzuvn& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	bool operator != (const Vert_xyz& rhs)
	{
		bool retval = false;

		if (Util::isNotEqual(x, rhs.x, MATH_TOLERANCE))
		{
			if (Util::isNotEqual(y, rhs.y, MATH_TOLERANCE))
			{
				if (Util::isNotEqual(z, rhs.z, MATH_TOLERANCE))
				{
					retval = true;
				}
			}
		}

		return retval;
	}

};

struct Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};

struct Vert_xyzuvnw
{

	float x;
	float y;
	float z;

	//split out of here cuz some objects may have multiple uv's
	float u;
	float v;

	//split out of here 
	float nx;
	float ny;
	float nz;

	//todo: move out of here, rigid objects wont have theese.
	float w0;
	float w1;
	float w2;
	float w3;

};

struct Vert_Weights
{
	float w0;
	float w1;
	float w2;
	float w3;
};

struct Bone_Index
{

public:

	Bone_Index() : w0BoneIndex(0), w1BoneIndex(0), w2BoneIndex(0), w3BoneIndex(0)
	{
	}

	int w0BoneIndex;
	int w1BoneIndex;
	int w2BoneIndex;
	int w3BoneIndex;

};

class Model
{
public:

	Model();

	Model(const Model &) = delete;
	Model& operator=(const Model &) = delete;

	virtual ~Model();

	int numVerts;
	int numTris;
	//int numShapes;

	//todo: theese are just for debugging and are copies of what is stored on the gpu already so they can be removed.
	Vert_xyzuvn* modelData;
	Tri_index*   triList;

	GLuint vao;
	GLuint vbo[2];

	//TODO:: move shapes out of base model
	GLuint vboComputeResult;
	//int numShapes;
	//GLuint vboShapes[16];


protected:

	virtual void privCreateVAO(const char * const modelFileName) = 0;
	virtual void privCreateVAO(const char * const pPackageFileName, const char * const pModelName) = 0;

private:
	
};

////basically model is a mesh and I wanna refactor model to be mesh and then have composite model be just model...but babysteps
////TODO:: undo the STL it's awful but I am just testing currently.
//class CompositeModel
//{
//
//	CompositeModel()
//	{
//
//	}
//
//	~CompositeModel()
//	{
//
//	}
//
//private:
//	std::vector<Model> subModels;
//
//};
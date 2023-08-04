#pragma once

#include "Anim.h"
#include "FrameBucket.h"
#include "baseManager.h"
#include "MathEngine.h"
#include "ScratchSpace.h"
#include "PCSTreeForwardIterator.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "GameObjectRigid.h"

//TODO:: put all the types of graphics objects into a header wrapper so I can include them all easier.
#include "GraphicsObject_SkinFT.h"
#include "GraphicsObjectSkinLight.h"
#include "GraphicsObject_DoubleSidedTexture.h"
#include "GraphicsObject_SkinShapeDefo.h"
#include "GraphicsObject_SkinShapeDefoLit.h"


#include "ComputeShaderObject.h"
#include "ComputeShaderTest.h"
#include "ShaderObject.h"

//TODO:: Remove this gotta get rid of STL
//#pragma warning(push)
//#pragma warning( disable: 4820)
//
//#include <vector>
//
//#pragma warning(pop)
//class Pose
//{
//	unsigned int numBones;
//	Vect* TArray;
//	Vect* SArray;
//};

#define BONE_WIDTH 1.0f

class Skelly
{
public:

	unsigned int numBones;
	GameObject* pHeriarchyRoot;

};

struct ShapeFrame_Bucket
{
	ShapeFrame_Bucket *nextBucket;
	ShapeFrame_Bucket *prevBucket;
	Time		  KeyTime;
	float		  pWeights[16];
	//char		  pad[4];

	ShapeFrame_Bucket() : nextBucket(0), prevBucket(0), pWeights{ 0 }
	{}

	~ShapeFrame_Bucket()
	{}
};

struct ComputeHeader
{
	unsigned int numVerts;
	unsigned int numShapes;
};


class Vert_xyzw
{
public:	
	float x;
	float y;
	float z;
	float w;

	Vert_xyzw& operator = (const Vert_xyz& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0f;
		return *this;
	}

	Vert_xyzw& operator = (const Vert_xyzuvn& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0f;
		return *this;
	}

	bool operator != (const Vert_xyzw& rhs)
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

class SSBOManager
{

};

//TODO:: make sure none of this leaks
class ComputeShaderDefoMixer
{
public:

	ComputeShaderDefoMixer(unsigned int numShapes, unsigned int numVerts, Model* pMdl)
	{
		counter = 0;
		pObj = new ComputeShaderObject(ShaderObject::ShaderName::ShapeDefoCompute, "ComputeDefo");

		//Vert_xyz* basisArray = new Vert_xyz[numVerts];
		basisArray = new Vert_xyzw[numVerts];

		for (unsigned int i = 0; i < numVerts; i++)
		{
			basisArray[i] = pMdl->modelData[i];
		}

		NumShapes = numShapes;
		NumVerts = numVerts;

		//glGenBuffers(1, &computeShaderBuffer);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShaderBuffer);
		glUseProgram(pObj->getProgramObject());
		//	NumVerts = numVerts * 2;

		glGenBuffers(1, &ssboBasis);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboBasis);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyzw)*(int)NumVerts, basisArray, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboBasis);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		//glGenBuffers(1, &ssboWeights);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);

		ComputeHeader tData;
		tData.numShapes = NumShapes;
		tData.numVerts = NumVerts;

		glGenBuffers(1, &ssboData);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboData);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(ComputeHeader), &tData, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboData);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		//Vert_xyz* testArray = new Vert_xyz[numVerts];

		glGenBuffers(1, &ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)NumVerts, NULL, GL_STATIC_DRAW);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyzw)* (int)NumVerts, NULL, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		//delete[] testArray;
		//delete[] basisArray;

		ShapeWeights = new float[NumShapes] {0.0f};

		//glGenBuffers(1, &ssboWeights);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)  sizeof(float)*(int)NumShapes, NULL, GL_STATIC_DRAW);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboWeights);

		


		//for (unsigned int i = 0; i < NumShapes; i++)
		//{
		//	ShapeWeights[i] = 2.0f;
		//}

		glGenBuffers(1, &uniformWeights);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformWeights);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, uniformWeights);
		glBufferData(GL_UNIFORM_BUFFER, (GLsizeiptr)sizeof(float)*(int)numShapes, ShapeWeights, GL_STATIC_DRAW);

		UniformWeightsLoc = pObj->GetLocation("test");
		glUniform1fv(UniformWeightsLoc, (GLsizeiptr) sizeof(float)*(int)NumShapes, ShapeWeights);

		glGenBuffers(1, &ssboShapes);

		//glUniform1f(UniformWeightsLoc, ShapeWeights[10]);
		//glUinformMatrix4(UniformWeightsLoc, 1, GL_FALSE, ()

		//UniformWeightsLoc = pObj->GetBufferLocation("test");
		//glUniformBlockBinding(pObj->getProgramObject(), 3, 1 );
		
		/*
		pObj = new ComputeShaderObject(ShaderObject::ShaderName::ShapeDefoCompute, "ComputeDefo");

		Vert_xyz* basisArray = new Vert_xyz[numVerts];

		for (unsigned int i = 0; i < numVerts; i++)
		{
			basisArray[i] = pMdl->modelData[i];
		}

		//glGenBuffers(1, &computeShaderBuffer);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShaderBuffer);

		glUseProgram(pObj->getProgramObject());
		NumVerts = numVerts;

		ComputeHeader tData;
		tData.numShapes = numShapes;
		tData.numVerts = numVerts;

		Vert_xyz* testArray = new Vert_xyz[numVerts];

		glGenBuffers(1, &ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, testArray, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


		glGenBuffers(1, &ssboData);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboData);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(ComputeHeader), &tData, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboData);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


		//glGenBuffers(1, &ssboWeights);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numShapes, ShapeWeights, GL_STATIC_DRAW);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboWeights);


		glGenBuffers(1, &ssboBasis);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboBasis);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, basisArray, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboBasis);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		//glGenBuffers(16, &ssboShapes[0]);

		delete[] basisArray;
		delete[] testArray;
		/*glGenBuffers(1, &countBuffers);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 8, countBuffers);
		glBufferStorage(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_MAP_READ_BIT);
		GLuint zero = 0;
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);*/

	}

	void loadShapes(File::Handle fh, int numShapes, int numVerts)
	{

		//	glUseProgram(pObj->getProgramObject());
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboShapes[bindNumber]);
		//	glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)NumVerts, shape, GL_STATIC_DRAW);
		//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)4 + bindNumber, ssboShapes[bindNumber]);
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, (GLuint)0);

		//NumShapes = (unsigned int)numShapes;

		Vert_xyz* CombinedShapeBuff =  new Vert_xyz [(unsigned int) numVerts*numShapes];
		Vert_xyzw* fixedCombinedBuff = new Vert_xyzw[(unsigned int) numVerts*numShapes];
		//for (int readShape = 0; readShape < numShapes; readShape++)
		//{
			File::Read(fh, CombinedShapeBuff, sizeof(Vert_xyz) * numVerts * numShapes);

			for (unsigned int i = 0; i < NumVerts * numShapes; i++)
			{
				fixedCombinedBuff[i] = CombinedShapeBuff[i];
			}

			BindShapes(fixedCombinedBuff);
		//}
			delete[] CombinedShapeBuff;
			delete[] fixedCombinedBuff;

			//confirmShapeDifference(CombinedShapeBuff, numShapes, numVerts);
	}

	void confirmShapeDifference(Vert_xyzw* CombinedShapeBuff,int numShapes,int numVerts)
	{
		Trace::out("Testing for shape differences\n");

		for (int shape = 1; shape < numShapes+1; shape++)
		{
			Trace::out("Shape %i\n", shape);
			for(int v = 0; v < numVerts; v++)
			{
				if (basisArray[v] != CombinedShapeBuff[v*shape])
				{
					Trace::out("Vert %i differs %f %f %f vs %f %f %f\n", v, basisArray[v].x, basisArray[v].y, basisArray[v].z, CombinedShapeBuff[v].x, CombinedShapeBuff[v].y, CombinedShapeBuff[v].z);
				}

			}
		}
	}

	~ComputeShaderDefoMixer()
	{
		delete pObj;
		delete[] basisArray;
		glDeleteBuffers(1, &ssboBasis);
		glDeleteBuffers(1, &ssboOut);
		//glDeleteBuffers(1, &ssboWeights);
		glDeleteBuffers(1, &uniformWeights);
		glDeleteBuffers(1, &ssboData);
		glDeleteBuffers(1, &ssboShapes);
	}

	//void BindShape(int bindNumber, Vert_xyz* shape)
	//{
	//	glUseProgram(pObj->getProgramObject());
	//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboShapes[bindNumber]);
	//	glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)NumVerts, shape, GL_STATIC_DRAW);
	//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)4 + bindNumber, ssboShapes[bindNumber]);
	//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, (GLuint)0);
	//}

	void Dispatch()
	{
		counter++;
	

		//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 16, ShapeWeights, GL_DYNAMIC_DRAW);

	/*	Trace::out("counter %i Weights in dispatch ",counter);
		for (unsigned int i = 0; i < NumShapes; i++)
		{
			Trace::out(" %f ", ShapeWeights[i]);
		}
		Trace::out("\n");*/

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		//GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, &shader_data, sizeof(shader_data))
		//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glUseProgram(pObj->getProgramObject());

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssboBasis);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboData);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboData);
	
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboOut);
		
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboShapes);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssboShapes);

		//ShapeWeights = new float[NumShapes] { 5.0f };
		//glDeleteBuffers(1, &ssboWeights);
		//glGenBuffers(1, &ssboWeights);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)  sizeof(float)*(int)NumShapes, ShapeWeights, GL_STATIC_DRAW);
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboWeights);

		//glGenBuffers(1, &ssboShapes);

		
		//GLuint block_index = 0;
		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "bD");
		//GLuint ssbo_binding_point_index = 2;
		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, ssbo_binding_point_index);


		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		////GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);//GL_MAP_WRITE_BIT
		//GLvoid* p = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (int)NumShapes * (int)sizeof(float), GL_MAP_WRITE_BIT);
		//memcpy(p, ShapeWeights, sizeof(float)*NumShapes);
		//glFlushMappedBufferRange(GL_SHADER_STORAGE_BUFFER, 0, (int)NumShapes * (int)sizeof(float));
		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		//if (counter == 1000)
		//{
		//	delete[] ShapeWeights;
		//	ShapeWeights = new float[NumShapes] {5.0f};
		//}

		//glGenBuffers(1, &ssboWeights);
		///glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		///
		///glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)  sizeof(float)*(int)NumShapes, ShapeWeights, GL_STATIC_DRAW);
		///glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboWeights);
		

	//	UniformWeightsLoc = pObj->GetLocation("test");

		glBindBuffer(GL_UNIFORM_BUFFER, uniformWeights);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, uniformWeights);
		glUniform1fv(UniformWeightsLoc, (GLsizeiptr) sizeof(float)*(int)NumShapes, ShapeWeights);
		//glUniform1fv(UniformWeightsLoc, (GLsizeiptr) sizeof(float)*(int)NumShapes, ShapeWeights);

		//glUniform1fv((int) uniformWeights, (GLsizeiptr) (sizeof(float) * (int)NumShapes), ShapeWeights);

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboWeights);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(sizeof(float) * NumShapes), ShapeWeights, GL_DYNAMIC_DRAW);
		
		glDispatchCompute((GLuint)NumVerts, 1, 1); //32 is from the compute shader for work sze
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
		//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
	//	glMemoryBarrier(GL_ALL_BARRIER_BITS);

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, countBuffers);
		//GLuint *ptr = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		//GLuint vertCount = ptr[0];
		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		//Trace::out("VertCount = %i", vertCount);

	}


	//void BindOutputVBO(GLuint outputVBO)
	//{
	//	glBindBufferBase(GL_UNIFORM_BUFFER, 0, outputVBO);
	//}

	Vert_xyzw * basisArray;
	unsigned int NumShapes;
	unsigned int NumVerts;
	float *ShapeWeights;
	ComputeShaderObject* pObj;
	GLuint ssboOut;
	//GLuint countBuffers;
	//GLuint ssboWeights;
	GLuint uniformWeights;
	GLint UniformWeightsLoc;
	GLuint ssboData;
	GLuint ssboBasis;
	GLuint ssboShapes;
	int counter;

private:

	void BindShapes(Vert_xyzw* allShapesBuffer)
	{
		glUseProgram(pObj->getProgramObject());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboShapes);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyzw)*(int)NumVerts*(int)NumShapes, allShapesBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)4, ssboShapes);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, (GLuint)0);
	}

};

class ShapeClip : public DLink
{
public:

	~ShapeClip()
	{
		dClean();
	}

	//unsigned int clipID;
	//GraphicsObject_SkinShapeDefo* target;
	bool pad[4];
	Time maxTime;
	int numShapes;
	ShapeFrame_Bucket* pHead;
	ComputeShaderDefoMixer* pMixerProgram;

	bool pad2[4];

	void dClean()
	{
		//clipID = 0;
		maxTime = Time(Time::ZERO);

		if (pHead != 0)
		{
			ShapeFrame_Bucket* tmp = pHead->nextBucket;
			while (pHead != 0)
			{
				tmp = pHead->nextBucket;
				delete pHead;
				pHead = tmp;
			}
		}
		//delete pHead; 
	}

};

class Clip : public DLink
{

public:

	~Clip()
	{
		dClean();
	}

	//unsigned int clipID;
	bool pad[4];
	Time maxTime;
	Frame_Bucket* pHead;
	ShapeClip *pShapeHead;
	//ShapeDeformerList* pShapeHead;
	//bool pad2[4];

	void addShapeClip(ShapeClip* pClipToAdd)
	{
		addToFront((DLink*&)pShapeHead, pClipToAdd);
	}

	void dClean()
	{
		//clipID = 0;
		maxTime = Time(Time::ZERO);

		if (pHead != 0)
		{
			Frame_Bucket* tmp = pHead->nextBucket;
			while (pHead != 0)
			{
				tmp = pHead->nextBucket;
				delete pHead;
				pHead = tmp;
			}
		}
		//delete pHead; 
	}
};

//class ShapeDeformerList : DLink
//{
//public:
//	
//	int numDeformers;
//
//	~ShapeDeformerList()
//	{
//		dClean();
//	}
//
//	void dClean()
//	{
//		//clipID = 0;
//		numDeformers = 0;
//
//		if (pHead != 0)
//		{
//			ShapeFrame_Bucket* tmp = pHead->nextBucket;
//			while (pHead != 0)
//			{
//				tmp = pHead->nextBucket;
//				delete pHead;
//				pHead = tmp;
//			}
//		}
//		//delete pHead; 
//	}
//
//
//};


//class ComputeShaderTest
//{
//public:
//
//	ComputeShaderTest(unsigned int numVerts, Model* pMdl)
//	{
//
//		pObj = new ComputeShaderObject(ShaderObject::ShaderName::ShapeDefoCompute, "ComputeShader");
//
//		Vert_xyz* basisArray = new Vert_xyz[numVerts];
//
//		for (unsigned int i = 0; i < numVerts; i++)
//		{
//			basisArray[i] = pMdl->modelData[i];
//		}
//
//		//glGenBuffers(1, &computeShaderBuffer);
//		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
//		//glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);
//		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShaderBuffer);
//		glUseProgram(pObj->getProgramObject());
//		storedNumVerts = numVerts * 2;
//
//		glGenBuffers(1, &ssboIn);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIn);
//		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, basisArray, GL_STATIC_DRAW);
//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboIn);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//		glGenBuffers(1, &ssboNumVerts);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboNumVerts);
//		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(unsigned int), &storedNumVerts, GL_STATIC_DRAW);
//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboNumVerts);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//		Vert_xyz* testArray = new Vert_xyz[numVerts];
//
//		glGenBuffers(1, &ssboOut);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
//		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, testArray, GL_STATIC_DRAW);
//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboOut);
//		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//		delete[] testArray;
//		//glGenBuffers(1, &atomicCouter);
//		//// bind the buffer and define its initial storage capacity
//		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCouter);
//		//glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 3, NULL, GL_DYNAMIC_DRAW);
//		//// unbind the buffer 
//		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
//
//		//GLuint block_index = 0;
//		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "outputMesh");
//		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 0);
//		//
//		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "data");
//		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 1);
//		//
//		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "inputMesh");
//		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 2);
//
//
//		delete[] basisArray;
//
//	}
//
//	~ComputeShaderTest()
//	{
//		glDeleteBuffers(1, &ssboIn);
//		glDeleteBuffers(1, &ssboOut);
//		glDeleteBuffers(1, &ssboNumVerts);
//	}
//
//	void Dispatch()
//	{
//		glUseProgram(pObj->getProgramObject());
//		//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 16, ShapeWeights, GL_DYNAMIC_DRAW);
//
//		glDispatchCompute((GLuint)storedNumVerts / 128, 1, 1); //32 is from the compute shader for work sze
//		//glMemoryBarrier(GL_ALL_BARRIER_BITS);
//		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
//		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//		//GLuint *userCounters;
//		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCouter);
//		//// again we map the buffer to userCounters, but this time for read-only access
//		//userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
//		//	0,
//		//	sizeof(GLuint) * 3,
//		//	GL_MAP_READ_BIT
//		//);
//		//// copy the values to other variables because...
//		//unsigned int bufferCount = userCounters[0];
//		//// ... as soon as we unmap the buffer
//		//// the pointer userCounters becomes invalid.
//		//glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
//		//
//		//Trace::out("Atomic Count at %i\n", bufferCount);
//		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, countBuffers);
//		//GLuint *ptr = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
//		//GLuint vertCount = ptr[0];
//		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//		//Trace::out("VertCount = %i", vertCount);
//
//	}
//
//	GLuint getOutputStorageBlock()
//	{
//		return glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "outputMesh");
//	}
//
//	GLuint atomicCouter;
//	GLuint ssboIn;
//	GLuint ssboOut;
//	GLuint ssboNumVerts;
//	unsigned int storedNumVerts;
//	ComputeShaderObject* pObj;
//
//};

class CompositeAnimController2 : public baseManager
{
public:

	unsigned int controllerID;
	Time    internalTime;
	Skelly	Skeleton;
	Bone*   resultArray;
	Clip*   pCurClip;
	//ShapeClip*   pCurShapeClip;
	int		curAnims;


	GLuint bindBuffer;
	GLuint boneBuffer;

	//Matrix* bindArray;
	//Matrix* boneArray;

	//ComputeShaderTest* pTestcompute;
	//ComputeShaderDefoMixer* pTestcomputeDefo;
	//ComputeShaderDefoMixer* pTestMixer
	bool pad2[4];
	//TEMP to work on the skinning
	//GameObject* pMeshArray;
	//std::vector<GameObject> pMeshArray; 
	Matrix* boneResultArray;
	Matrix* bindPoseArray;


	CompositeAnimController2(const char* skeletonFile, const char* bindPoseFile, unsigned int _controllerId)
		: internalTime(Time::ZERO), pCurClip(0)// pClipHead(0), pCurClip(0)
	{

		//Hack
		controllerID = _controllerId;

		

		setSkellyFromFile(skeletonFile);
		setBindPoseFromFile(bindPoseFile);

		glGenBuffers(1, &bindBuffer);

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2*(1+controllerID), bindBuffer);
		glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 200), bindPoseArray, GL_STATIC_DRAW);

		glGenBuffers(1, &boneBuffer);

		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3 * (1 + controllerID), boneBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 200, boneResultArray, GL_DYNAMIC_DRAW);

		Trace::out("end Anim Constructor\n");

	}

	CompositeAnimController2(const CompositeAnimController2&) = delete;
	CompositeAnimController2& operator = (const CompositeAnimController2&) = delete;

	~CompositeAnimController2()
	{
		delete[] resultArray;
		delete[] boneResultArray;

		//delete[]  pMeshArray;
		delete[] bindPoseArray;

		glDeleteBuffers(1, &bindBuffer);
		glDeleteBuffers(1, &boneBuffer);

		//delete pTestcompute;
	}

	void SharedData(ShaderObject* pShaderObj, GraphicsObjectSkinLight* pGraphics)
	{
		glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->bindArray, 2*(1+controllerID));
		//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->boneArray, 3);

		//pShaderObj->SetActive();
		//
		//glBindBufferBase(GL_UNIFORM_BUFFER, pGraphics->bindArray, bindBuffer);
		//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->bindArray, bindBuffer);
		//
		//glBindBufferBase(GL_UNIFORM_BUFFER, pGraphics->boneArray, boneBuffer);
		//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->boneArray, boneBuffer);

	}

	void SharedDataWithShapes(ShaderObject* pShaderObj, GraphicsObject_SkinShapeDefo* pGraphics)
	{
		glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->bindArray, 2 * (1 + controllerID));
	}

	void pushBonesToGPU()
	{

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		//GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, bindPoseArray, sizeof(Matrix)*80);
		//glUnmapBuffer(GL_UNIFORM_BUFFER);

		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		//GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, boneResultArray, sizeof(Matrix) * 80);
		//glUnmapBuffer(GL_UNIFORM_BUFFER);

		//glBindBufferBase(GL_UNIFORM_BUFFER, 3, boneBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 80, boneResultArray, GL_STATIC_DRAW);

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 80), bindPoseArray, GL_STATIC_DRAW);
		//
		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 80), boneResultArray, GL_STATIC_DRAW);
	}

	//void addShapesToModel(Model* pMdl, GraphicsObject_SkinShapeDefo* pGraphicsObject, const char * shapesFileName)
	//{

	//	File::Handle fh;

	//	if (File::Error::SUCCESS == File::Open(fh, shapesFileName, File::Mode::READ))
	//	{

	//		int numShapes;
	//		unsigned int numVertsPerShape;
	//		unsigned int numKeyFrames;

	//		File::Read(fh, &numShapes, sizeof(int));
	//		File::Read(fh, &numVertsPerShape, sizeof(int));
	//		File::Read(fh, &numKeyFrames, sizeof(int));

	//		//Vert_xyz** Shapes = new Vert_xyz*[numShapes];

	//		Vert_xyz* Shape = new Vert_xyz[numVertsPerShape]{ 0 };

	//		//TODO:: do the compute shader instead for all the other buffer objects
	//		//pMdl->numShapes = numShapes;

	//		ComputeShaderDefoMixer* ShapeMixer = new ComputeShaderDefoMixer(numShapes,(int) numVertsPerShape);
	//		
	//		assert(pMdl->vao != 0);
	//		glBindVertexArray(pMdl->vao);
	//		glGenBuffers(1, &pMdl->vboComputeResult);

	//		glBindBuffer(GL_ARRAY_BUFFER, pMdl->vboComputeResult);

	//		//ShapeMixer->BindOutputVBO(pMdl->vboComputeResult);
	//		glBindVertexArray(pMdl->vao);
	//		
	//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pMdl->vboComputeResult);
	//		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyz)*numVertsPerShape), NULL, GL_STATIC_DRAW);

	//		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//		glEnableVertexAttribArray(5);


	//		//glShaderStorageBlockBinding(ShapeMixer->pObj->getProgramObject(), 0, 0);

	//		glGenBuffers(1, &ShapeMixer->vboData);

	//		ComputeHeader cpHdr;
	//		cpHdr.numShapes = numShapes;
	//		cpHdr.numVerts = (int) numVertsPerShape;

	//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ShapeMixer->vboData);

	//		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ComputeHeader), &cpHdr, GL_STATIC_DRAW);

	//		//TODO:: put the basis shapes into the .shapes file to make this easier and way faster

	//		glShaderStorageBlockBinding(ShapeMixer->pObj->getProgramObject(), 0, 2);

	//		glGenBuffers(1, &ShapeMixer->vboBasis);
	//		
	//		Vert_xyz* basisArray = new Vert_xyz[numVertsPerShape];

	//		for (unsigned int i = 0; i < numVertsPerShape; i++)
	//		{
	//			basisArray[i] = pMdl->modelData[i];
	//		}

	//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ShapeMixer->vboBasis);
	//		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(sizeof(Vert_xyz)*numVertsPerShape), basisArray, GL_STATIC_DRAW);

	//		delete basisArray;

	//		for (int readShape = 0; readShape < numShapes; readShape++)
	//		{
	//			File::Read(fh, Shape, sizeof(Vert_xyz)*numVertsPerShape);
	//			glGenBuffers(1, &ShapeMixer->vboShapes[readShape]);
	//			glBindBufferBase(GL_SHADER_STORAGE_BUFFER,  readShape+4u,ShapeMixer->vboShapes[readShape]);
	//			glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(sizeof(Vert_xyz)*numVertsPerShape), Shape, GL_STATIC_DRAW);
	//		}

	//		Trace::out("numShapes %i\n", numShapes);
	//		Trace::out("NumVerts %i\n",   numVertsPerShape);
	//		Trace::out("numKeyFrames %i\n", numKeyFrames);

	//		//float* KeyframeWeights = new float[numShapes];


	//		ShapeClip* pShapeClip = new ShapeClip();
	//		pShapeClip->maxTime = ((int)numKeyFrames) * Time(Time::NTSC_30_FRAME);
	//		pShapeClip->numShapes = numShapes;

	//		pShapeClip->pHead = new ShapeFrame_Bucket();
	//		pShapeClip->pHead->prevBucket = 0;
	//		pShapeClip->pHead->nextBucket = 0;
	//		pShapeClip->pHead->KeyTime = Time(Time::ZERO);

	//		pShapeClip->pMixerProgram = ShapeMixer;

	//		pCurClip->addShapeClip(pShapeClip);

	//		//pCurClip->pShapeHead->addToFront(pCurClip->pShapeHead, pShapeClip);

	//		pGraphicsObject->setWeightsArray(pShapeClip->pHead->pWeights, (unsigned int)numShapes);
	//		pShapeClip->pMixerProgram->ShapeWeights = pShapeClip->pHead->pWeights;

	//		glGenBuffers(1, &pShapeClip->pMixerProgram->vboWeights);
	//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, pShapeClip->pMixerProgram->vboWeights);
	//		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(sizeof(float) * 16), pShapeClip->pMixerProgram->ShapeWeights, GL_STATIC_DRAW);

	//		//GLuint block_index = 0;
	//		//block_index = glGetProgramResourceIndex(, GL_SHADER_STORAGE_BLOCK, "shader_data");


	//		ShapeFrame_Bucket* prev = pShapeClip->pHead;

	//		for (int FrameIndex = 0; FrameIndex < (int)numKeyFrames; FrameIndex++)
	//		{
	//			ShapeFrame_Bucket *pTmp = new ShapeFrame_Bucket();
	//			pTmp->prevBucket = prev;
	//			pTmp->nextBucket = 0;
	//			pTmp->KeyTime = FrameIndex * Time(Time::NTSC_30_FRAME);
	//			prev->nextBucket = pTmp;

	//			prev = pTmp;

	//			File::Read(fh, pTmp->pWeights, sizeof(float) * numShapes);
	//			//File::Read(fh, KeyframeWeights, sizeof(float) * numShapes);

	//			/*Trace::out("Frame %i ------------\n Weights: ", FrameIndex);
	//			for (int weightindex = 0; weightindex < numShapes; weightindex++)
	//			{
	//				Trace::out(" %f ", pTmp->pWeights[weightindex]);


	//			}
	//			Trace::out("\n");*/
	//		}
	//		File::Close(fh);

	//		delete[] Shape;

	//	}
	//	else
	//	{
	//		Trace::out("Shape File Failed to open\n");
	//		assert(false);
	//	}
	//}

	void addShapesToModel(Model* pMdl, ShaderObject* pShaderObj, GraphicsObject_SkinShapeDefo* pGraphicsObject, const char * shapesFileName)
	{

		UNUSED_VAR(pGraphicsObject);
		UNUSED_VAR(shapesFileName);

		File::Handle fh;

		if (File::Error::SUCCESS == File::Open(fh, shapesFileName, File::Mode::READ))
		{

			int numShapes;
			unsigned int numVertsPerShape;
			unsigned int numKeyFrames;

			File::Read(fh, &numShapes, sizeof(int));
			File::Read(fh, &numVertsPerShape, sizeof(int));
			File::Read(fh, &numKeyFrames, sizeof(int));

			//Vert_xyz* Shape = new Vert_xyz[numVertsPerShape]{ 0 };

			ComputeShaderDefoMixer* ShapeMixer = new ComputeShaderDefoMixer((unsigned int) numShapes, (unsigned int)pMdl->numVerts, pMdl);

			assert(pMdl->vao != 0);
			glUseProgram(pShaderObj->getProgramObject());
			glBindVertexArray(pMdl->vao);

			//glGenBuffers(1, &pMdl->vboComputeResult);

			//glBindBuffer(GL_ARRAY_BUFFER, pMdl->vboComputeResult);
			glBindBuffer(GL_ARRAY_BUFFER, ShapeMixer->ssboOut);
			//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*pMdl->numVerts, basisArray, GL_STATIC_DRAW);

			glVertexAttribPointer(5, 4 ,GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(5);

			ShapeMixer->loadShapes(fh, numShapes, (int)numVertsPerShape);

			//ShapeMixer->Dispatch();

			Trace::out("numShapes %i\n", numShapes);
			Trace::out("NumVerts %i\n", numVertsPerShape);
			Trace::out("numKeyFrames %i\n", numKeyFrames);

			ShapeClip* pShapeClip = new ShapeClip();
			pCurClip->addShapeClip(pShapeClip);

			pShapeClip->maxTime = ((int)numKeyFrames) * Time(Time::NTSC_30_FRAME);
			pShapeClip->numShapes = numShapes;

			pShapeClip->pHead = new ShapeFrame_Bucket();
			pShapeClip->pHead->prevBucket = 0;
			pShapeClip->pHead->nextBucket = 0;
			pShapeClip->pHead->KeyTime = Time(Time::ZERO);

			pShapeClip->pMixerProgram = ShapeMixer;
			ShapeMixer->ShapeWeights = pShapeClip->pHead->pWeights;

			ShapeFrame_Bucket* prev = pShapeClip->pHead;

			for (int FrameIndex = 0; FrameIndex < (int)numKeyFrames; FrameIndex++)
			{
				ShapeFrame_Bucket *pTmp = new ShapeFrame_Bucket();
				pTmp->prevBucket = prev;
				pTmp->nextBucket = 0;
				pTmp->KeyTime = FrameIndex * Time(Time::NTSC_30_FRAME);
				prev->nextBucket = pTmp;

				prev = pTmp;

				File::Read(fh, pTmp->pWeights, sizeof(float) * numShapes);
				//File::Read(fh, KeyframeWeights, sizeof(float) * numShapes);

				/*Trace::out("Frame %i ------------\n Weights: ", FrameIndex);
				for (int weightindex = 0; weightindex < numShapes; weightindex++)
				{
					Trace::out(" %f ", pTmp->pWeights[weightindex]);


				}
				Trace::out("\n");*/
			}
			File::Close(fh);

			//	ShapeMixer->Dispatch();


			//	//float* KeyframeWeights = new float[numShapes];
		}
		else
		{
			Trace::out("Shape File Failed to open\n");
			assert(false);
		}

		//	ShapeClip* pShapeClip = new ShapeClip();
		//	pShapeClip->maxTime = ((int)numKeyFrames) * Time(Time::NTSC_30_FRAME);
		//	pShapeClip->numShapes = numShapes;

		//	pShapeClip->pHead = new ShapeFrame_Bucket();
		//	pShapeClip->pHead->prevBucket = 0;
		//	pShapeClip->pHead->nextBucket = 0;
		//	pShapeClip->pHead->KeyTime = Time(Time::ZERO);

		//	pShapeClip->pMixerProgram = ShapeMixer;

		//	pCurClip->addShapeClip(pShapeClip);

		//	//pCurClip->pShapeHead->addToFront(pCurClip->pShapeHead, pShapeClip);

		//	pGraphicsObject->setWeightsArray(pShapeClip->pHead->pWeights, (unsigned int)numShapes);
		//	pShapeClip->pMixerProgram->ShapeWeights = pShapeClip->pHead->pWeights;

		//	ShapeFrame_Bucket* prev = pShapeClip->pHead;

		//	for (int FrameIndex = 0; FrameIndex < (int)numKeyFrames; FrameIndex++)
		//	{
		//		ShapeFrame_Bucket *pTmp = new ShapeFrame_Bucket();
		//		pTmp->prevBucket = prev;
		//		pTmp->nextBucket = 0;
		//		pTmp->KeyTime = FrameIndex * Time(Time::NTSC_30_FRAME);
		//		prev->nextBucket = pTmp;

		//		prev = pTmp;

		//		File::Read(fh, pTmp->pWeights, sizeof(float) * numShapes);
		//		//File::Read(fh, KeyframeWeights, sizeof(float) * numShapes);

		//		/*Trace::out("Frame %i ------------\n Weights: ", FrameIndex);
		//		for (int weightindex = 0; weightindex < numShapes; weightindex++)
		//		{
		//			Trace::out(" %f ", pTmp->pWeights[weightindex]);
		//		}
		//		Trace::out("\n");*/
		//	}
		//	File::Close(fh);

		//	delete[] Shape;

		//	ShapeMixer->dispatch();
		//}
		//else
		//{
		//	Trace::out("Shape File Failed to open\n");
		//	assert(false);
		//}
	}


	//void workingConfig(Model* pMdl, ShaderObject* pShaderObj)
	//{
	//	Vert_xyz* basisArray = new Vert_xyz[(unsigned int)pMdl->numVerts];

	//	for (unsigned int i = 0; i < (unsigned int)pMdl->numVerts; i++)
	//	{
	//		basisArray[i] = pMdl->modelData[i];
	//	}

	//	assert(pMdl->vao != 0);
	//	glUseProgram(pShaderObj->getProgramObject());
	//	glBindVertexArray(pMdl->vao);

	//	glGenBuffers(1, &pMdl->vboComputeResult);

	//	glBindBuffer(GL_ARRAY_BUFFER, pMdl->vboComputeResult);
	//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*pMdl->numVerts, basisArray, GL_STATIC_DRAW);

	//	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(5);

	//	delete[] basisArray;
	//}

	//void DebugComputeShader(Model* pMdl, ShaderObject* pShaderObj)
	//{
	//	pTestcompute = new ComputeShaderTest((unsigned int)pMdl->numVerts, pMdl);

	//	GLuint ssboOut = pTestcompute->getOutputStorageBlock();
	//	Trace::out("ssboOut bound to %i\n", ssboOut);

	//	pTestcompute->Dispatch();

	//	/*Vert_xyz* basisArray = new Vert_xyz[(unsigned int)pMdl->numVerts];

	//	for (unsigned int i = 0; i < (unsigned int)pMdl->numVerts; i++)
	//	{
	//		basisArray[i] = pMdl->modelData[i];
	//	}*/

	//	assert(pMdl->vao != 0);
	//	glUseProgram(pShaderObj->getProgramObject());
	//	glBindVertexArray(pMdl->vao);

	//	//glGenBuffers(1, &pMdl->vboComputeResult);

	//	//glBindBuffer(GL_ARRAY_BUFFER, pMdl->vboComputeResult);
	//	glBindBuffer(GL_ARRAY_BUFFER, pTestcompute->ssboOut);
	//	//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*pMdl->numVerts, basisArray, GL_STATIC_DRAW);

	//	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(5);

	//	//delete[] basisArray;

	//	//Use buffer as a set of vertices
	//	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	//	//glBindBuffer(GL_ARRAY_BUFFER, pTestcompute->ssboOut);
	//	//glBindBuffer(GL_ARRAY_BUFFER, ssboOut);
	//	//
	//	//
	//	////Create the vertex shader
	//	//glUseProgram(pShaderObj->getProgramObject());
	//	//
	//	//	//Enable the vertex array
	//	//glVertexAttribPointer(5, 3, GL_FLOAT, 0, 0, (void*)0);
	//	//glEnableVertexAttribArray(5);

	//		//Draw it
	//		//glClear(GL_COLOR_BUFFER_BIT);
	//		//glBindVertexArray(VAO);
	//		//glDrawArrays(GL_POINTS, 0, numVertices);
	//		//glFlush();

	//	//		glBindVertexArray(pMdl->vao);
	//	//		glGenBuffers(1, &pMdl->vboComputeResult);

	//	//		glBindBuffer(GL_ARRAY_BUFFER, pMdl->vboComputeResult);

	//	//		//ShapeMixer->BindOutputVBO(pMdl->vboComputeResult);
	//	//		glBindVertexArray(pMdl->vao);
	//	//		
	//	//		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pMdl->vboComputeResult);
	//	//		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyz)*numVertsPerShape), NULL, GL_STATIC_DRAW);

	//	//		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	//		glEnableVertexAttribArray(5);

	//	//Vert_xyz* basisArray = new Vert_xyz[(unsigned int)pMdl->numVerts];
	//	//
	//	//for (int i = 0; i < pMdl->numVerts; i++)
	//	//{
	//	//	basisArray[i] = pMdl->modelData[i];
	//	//}
	//	//
	//	//glUseProgram(pShaderObj->getProgramObject());
	//	//glBindVertexArray(pMdl->vao);
	//	//
	//	//glGenBuffers(1, &pMdl->vboComputeResult);
	//	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, pMdl->vboComputeResult);
	//	//glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)pMdl->numVerts, basisArray, GL_DYNAMIC_COPY);
	//	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//	//
	//	//
	//	//GLuint block_index = 0;
	//	//block_index = glGetProgramResourceIndex(pShaderObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "shapeVertObj");
	//	//glShaderStorageBlockBinding(pShaderObj->getProgramObject(), block_index, 5);
	//	//
	//	//delete[] basisArray;

	//	//	//glGenBuffers(1, &pMdl->vboComputeResult);
	//	//
	//	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, pTestcompute->ssboOut);
	//	//
	//	//GLuint ssbo_binding_point_index = 0;
	//	//glShaderStorageBlockBinding(pShaderObj->getProgramObject(), ssboOut, ssbo_binding_point_index);
	//	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssboOut, pMdl->vboComputeResult);
	//	//
	//	//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//	//glEnableVertexAttribArray(5);

	//}

	void addMeshWithShapes(Model* pMdl, ShaderObject *pShaderObj, TextureName tName, const char* shapesFileName, bool lighting = true)
	{
		UNUSED_VAR(lighting);
		UNUSED_VAR(shapesFileName);

		GameObjectRigid *pGameObj;
		GraphicsObject_SkinShapeDefo  *pGraphics;

		Vect color(1.0f, 1.0f, 1.0f, 1.0f);
		Vect pos(0.0f, 1000.0f, 4.0f,1.0f);

		//Humanoid Skin
		//pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, TextureName::Gray, color, pos);
		if (lighting)
		{
			//pGraphics = new GraphicsObject_SkinShapeDefo(pMdl, pShaderObj, tName, true);
			pGraphics = (GraphicsObject_SkinShapeDefo*) new GraphicsObject_SkinShapeDefoLit(pMdl, pShaderObj, tName, color, pos, true);
		}
		else
		{
			pGraphics = (GraphicsObject_SkinShapeDefo*) new GraphicsObject_SkinShapeDefo(pMdl, pShaderObj, tName);
		}

		//DebugComputeShader(pMdl, pShaderObj);

		addShapesToModel(pMdl, pShaderObj, pGraphics, shapesFileName);

		//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pMdl, pShaderObj, TextureName::Gray, color, pos, StageHeapPtr);

		pGraphics->SetBoneArray(boneResultArray, Skeleton.numBones);

		pGraphics->setBindArray(bindPoseArray, Skeleton.numBones);

		//pGraphics->bindArray = bindBuffer;
		//pGraphics->boneArray = boneBuffer;
		SharedDataWithShapes(pShaderObj, pGraphics);

		pGameObj = new GameObjectRigid(pGraphics);

		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());//TODO::change to skelly heigherarchy root so everythign to do with this character can be collision tested together

	}

	//must be skinned model IDK if I'm gonna differentiate yet or how.
	void addMesh(Model* pMdl, ShaderObject * pShaderObj, TextureName tName, bool DoubleSided, bool lighting = false)
	{

		GameObjectRigid *pGameObj;
		//GraphicsObject_SkinFT  *pGraphics;
		GraphicsObjectSkinLight *pGraphics;

		Vect color(1.0f, 1.0f, 1.0f, 1.0f);
		Vect pos(0.0f, -100.0f, 10.0f);

		//Humanoid Skin
		//pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, TextureName::Gray, color, pos);


		if (DoubleSided)
		{ //TODO:: allow double sided in skinlight

			if (lighting)
			{
				pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, tName, color, pos);
				pGraphics->disableBFC();
			} 
			else
			{
				pGraphics = (GraphicsObjectSkinLight*) new GraphicsObject_SkinFT(pMdl, pShaderObj, tName);
			}
		}
		else
		{
			if (lighting)
			{
				pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, tName, color, pos);
 			} 
			else
			{
				pGraphics = (GraphicsObjectSkinLight*) new GraphicsObject_SkinFT(pMdl, pShaderObj, tName);
			}
			
		}

		//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pMdl, pShaderObj, TextureName::Gray, color, pos, StageHeapPtr);

		pGraphics->SetBoneArray(boneResultArray, Skeleton.numBones);

		pGraphics->setBindArray(bindPoseArray, Skeleton.numBones);

		//pGraphics->bindArray = bindBuffer;
		//pGraphics->boneArray = boneBuffer;
		SharedData(pShaderObj, pGraphics);

		pGameObj = new GameObjectRigid(pGraphics);

		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());//TODO::change to skelly heigherarchy root so everythign to do with this character can be collision tested together
	}

	void setSkelly(Skelly& _Skeleton)
	{
		this->Skeleton = _Skeleton;
	}

	void setBindPoseFromFile(const char* FileName)
	{
		bindPoseArray = new Matrix[Skeleton.numBones]();
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{
			unsigned int numBinds = 0;

			File::Read(fh, &numBinds, sizeof(int));

			int* bindIndex = new int[numBinds];
			Matrix* tempBinds = new Matrix[numBinds];

			File::Read(fh, bindIndex, numBinds * sizeof(int));
			File::Read(fh, tempBinds, numBinds * sizeof(float) * 16);

			for (unsigned int i = 0; i < numBinds; i++)
			{
				Trace::out("bind %i maps to bone %i\n", i, bindIndex[i]);
				bindPoseArray[bindIndex[i]] = tempBinds[i];
			}

			//bindPoseArray[135] = tempBinds[126];
			//bindPoseArray[136] = tempBinds[127];

			delete[] bindIndex;
		}
	}

	void setSkellyFromFile(const char* FileName)
	{
		// Load the model
		//PyramidModel *pPyramidModel = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) PyramidModel("pyramidModel.azul");
//		Model *pPyramidModel = ModelMan::find(ModelName::Pryamid);

		//assert(pPyramidModel);

		// Create/Load Shader 
		//ShaderObject *pShaderObject_textureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
//		ShaderObject* pShaderObject_textureLight = ShaderManager::addShader(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
		//assert(pShaderObject_textureLight);

		// GraphicsObject for a specific instance
//		GraphicsObject_TextureLight *pGraphics_TextureLight;

		// Create GameObject
		Vect color(1.50f, 1.50f, 1.50f, 1.5f);
		Vect pos(-1.0f, -1.0f, -1.0f);

		PCSTree *pTree = GameObjectMan::GetPCSTree();
		//root = pTree->GetRoot();

		File::Handle fh;

		SkellyBone* boneArray;

		GameObjectAnim* pBObj;

		GameObjectAnim** GameObjectList = 0;

		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{

			unsigned int BoneCount = 0;

			File::Read(fh, &BoneCount, sizeof(int));

			//boneArray = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) SkellyBone[BoneCount];
			boneArray = new SkellyBone[BoneCount];
			//resultArray = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[BoneCount];
			resultArray = new Bone[BoneCount];
			//GameObjectList = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim*[BoneCount];
			GameObjectList = new GameObjectAnim*[BoneCount];

			//TEMP to debug vertex shader
			boneResultArray = new Matrix[BoneCount];

			//boneArray = new SkellyBone[BoneCount];
			//GameObjectList = new GameObjectAnim*[BoneCount];

			File::Read(fh, boneArray, BoneCount * sizeof(SkellyBone));

			//pGraphics_TextureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			//pBObj = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetHeap());

			//NullModel* pNullMod = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) NullModel(0);
			Model* pNullMod = ModelMan::find(ModelName::NullMod);
			assert(pNullMod);
			ShaderObject *pShader = ShaderManager::find(ShaderObject::ShaderName::NULL_SHADER);

			//GraphicsObject* pNullGraphics = new(ScratchSpace::GetGraphicsHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Null(pNullMod, pShader, ScratchSpace::GetHeap());
			GraphicsObject* pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);

			GameObjectRigid* pContainer = new GameObjectRigid(pNullGraphics);
			Skeleton.pHeriarchyRoot = pContainer;

			GameObjectMan::Add(Skeleton.pHeriarchyRoot, GameObjectMan::GetRoot());

			pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);
			//pShader = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			pBObj = new GameObjectAnim(pNullGraphics);

			pBObj->SetIndex(boneArray[0].index);
			pBObj->SetName(boneArray[0].name);
			pBObj->SetResult(resultArray);

			GameObjectMan::Add(pBObj, Skeleton.pHeriarchyRoot);

			GameObjectList[0] = pBObj;
			//pFirstBone = pBObj;
			//Skeleton.pHeriarchyRoot = pBObj;
			Skeleton.numBones = BoneCount;

			for (unsigned int i = 1; i < BoneCount; i++)
			{

				//pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos);
				pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);
				//pBObj = new(ScratchSpace::GetGOHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetGOManHeap());
				pBObj = new GameObjectAnim(pNullGraphics);

				pBObj->SetIndex(boneArray[i].index);
				pBObj->SetName(boneArray[i].name);
				pBObj->SetResult(resultArray);
				GameObjectMan::Add(pBObj, GameObjectList[boneArray[i].parentIndex]);

				GameObjectList[i] = pBObj;

			}

			File::Close(fh);

			delete[] boneArray;
			delete[] GameObjectList;

		}

		pTree->Print();

	}

	void nextAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void prevAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void SetAnimationPose(const Time&)
	{

		// REMEMBER:
		//    that the bone pose are WORLD not GLOBAL
		//    Animations are relative!

		// First thing, get the first frame of animation
		//Time tCurr(Time::ZERO);

		//ProcessAnimation(tCurr);

		//calls update on every object //TODO: optimize this away there has to be a better way.
		//GameObjectMan::Update(tCurr);

		PCSTreeForwardIterator pItr(Skeleton.pHeriarchyRoot);
		PCSNode *pNode = pItr.First();
		pNode = pItr.Next();
		GameObjectAnim *pGameObj = 0;

		//Trace::out("Pose start\n");
		int loopCount = 0;
		while (!pItr.IsDone())
		{
			assert(pNode);

			pGameObj = (GameObjectAnim *)pNode;

			//Trace::out("Updating Item #%i @addr %x ", loopCount, pGameObj);
			//pGameObj->PrintNode();

			setBonePose(pGameObj);
			//Trace::out("\n");

			pNode = pItr.Next();
			loopCount++;
		}

		pushBonesToGPU();

		//Trace::out("End of pose\n");
		//GameObject *pChildNode = (GameObject *)pNode->GetChild();
		//GameObjectAnim*pParentNode = (GameObjectAnim*)pNode;

		//walk_anim_node(pNode);

	}


	void setBonePose(GameObjectAnim*node)
	{
		// Now get the world matrices
		GameObjectAnim * childNode = (GameObjectAnim *)node;
		GameObjectAnim * parentNode;// = (GameObjectAnim *)node->GetParent();

									//if (parentNode == Skeleton.pHeriarchyRoot->GetChild() || parentNode == Skeleton.pHeriarchyRoot)
									//return;

									//if (node->GetParent()->GetParent()->GetParent() != 0)
									//	parentNode = (GameObjectAnim *)node->GetParent();
									//else
									//	parentNode = 0;

		if (node->GetParent()->GetParent() != 0)
			parentNode = (GameObjectAnim *)node->GetParent();
		else
			parentNode = 0;
		if (parentNode != 0 && childNode != 0)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions
			Vect ptA = start * *parentNode->GetWorld();
			Vect ptB = start * *childNode->GetWorld();

			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(SCALE, BONE_WIDTH, BONE_WIDTH, mag);
			//Quat   Q(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f) * *childNode->GetWorld());
			Quat   Q(ROT_ORIENT, dir.norm(), Vect(0.0f, 1.0f, 0.0f) * *parentNode->GetWorld());
			Matrix T(TRANS, ptB);
			Matrix BoneOrient = S * Q * T;

			//Matrix Spre(SCALE, Vect(0.15f, 0.15f, 0.15f, 0.15f));
			//Matrix S(SCALE, bResult->S);
			//Matrix R(bResult->Q);
			//Matrix T(TRANS, bResult->T);

			Bone tmp = resultArray[childNode->indexBoneArray];

			/*Matrix toInv = *childNode->pLocal;
			toInv.inv();*/

			boneResultArray[childNode->indexBoneArray] = (Matrix(SCALE, tmp.S) * tmp.Q * Matrix(TRANS, tmp.T) ** parentNode->GetWorld());
			//boneResultArray[childNode->indexBoneArray] = (Matrix(SCALE, 1.0f,1.0f,1.0f) * tmp.Q * Matrix(TRANS, tmp.T) ** parentNode->GetWorld());

			childNode->SetBoneOrientation(BoneOrient);
		}
	}


	/*void addClip(Clip*)
	{

	}*/

	void addClipFromFile(const char* fileName)
	{
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, fileName, File::Mode::READ))
		{

			pCurClip = new Clip();

			addToActive((DLink&)*pCurClip);

			unsigned int frameCount = 0;
			unsigned int boneCount = 0;
			File::Read(fh, &frameCount, sizeof(unsigned int));
			File::Read(fh, &boneCount, sizeof(unsigned int));
			assert(frameCount != 0);
			assert(boneCount != 0);

			pCurClip->maxTime = ((int)frameCount) * Time(Time::NTSC_30_FRAME);

			// --------  Result Frame  ----------------------

			pCurClip->pHead = new Frame_Bucket();
			pCurClip->pHead->prevBucket = 0;
			pCurClip->pHead->nextBucket = 0;
			pCurClip->pHead->KeyTime = Time(Time::ZERO);
			//pCurClip->pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[boneCount];
			pCurClip->pHead->pBone = new Bone[boneCount];

			Frame_Bucket* prev = pCurClip->pHead;

			//HACKY signed unsigned mix here because there's no unsigned operator in the TIME lib yet.
			for (int i = 0; i < (int)frameCount; i++)
			{

				Frame_Bucket *pTmp = new Frame_Bucket();
				pTmp->prevBucket = prev;
				pTmp->nextBucket = 0;
				pTmp->KeyTime = i * Time(Time::NTSC_30_FRAME);
				//pTmp->pBone = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[boneCount];
				pTmp->pBone = new Bone[boneCount];
				prev->nextBucket = pTmp;

				prev = pTmp;

				File::Read(fh, pTmp->pBone, sizeof(Bone) * boneCount);

				//for (unsigned int j = 0; j < boneCount; j++)
				//{
				//	pTmp->pBone[j].S = Vect(1.0f, 1.0f, 1.0f);
				//}

				//Debugging WonkyLegs

				//if (i == 5)
				//{
				//	for (unsigned int bone = 0; bone < boneCount; bone++)
				//	{
				//		resultArray[bone].T.set(pTmp->pBone[bone].T);
				//		resultArray[bone].S.set(pTmp->pBone[bone].S);
				//	}
				//	
				//}

				//int bone1 = 127;
				//int bone2 = 136;
				//
				//if (pTmp->pBone[bone1] != pTmp->pBone[bone2])
				//{
				//	Trace::out("Frame %i doesn't match!!\nf1 T(%f , %f , %f ) Q (%f , %f , %f , %f) S( %f, %f, %f)\n",i, pTmp->pBone[bone1].T.x(), pTmp->pBone[bone1].T.y(), pTmp->pBone[bone1].T.z(), pTmp->pBone[bone1].Q.qx(), pTmp->pBone[bone1].Q.qy(), pTmp->pBone[bone1].Q.qz(), pTmp->pBone[bone1].Q.real(), pTmp->pBone[bone1].S.x(), pTmp->pBone[bone1].S.y(), pTmp->pBone[bone1].S.z());
				//	Trace::out("f2 T(%f , %f , %f ) Q (%f , %f , %f , %f) S( %f, %f, %f)\n", pTmp->pBone[bone2].T.x(), pTmp->pBone[bone2].T.y(), pTmp->pBone[bone2].T.z(), pTmp->pBone[bone2].Q.qx(), pTmp->pBone[bone2].Q.qy(), pTmp->pBone[bone2].Q.qz(), pTmp->pBone[bone2].Q.real(), pTmp->pBone[bone2].S.x(), pTmp->pBone[bone2].S.y(), pTmp->pBone[bone2].S.z());
				//	//pTmp->pBone[bone1] = pTmp->pBone[bone2];
				//	//pTmp->pBone[128] = pTmp->pBone[137];
				//
				//	//pTmp->pBone[135] = pTmp->pBone[126];
				//	//pTmp->pBone[136] = pTmp->pBone[127];
				//	//pTmp->pBone[137] = pTmp->pBone[128];
				//
				//	pTmp->pBone[135].S = Vect(1.0f, 1.0f, 1.0f);
				//	pTmp->pBone[136].S = Vect(1.0f, 1.0f, 1.0f);
				//	pTmp->pBone[137].S = Vect(1.0f, 1.0f, 1.0f);
				//
				//	//pTmp->pBone[138] = pTmp->pBone[1]
 			//	
				//}

			}
			Trace::out("FrameCount = %i\n",frameCount);
			File::Close(fh);

			//processAnimHelper(Time(Time::ZERO), pCurClip);
		}
		else
		{
			assert(false);
		}
	}

	void FindMaxTime(Time &tMax)
	{
		Frame_Bucket *pTmp = pCurClip->pHead->nextBucket;

		while (pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;
	}

	float fLerp(const float a, const float b, float t)
	{
		return a + t * (b - a);
	}

	void shapeBlend(const Time& tCurr, ShapeClip* pClip)
	{
			ShapeFrame_Bucket* pHead = pClip->pHead;
			// Note: these static should be tied into an animation system

			ShapeFrame_Bucket *pTmp = pHead->nextBucket;

			// Get the result bone array
			// Remember the firs Frame is the result
			//Bone *bResult = pHead->pBone;
			float *sResult = pHead->pWeights;

			// First one is the result, skip it
			pTmp = pHead->nextBucket;

			// Find which key frames
			while (tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0)
			{
				pTmp = pTmp->nextBucket;
			}

			// pTmp is the "B" key frame
			// pTmp->prev is the "A" key frame
			ShapeFrame_Bucket *pA = pTmp->prevBucket;
			ShapeFrame_Bucket *pB = pTmp;

			// find the "S" of the time
			float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

			if (tS > 1.0f)
			{
				tS = 1.0f;
			}

			// interpolate to 
			float *bA = pA->pWeights;
			float *bB = pB->pWeights;
			//Trace::out("Shape Lerp\n");
			// Interpolate to tS time, for all bones
			for (unsigned int i = 0; i < (unsigned int)pClip->numShapes; i++)
			{
				//If there is only one targetShape on this channel, the influence is easy to calculate :
				//influence = (targetShape - baseGeometry) * weight * 0.01

				//Lerp the weights and scale by .01
				sResult[i] = fLerp(*bA, *bB, tS);

				//Trace::out("result %i = %f is %f between %f and %f\n", i, sResult[i], tS, *bA, *bB);

				bA++;
				bB++;
				//sResult++;
			}
	}


	void DeformShapes(const Time& tCurr, Clip* pClip)
	{

		ShapeClip* cur = pClip->pShapeHead;
		while (cur)
		{
			shapeBlend(tCurr, cur);
			cur->pMixerProgram->Dispatch();
			cur = (ShapeClip*)cur->pNext;
		}
	}

	void processAnimHelper(const Time& tCurr, Clip* pClip)
	{
		//pTestcomputeDefo->Dispatch();
		DeformShapes(tCurr, pClip);

		Frame_Bucket* pHead = pClip->pHead;
		// Note: these static should be tied into an animation system

		Frame_Bucket *pTmp = pHead->nextBucket;

		// Get the result bone array
		// Remember the firs Frame is the result
		//Bone *bResult = pHead->pBone;
		Bone *bResult = resultArray;

		// First one is the result, skip it
		pTmp = pHead->nextBucket;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		Frame_Bucket *pA = pTmp->prevBucket;
		Frame_Bucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);


		if (tS > 1.0f)
		{
			tS = 1.0f;
		}

		// interpolate to 
		Bone *bA = pA->pBone;
		Bone *bB = pB->pBone;

		// Interpolate to tS time, for all bones
		for (unsigned int i = 0; i < Skeleton.numBones; i++)
		{
			// interpolate ahoy!
			//if (i < 3)
			//{
				VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
				QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
				VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
			//}
			//else
			//{

			//	//VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
			//	QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
			//	//VectApp::Lerp(bResult->S, bA->S, bB->S, tS);
			//}
			//boneResultArray[i] = Matrix(TRANS,bResult->T) * Matrix(bResult->Q) * Matrix(SCALE, bResult->S);


			// advance the pointer
			bA++;
			bB++;
			bResult++;
		}
	}

	void CProcessAnimation(const Time& deltaTime)
	//void CProcessAnimation(const float deltaT)
	{
		//shapeBlend(internalTime, pCurShapeClip);
//		pTestcompute->Dispatch();
		processAnimHelper(internalTime, pCurClip);

		//processAnimHelper(Time(Time::ZERO), pCurClip);

		//Time deltaTime = deltaT * Time(Time::NTSC_30_FRAME);
		Time maxTime;

		SetAnimationPose(deltaTime);

		internalTime += deltaTime;

		FindMaxTime(maxTime);

		static const Time tZero(Time::ZERO);

		// protection for time values for looping
		if (internalTime < tZero)
		{
			internalTime = maxTime;
		}
		else if (internalTime > maxTime)
		{
			//internalTime = tZero;
		}
		else
		{
			// do nothing
		}

	}

	DLink* dCreateNode()
	{
		DLink* newNode = (DLink*) new Clip();
		assert(newNode != nullptr);
		return newNode;
	}

	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		bool retval = false;
		if (((CameraLink*)&pLinkA)->GetName() == ((CameraLink*)&pLinkB)->GetName())
		{
			retval = true;
		}

		return retval;

	}

	void dClearNode(DLink* pLink)
	{
		pLink->dClean();
	}
};

class CompositeAnimController : public baseManager
{
public:

	bool pad1[4];
	Time    internalTime;
	Skelly Skeleton;
	Bone*   resultArray;
	Clip*   pCurClip;
	int		curAnims;


	GLuint bindBuffer;
	GLuint boneBuffer;

	//Matrix* bindArray;
	//Matrix* boneArray;


	bool pad2[4];
	//TEMP to work on the skinning
	//GameObject* pMeshArray;
	//std::vector<GameObject> pMeshArray; 
	Matrix* boneResultArray;
	Matrix* bindPoseArray;


	CompositeAnimController(const char* skeletonFile, const char* bindPoseFile)
		: internalTime(Time::ZERO), pCurClip(0)// pClipHead(0), pCurClip(0)
	{
		setSkellyFromFile(skeletonFile);
		setBindPoseFromFile(bindPoseFile);

		glGenBuffers(1, &bindBuffer);

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, bindBuffer);
		glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 200), bindPoseArray, GL_STATIC_DRAW);

		glGenBuffers(1, &boneBuffer);

		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, boneBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 200, boneResultArray, GL_STATIC_DRAW);

	}

	CompositeAnimController(const CompositeAnimController&) = delete;
	CompositeAnimController& operator = (const CompositeAnimController&) = delete;

	~CompositeAnimController()
	{
		delete[] resultArray;
		delete[] boneResultArray;

		//delete[]  pMeshArray;
		delete[] bindPoseArray;

		//glDeleteBuffers(1, &bindBuffer);
		glDeleteBuffers(1, &boneBuffer);
	}

	void SharedData(ShaderObject* pShaderObj, GraphicsObject_SkinFT* pGraphics)
	{
		glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->bindArray, 2);
		//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->boneArray, 3);

	//pShaderObj->SetActive();
	//
	//glBindBufferBase(GL_UNIFORM_BUFFER, pGraphics->bindArray, bindBuffer);
	//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->bindArray, bindBuffer);
	//
	//glBindBufferBase(GL_UNIFORM_BUFFER, pGraphics->boneArray, boneBuffer);
	//glUniformBlockBinding(pShaderObj->getProgramObject(), pGraphics->boneArray, boneBuffer);


	}

	void pushBonesToGPU()
	{

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		//GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, bindPoseArray, sizeof(Matrix)*80);
		//glUnmapBuffer(GL_UNIFORM_BUFFER);

		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		//GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		//memcpy(p, boneResultArray, sizeof(Matrix) * 80);
		//glUnmapBuffer(GL_UNIFORM_BUFFER);

		//glBindBufferBase(GL_UNIFORM_BUFFER, 3, boneBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 80, boneResultArray, GL_STATIC_DRAW);

		//glBindBuffer(GL_UNIFORM_BUFFER, bindBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 80), bindPoseArray, GL_STATIC_DRAW);
		//
		//glBindBuffer(GL_UNIFORM_BUFFER, boneBuffer);
		//glBufferData(GL_UNIFORM_BUFFER, (sizeof(Matrix) * 80), boneResultArray, GL_STATIC_DRAW);
	}

	//must be skinned model IDK if I'm gonna differentiate yet or how.
	void addMesh(Model* pMdl, ShaderObject * pShaderObj, TextureName tName, bool DoubleSided)
	{
		GameObjectRigid *pGameObj;
		GraphicsObject_SkinFT  *pGraphics;

		Vect color(1.50f, 1.50f, 1.50f, 1.5f);
		Vect pos(0.0f, 100.0f, 100.0f);

		//Humanoid Skin
		//pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, TextureName::Gray, color, pos);
		if (DoubleSided)
		{
			pGraphics = (GraphicsObject_SkinFT*) new GraphicsObject_DoubleSidedTexture(pMdl, pShaderObj, tName);

		}
		else
		{
			pGraphics = new GraphicsObject_SkinFT(pMdl, pShaderObj, tName);
		}

		//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pMdl, pShaderObj, TextureName::Gray, color, pos, StageHeapPtr);

		pGraphics->SetBoneArray(boneResultArray, Skeleton.numBones);

		pGraphics->setBindArray(bindPoseArray, Skeleton.numBones);

		//pGraphics->bindArray = bindBuffer;
		//pGraphics->boneArray = boneBuffer;
		SharedData(pShaderObj, pGraphics);

		pGameObj = new GameObjectRigid(pGraphics);

		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());//TODO::change to skelly heigherarchy root so everythign to do with this character can be collision tested together
	}

	void setSkelly(Skelly& _Skeleton)
	{
		this->Skeleton = _Skeleton;
	}

	void setBindPoseFromFile(const char* FileName)
	{
		bindPoseArray = new Matrix[Skeleton.numBones]();
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{
			unsigned int numBinds = 0;

			File::Read(fh, &numBinds, sizeof(int));

			int* bindIndex = new int[numBinds];
			Matrix* tempBinds = new Matrix[numBinds];

			File::Read(fh, bindIndex, numBinds * sizeof(int));
			File::Read(fh, tempBinds, numBinds * sizeof(float) * 16);

			for (unsigned int i = 0; i < numBinds; i++)
			{
				//Trace::out("bind %i maps to bone %i\n", i, bindIndex[i]);
				bindPoseArray[bindIndex[i]] = tempBinds[i];
			}

			delete[] bindIndex;
		}
	}

	void setSkellyFromFile(const char* FileName)
	{
		// Load the model
		//PyramidModel *pPyramidModel = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) PyramidModel("pyramidModel.azul");
//		Model *pPyramidModel = ModelMan::find(ModelName::Pryamid);

		//assert(pPyramidModel);

		// Create/Load Shader 
		//ShaderObject *pShaderObject_textureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
//		ShaderObject* pShaderObject_textureLight = ShaderManager::addShader(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
	//	assert(pShaderObject_textureLight);

		// GraphicsObject for a specific instance
		//GraphicsObject_TextureLight *pGraphics_TextureLight;

		// Create GameObject
		Vect color(1.50f, 1.50f, 1.50f, 1.5f);
		Vect pos(-1.0f, -1.0f, -1.0f);

		PCSTree *pTree = GameObjectMan::GetPCSTree();
		//root = pTree->GetRoot();

		File::Handle fh;

		SkellyBone* boneArray;

		GameObjectAnim* pBObj;

		GameObjectAnim** GameObjectList = 0;

		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{

			unsigned int BoneCount = 0;

			File::Read(fh, &BoneCount, sizeof(int));

			//boneArray = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) SkellyBone[BoneCount];
			boneArray = new SkellyBone[BoneCount];
			//resultArray = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[BoneCount];
			resultArray = new Bone[BoneCount];
			//GameObjectList = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim*[BoneCount];
			GameObjectList = new GameObjectAnim*[BoneCount];

			//TEMP to debug vertex shader
			boneResultArray = new Matrix[BoneCount];

			//boneArray = new SkellyBone[BoneCount];
			//GameObjectList = new GameObjectAnim*[BoneCount];

			File::Read(fh, boneArray, BoneCount * sizeof(SkellyBone));

			//pGraphics_TextureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			//pBObj = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetHeap());

			//NullModel* pNullMod = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) NullModel(0);
			Model* pNullMod = ModelMan::find(ModelName::NullMod);
			assert(pNullMod);
			ShaderObject *pShader = ShaderManager::find(ShaderObject::ShaderName::NULL_SHADER);

			//GraphicsObject* pNullGraphics = new(ScratchSpace::GetGraphicsHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Null(pNullMod, pShader, ScratchSpace::GetHeap());
			GraphicsObject* pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);

			GameObjectRigid* pContainer = new GameObjectRigid(pNullGraphics);
			Skeleton.pHeriarchyRoot = pContainer;

			GameObjectMan::Add(Skeleton.pHeriarchyRoot, GameObjectMan::GetRoot());

			pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);
			//pShader = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			pBObj = new GameObjectAnim(pNullGraphics);

			pBObj->SetIndex(boneArray[0].index);
			pBObj->SetName(boneArray[0].name);
			pBObj->SetResult(resultArray);

			GameObjectMan::Add(pBObj, Skeleton.pHeriarchyRoot);

			GameObjectList[0] = pBObj;
			//pFirstBone = pBObj;
			//Skeleton.pHeriarchyRoot = pBObj;
			Skeleton.numBones = BoneCount;

			for (unsigned int i = 1; i < BoneCount; i++)
			{

				//pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos);
				pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);
				//pBObj = new(ScratchSpace::GetGOHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetGOManHeap());
				pBObj = new GameObjectAnim(pNullGraphics);

				pBObj->SetIndex(boneArray[i].index);
				pBObj->SetName(boneArray[i].name);
				pBObj->SetResult(resultArray);
				GameObjectMan::Add(pBObj, GameObjectList[boneArray[i].parentIndex]);

				GameObjectList[i] = pBObj;

			}

			File::Close(fh);

			delete[] boneArray;
			delete[] GameObjectList;

		}

		pTree->Print();

	}

	void nextAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void prevAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void SetAnimationPose(const Time&)
	{

		// REMEMBER:
		//    that the bone pose are WORLD not GLOBAL
		//    Animations are relative!

		// First thing, get the first frame of animation
		//Time tCurr(Time::ZERO);

		//ProcessAnimation(tCurr);

		//calls update on every object //TODO: optimize this away there has to be a better way.
		//GameObjectMan::Update(tCurr);

		PCSTreeForwardIterator pItr(Skeleton.pHeriarchyRoot);
		PCSNode *pNode = pItr.First();
		pNode = pItr.Next();
		GameObjectAnim *pGameObj = 0;

		//Trace::out("Pose start\n");
		int loopCount = 0;
		while (!pItr.IsDone())
		{
			assert(pNode);

			pGameObj = (GameObjectAnim *)pNode;

			//Trace::out("Updating Item #%i @addr %x ", loopCount, pGameObj);
			//pGameObj->PrintNode();

			setBonePose(pGameObj);
			//Trace::out("\n");

			pNode = pItr.Next();
			loopCount++;
		}

		pushBonesToGPU();

		//Trace::out("End of pose\n");
		//GameObject *pChildNode = (GameObject *)pNode->GetChild();
		//GameObjectAnim*pParentNode = (GameObjectAnim*)pNode;

		//walk_anim_node(pNode);

	}


	void setBonePose(GameObjectAnim*node)
	{
		// Now get the world matrices
		GameObjectAnim * childNode = (GameObjectAnim *)node;
		GameObjectAnim * parentNode;// = (GameObjectAnim *)node->GetParent();

									//if (parentNode == Skeleton.pHeriarchyRoot->GetChild() || parentNode == Skeleton.pHeriarchyRoot)
									//return;

									//if (node->GetParent()->GetParent()->GetParent() != 0)
									//	parentNode = (GameObjectAnim *)node->GetParent();
									//else
									//	parentNode = 0;

		if (node->GetParent()->GetParent() != 0)
			parentNode = (GameObjectAnim *)node->GetParent();
		else
			parentNode = 0;


		if (parentNode != 0 && childNode != 0)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions
			Vect ptA = start * *parentNode->GetWorld();
			Vect ptB = start * *childNode->GetWorld();


			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(SCALE, BONE_WIDTH, BONE_WIDTH, mag);
			//Quat   Q(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f) * *childNode->GetWorld());
			Quat   Q(ROT_ORIENT, dir.norm(), Vect(0.0f, 1.0f, 0.0f) * *parentNode->GetWorld());
			Matrix T(TRANS, ptB);
			Matrix BoneOrient = S * Q * T;

			//Matrix Spre(SCALE, Vect(0.15f, 0.15f, 0.15f, 0.15f));
			//Matrix S(SCALE, bResult->S);
			//Matrix R(bResult->Q);
			//Matrix T(TRANS, bResult->T);

			Bone tmp = resultArray[childNode->indexBoneArray];

			/*Matrix toInv = *childNode->pLocal;
			toInv.inv();*/

			boneResultArray[childNode->indexBoneArray] = (Matrix(SCALE, tmp.S) * tmp.Q * Matrix(TRANS, tmp.T) ** parentNode->GetWorld());

			childNode->SetBoneOrientation(BoneOrient);
		}
	}

	/*void addClip(Clip*)
	{

	}*/

	void addClipFromFile(const char* fileName)
	{
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, fileName, File::Mode::READ))
		{

			pCurClip = new Clip();

			addToActive((DLink&)*pCurClip);

			unsigned int frameCount = 0;
			unsigned int boneCount = 0;
			File::Read(fh, &frameCount, sizeof(unsigned int));
			File::Read(fh, &boneCount, sizeof(unsigned int));
			assert(frameCount != 0);
			assert(boneCount != 0);

			pCurClip->maxTime = ((int)frameCount) * Time(Time::NTSC_30_FRAME);

			// --------  Result Frame  ----------------------

			pCurClip->pHead = new Frame_Bucket();
			pCurClip->pHead->prevBucket = 0;
			pCurClip->pHead->nextBucket = 0;
			pCurClip->pHead->KeyTime = Time(Time::ZERO);
			//pCurClip->pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[boneCount];
			pCurClip->pHead->pBone = new Bone[boneCount];

			Frame_Bucket* prev = pCurClip->pHead;

			//HACKY signed unsigned mix here because there's no unsigned operator in the TIME lib yet.
			for (int i = 0; i < (int)frameCount; i++)
			{

				Frame_Bucket *pTmp = new Frame_Bucket();
				pTmp->prevBucket = prev;
				pTmp->nextBucket = 0;
				pTmp->KeyTime = i * Time(Time::NTSC_30_FRAME);
				//pTmp->pBone = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[boneCount];
				pTmp->pBone = new Bone[boneCount];
				prev->nextBucket = pTmp;

				prev = pTmp;

				File::Read(fh, pTmp->pBone, sizeof(Bone) * boneCount);

				for (unsigned int j = 0; j < boneCount; j++)
				{
					pTmp->pBone[j].S = Vect(1.0f, 1.0f, 1.0f);
				}

			}

			File::Close(fh);

			//processAnimHelper(Time(Time::ZERO), pCurClip);
		}
		else
		{
			assert(false);
		}
	}

	void FindMaxTime(Time &tMax)
	{
		Frame_Bucket *pTmp = pCurClip->pHead->nextBucket;

		while (pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;
	}

	void processAnimHelper(const Time& tCurr, Clip* pClip)
	{

		Frame_Bucket* pHead = pClip->pHead;
		// Note: these static should be tied into an animation system

		Frame_Bucket *pTmp = pHead->nextBucket;

		// Get the result bone array
		// Remember the firs Frame is the result
		//Bone *bResult = pHead->pBone;
		Bone *bResult = resultArray;

		// First one is the result, skip it
		pTmp = pHead->nextBucket;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		Frame_Bucket *pA = pTmp->prevBucket;
		Frame_Bucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		// interpolate to 
		Bone *bA = pA->pBone;
		Bone *bB = pB->pBone;

		// Interpolate to tS time, for all bones
		for (unsigned int i = 0; i < Skeleton.numBones; i++)
		{
			// interpolate ahoy!
			VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
			QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
			VectApp::Lerp(bResult->S, bA->S, bB->S, tS);

			//boneResultArray[i] = Matrix(TRANS,bResult->T) * Matrix(bResult->Q) * Matrix(SCALE, bResult->S);

			// advance the pointer
			bA++;
			bB++;
			bResult++;
		}
	}


	//void CProcessAnimation(const Time&)
	void CProcessAnimation(const float deltaT)
	{
		processAnimHelper(internalTime, pCurClip);

		//processAnimHelper(Time(Time::ZERO), pCurClip);

		Time deltaTime = deltaT * Time(Time::NTSC_30_FRAME);
		Time maxTime;

		SetAnimationPose(deltaTime);

		internalTime += deltaTime;

		FindMaxTime(maxTime);

		static const Time tZero(Time::ZERO);

		// protection for time values for looping
		if (internalTime < tZero)
		{
			internalTime = maxTime;
		}
		else if (internalTime > maxTime)
		{
			internalTime = tZero;
		}
		else
		{
			// do nothing
		}
	}

	DLink* dCreateNode()
	{
		DLink* newNode = (DLink*) new Clip();
		assert(newNode != nullptr);
		return newNode;
	}

	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		bool retval = false;
		if (((CameraLink*)&pLinkA)->GetName() == ((CameraLink*)&pLinkB)->GetName())
		{
			retval = true;
		}
		return retval;
	}

	void dClearNode(DLink* pLink)
	{
		pLink->dClean();
	}

};


class AnimController : public baseManager
{
public:

	bool pad1[4];
	Time    internalTime;
	Skelly Skeleton;
	Bone*   resultArray;
	Clip*   pCurClip;
	int		curAnims;


	bool pad2[4];
	//TEMP to work on the skinning
	//GameObject* pMesh;
	Matrix* boneResultArray;
	Matrix* bindPoseArray;


	AnimController()
		: internalTime(Time::ZERO), pCurClip(0)// pClipHead(0), pCurClip(0)
	{
	}

	AnimController(const AnimController&) = delete;
	AnimController& operator = (const AnimController&) = delete;

	~AnimController()
	{
		delete[] resultArray;
		delete[] boneResultArray;

		//delete  pMesh;
		delete[] bindPoseArray;
	}

	//must be skinned model IDK if I'm gonna differentiate yet or how.
	void setMesh(Model* pMdl, ShaderObject * pShaderObj, const char* bindPoseFile)
	{

		setBindPoseFromFile(bindPoseFile);

		GameObjectRigid *pGameObj;
		GraphicsObjectSkinLight  *pGraphics;

		Vect color(1.50f, 1.50f, 1.50f, 1.5f);
		Vect pos(0.0f, 100.0f, 100.0f);

		//Humanoid Skin
		pGraphics = new GraphicsObjectSkinLight(pMdl, pShaderObj, TextureName::Gray, color, pos);
		//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pMdl, pShaderObj, TextureName::Gray, color, pos, StageHeapPtr);

		pGraphics->SetBoneArray(boneResultArray, Skeleton.numBones);

		pGraphics->setBindArray(bindPoseArray, Skeleton.numBones);

		pGameObj = new GameObjectRigid(pGraphics);
		//pGameObj->SetPos(*Skeleton.pHeriarchyRoot->pPos);
		//pGameObj->SetScale(*Skeleton.pHeriarchyRoot->pScale);

		//pGameObj->SetPos(Vect(141.0f, -25.0f, 55.0f));
		//pGameObj->SetScale(Vect(1.0f, 1.0f, 1.0f));

		//Matrix RotX = Matrix(ROT_X, MATH_PI2);

		//*pGameObj->pQRot = Quat(RotX);

		GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());
	}

	void setSkelly(Skelly& _Skeleton)
	{
		this->Skeleton = _Skeleton;
	}

	void setBindPoseFromFile(const char* FileName)
	{
		bindPoseArray = new Matrix[Skeleton.numBones]();
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{
			unsigned int numBinds = 0;

			File::Read(fh, &numBinds, sizeof(int));

			int* bindIndex = new int[numBinds];
			Matrix* tempBinds = new Matrix[numBinds];

			File::Read(fh, bindIndex, numBinds * sizeof(int));
			File::Read(fh, tempBinds, numBinds * sizeof(float) * 16);

			for (unsigned int i = 0; i < numBinds; i++)
			{
				Trace::out("bind %i maps to bone %i\n", i, bindIndex[i]);
				bindPoseArray[bindIndex[i]] = tempBinds[i];
			}
			delete[] bindIndex;
		}
	}

	void setSkellyFromFile(const char* FileName)
	{
		// Load the model
		//PyramidModel *pPyramidModel = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) PyramidModel("pyramidModel.azul");
		Model *pPyramidModel = ModelMan::find(ModelName::Pryamid);

		assert(pPyramidModel);

		// Create/Load Shader 
		//ShaderObject *pShaderObject_textureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
		ShaderObject* pShaderObject_textureLight = ShaderManager::addShader(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
		assert(pShaderObject_textureLight);

		// GraphicsObject for a specific instance
		GraphicsObject_TextureLight *pGraphics_TextureLight;

		// Create GameObject
		Vect color(1.50f, 1.50f, 1.50f, 1.5f);
		Vect pos(-1.0f, -1.0f, -1.0f);

		PCSTree *pTree = GameObjectMan::GetPCSTree();
		//root = pTree->GetRoot();

		File::Handle fh;

		SkellyBone* boneArray;

		GameObjectAnim* pBObj;

		GameObjectAnim** GameObjectList = 0;

		if (File::Error::SUCCESS == File::Open(fh, FileName, File::Mode::READ))
		{

			unsigned int BoneCount = 0;

			File::Read(fh, &BoneCount, sizeof(int));

			//boneArray = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) SkellyBone[BoneCount];
			boneArray = new SkellyBone[BoneCount + 1];
			//resultArray = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[BoneCount];
			resultArray = new Bone[BoneCount + 1];
			//GameObjectList = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim*[BoneCount];
			GameObjectList = new GameObjectAnim*[BoneCount + 1];

			//TEMP to debug vertex shader
			boneResultArray = new Matrix[BoneCount + 1];

			//boneArray = new SkellyBone[BoneCount];
			//GameObjectList = new GameObjectAnim*[BoneCount];

			File::Read(fh, boneArray, BoneCount * sizeof(SkellyBone));

			//pGraphics_TextureLight = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			//pBObj = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetHeap());

			//NullModel* pNullMod = new(ScratchSpace::GetHeap3(), Mem::Align::Byte_16, __FILE__, __LINE__) NullModel(0);
			Model* pNullMod = ModelMan::find(ModelName::NullMod);
			assert(pNullMod);
			ShaderObject *pShader = ShaderManager::find(ShaderObject::ShaderName::NULL_SHADER);

			//GraphicsObject* pNullGraphics = new(ScratchSpace::GetGraphicsHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Null(pNullMod, pShader, ScratchSpace::GetHeap());
			GraphicsObject* pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);

			GameObjectRigid* pContainer = new GameObjectRigid(pNullGraphics);
			Skeleton.pHeriarchyRoot = pContainer;

			GameObjectMan::Add(Skeleton.pHeriarchyRoot, GameObjectMan::GetRoot());

			pNullGraphics = new GraphicsObject_Null(pNullMod, pShader);
			//pShader = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, ScratchSpace::GetHeap());
			pBObj = new GameObjectAnim(pNullGraphics);

			pBObj->SetIndex(boneArray[0].index);
			pBObj->SetName(boneArray[0].name);
			pBObj->SetResult(resultArray);

			GameObjectMan::Add(pBObj, Skeleton.pHeriarchyRoot);

			GameObjectList[0] = pBObj;
			//pFirstBone = pBObj;
			//Skeleton.pHeriarchyRoot = pBObj;
			Skeleton.numBones = BoneCount;

			for (unsigned int i = 1; i < BoneCount; i++)
			{

				pGraphics_TextureLight = new GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos);
				//pBObj = new(ScratchSpace::GetGOHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, ScratchSpace::GetGOManHeap());
				pBObj = new GameObjectAnim(pGraphics_TextureLight);

				pBObj->SetIndex(boneArray[i].index);
				pBObj->SetName(boneArray[i].name);
				pBObj->SetResult(resultArray);
				GameObjectMan::Add(pBObj, GameObjectList[boneArray[i].parentIndex]);

				GameObjectList[i] = pBObj;

			}

			File::Close(fh);

			delete[] boneArray;
			delete[] GameObjectList;

		}

		pTree->Print();
	}

	void nextAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void prevAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (Clip*)pActiveHead;
		}
		else
		{
			pCurClip = (Clip*)pCurClip->pNext;
		}
	}

	void SetAnimationPose(const Time&)
	{

		// REMEMBER:
		//    that the bone pose are WORLD not GLOBAL
		//    Animations are relative!

		// First thing, get the first frame of animation
		//Time tCurr(Time::ZERO);

		//ProcessAnimation(tCurr);

		//calls update on every object //TODO: optimize this away there has to be a better way.
		//GameObjectMan::Update(tCurr);

		PCSTreeForwardIterator pItr(Skeleton.pHeriarchyRoot);
		PCSNode *pNode = pItr.First();
		pNode = pItr.Next();
		GameObjectAnim *pGameObj = 0;

		//Trace::out("Pose start\n");
		int loopCount = 0;
		while (!pItr.IsDone())
		{
			assert(pNode);

			pGameObj = (GameObjectAnim *)pNode;

			//Trace::out("Updating Item #%i @addr %x ", loopCount, pGameObj);
			//pGameObj->PrintNode();

			setBonePose(pGameObj);
			//Trace::out("\n");

			pNode = pItr.Next();
			loopCount++;
		}

		//Trace::out("End of pose\n");
		//GameObject *pChildNode = (GameObject *)pNode->GetChild();
		//GameObjectAnim*pParentNode = (GameObjectAnim*)pNode;

		//walk_anim_node(pNode);

	}


	void setBonePose(GameObjectAnim*node)
	{
		// Now get the world matrices
		GameObjectAnim * childNode = (GameObjectAnim *)node;
		GameObjectAnim * parentNode;// = (GameObjectAnim *)node->GetParent();

		//if (parentNode == Skeleton.pHeriarchyRoot->GetChild() || parentNode == Skeleton.pHeriarchyRoot)
			//return;

		//if (node->GetParent()->GetParent()->GetParent() != 0)
		//	parentNode = (GameObjectAnim *)node->GetParent();
		//else
		//	parentNode = 0;

		if (node->GetParent()->GetParent() != 0)
			parentNode = (GameObjectAnim *)node->GetParent();
		else
			parentNode = 0;


		if (parentNode != 0 && childNode != 0)
		{
			// starting point
			Vect start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//  Now get the length and directions
			Vect ptA = start * *parentNode->GetWorld();
			Vect ptB = start * *childNode->GetWorld();


			// direction between the anchor points of the respective bones
			Vect dir = (ptA - ptB);

			// length of the bone 0
			float mag = dir.mag();

			Matrix S(SCALE, BONE_WIDTH, BONE_WIDTH, mag);
			//Quat   Q(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f) * *childNode->GetWorld());
			Quat   Q(ROT_ORIENT, dir.norm(), Vect(0.0f, 1.0f, 0.0f) * *parentNode->GetWorld());
			Matrix T(TRANS, ptB);
			Matrix BoneOrient = S * Q * T;

			//Matrix Spre(SCALE, Vect(0.15f, 0.15f, 0.15f, 0.15f));
			//Matrix S(SCALE, bResult->S);
			//Matrix R(bResult->Q);
			//Matrix T(TRANS, bResult->T);

			Bone tmp = resultArray[childNode->indexBoneArray];

			/*Matrix toInv = *childNode->pLocal;
			toInv.inv();*/

			boneResultArray[childNode->indexBoneArray] = (Matrix(SCALE, tmp.S) * tmp.Q * Matrix(TRANS, tmp.T) ** parentNode->GetWorld());

			childNode->SetBoneOrientation(BoneOrient);



		}
	}


	/*void addClip(Clip*)
	{

	}*/

	void addClipFromFile(const char* fileName)
	{
		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, fileName, File::Mode::READ))
		{

			pCurClip = new Clip();

			addToActive((DLink&)*pCurClip);

			unsigned int frameCount = 0;
			unsigned int boneCount = 0;
			File::Read(fh, &frameCount, sizeof(unsigned int));
			File::Read(fh, &boneCount, sizeof(unsigned int));
			assert(frameCount != 0);
			assert(boneCount != 0);

			pCurClip->maxTime = ((int)frameCount) * Time(Time::NTSC_30_FRAME);

			// --------  Result Frame  ----------------------

			pCurClip->pHead = new Frame_Bucket();
			pCurClip->pHead->prevBucket = 0;
			pCurClip->pHead->nextBucket = 0;
			pCurClip->pHead->KeyTime = Time(Time::ZERO);
			//pCurClip->pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[boneCount];
			pCurClip->pHead->pBone = new Bone[boneCount];

			Frame_Bucket* prev = pCurClip->pHead;

			//HACKY signed unsigned mix here because there's no unsigned operator in the TIME lib yet.
			for (int i = 0; i < (int)frameCount; i++)
			{

				Frame_Bucket *pTmp = new Frame_Bucket();
				pTmp->prevBucket = prev;
				pTmp->nextBucket = 0;
				pTmp->KeyTime = i * Time(Time::NTSC_30_FRAME);
				//pTmp->pBone = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[boneCount];
				pTmp->pBone = new Bone[boneCount];
				prev->nextBucket = pTmp;

				prev = pTmp;

				File::Read(fh, pTmp->pBone, sizeof(Bone) * boneCount);

				for (unsigned int j = 0; j < boneCount; j++)
				{
					pTmp->pBone[j].S = Vect(1.0f, 1.0f, 1.0f);
				}

			}

			File::Close(fh);

			//processAnimHelper(Time(Time::ZERO), pCurClip);
		}
		else
		{
			assert(false);
		}
	}

	void FindMaxTime(Time &tMax)
	{
		Frame_Bucket *pTmp = pCurClip->pHead->nextBucket;

		while (pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;
	}

	void processAnimHelper(const Time& tCurr, Clip* pClip)
	{

		Frame_Bucket* pHead = pClip->pHead;
		// Note: these static should be tied into an animation system

		Frame_Bucket *pTmp = pHead->nextBucket;

		// Get the result bone array
		// Remember the firs Frame is the result
		//Bone *bResult = pHead->pBone;
		Bone *bResult = resultArray;

		// First one is the result, skip it
		pTmp = pHead->nextBucket;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		Frame_Bucket *pA = pTmp->prevBucket;
		Frame_Bucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		// interpolate to 
		Bone *bA = pA->pBone;
		Bone *bB = pB->pBone;

		// Interpolate to tS time, for all bones
		for (unsigned int i = 0; i < Skeleton.numBones; i++)
		{
			// interpolate ahoy!
			VectApp::Lerp(bResult->T, bA->T, bB->T, tS);
			QuatApp::Slerp(bResult->Q, bA->Q, bB->Q, tS);
			VectApp::Lerp(bResult->S, bA->S, bB->S, tS);

			//boneResultArray[i] = Matrix(TRANS,bResult->T) * Matrix(bResult->Q) * Matrix(SCALE, bResult->S);


			// advance the pointer
			bA++;
			bB++;
			bResult++;
		}
	}


	//void CProcessAnimation(const Time&)
	void CProcessAnimation(const float deltaT)
	{

		processAnimHelper(internalTime, pCurClip);

		//processAnimHelper(Time(Time::ZERO), pCurClip);

		Time deltaTime = deltaT * Time(Time::NTSC_30_FRAME);
		Time maxTime;

		SetAnimationPose(deltaTime);

		internalTime += deltaTime;

		FindMaxTime(maxTime);

		static const Time tZero(Time::ZERO);

		// protection for time values for looping
		if (internalTime < tZero)
		{
			internalTime = maxTime;
		}
		else if (internalTime > maxTime)
		{
			internalTime = tZero;
		}
		else
		{
			// do nothing
		}

	}

	DLink* dCreateNode()
	{
		DLink* newNode = (DLink*) new Clip();
		assert(newNode != nullptr);
		return newNode;
	}

	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		bool retval = false;
		if (((CameraLink*)&pLinkA)->GetName() == ((CameraLink*)&pLinkB)->GetName())
		{
			retval = true;
		}

		return retval;

	}

	void dClearNode(DLink* pLink)
	{
		pLink->dClean();
	}

};
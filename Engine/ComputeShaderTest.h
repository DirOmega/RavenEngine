#pragma once

#include "ComputeShaderObject.h"
#include "MathEngine.h"
#include "NullModel.h"

class ComputeShaderTest
{
public:

	ComputeShaderTest(unsigned int numVerts, Model* pMdl)
	{

		pObj = new ComputeShaderObject(ShaderObject::ShaderName::ShapeDefoCompute, "ComputeShader");

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
		storedNumVerts = numVerts * 2;

		glGenBuffers(1, &ssboIn);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboIn);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, basisArray, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssboIn);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		glGenBuffers(1, &ssboNumVerts);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboNumVerts);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(unsigned int), &storedNumVerts, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssboNumVerts);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		Vert_xyz* testArray = new Vert_xyz[numVerts];

		glGenBuffers(1, &ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboOut);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr) sizeof(Vert_xyz)*(int)numVerts, testArray, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboOut);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		delete[] testArray;
		//glGenBuffers(1, &atomicCouter);
		//// bind the buffer and define its initial storage capacity
		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCouter);
		//glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint) * 3, NULL, GL_DYNAMIC_DRAW);
		//// unbind the buffer 
		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

		//GLuint block_index = 0;
		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "outputMesh");
		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 0);
		//
		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "data");
		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 1);
		//
		//block_index = glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "inputMesh");
		//glShaderStorageBlockBinding(pObj->getProgramObject(), block_index, 2);


		delete[] basisArray;

	}

	~ComputeShaderTest()
	{
		glDeleteBuffers(1, &ssboIn);
		glDeleteBuffers(1, &ssboOut);
		glDeleteBuffers(1, &ssboNumVerts);
	}

	void Dispatch()
	{
		glUseProgram(pObj->getProgramObject());
		//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 16, ShapeWeights, GL_DYNAMIC_DRAW);

		glDispatchCompute((GLuint)storedNumVerts / 128, 1, 1); //32 is from the compute shader for work sze
															   //glMemoryBarrier(GL_ALL_BARRIER_BITS);
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		//GLuint *userCounters;
		//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomicCouter);
		//// again we map the buffer to userCounters, but this time for read-only access
		//userCounters = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER,
		//	0,
		//	sizeof(GLuint) * 3,
		//	GL_MAP_READ_BIT
		//);
		//// copy the values to other variables because...
		//unsigned int bufferCount = userCounters[0];
		//// ... as soon as we unmap the buffer
		//// the pointer userCounters becomes invalid.
		//glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
		//
		//Trace::out("Atomic Count at %i\n", bufferCount);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, countBuffers);
		//GLuint *ptr = (GLuint*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
		//GLuint vertCount = ptr[0];
		//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		//Trace::out("VertCount = %i", vertCount);

	}

	GLuint getOutputStorageBlock()
	{
		return glGetProgramResourceIndex(pObj->getProgramObject(), GL_SHADER_STORAGE_BLOCK, "outputMesh");
	}

	GLuint atomicCouter;
	GLuint ssboIn;
	GLuint ssboOut;
	GLuint ssboNumVerts;
	unsigned int storedNumVerts;
	ComputeShaderObject* pObj;

};
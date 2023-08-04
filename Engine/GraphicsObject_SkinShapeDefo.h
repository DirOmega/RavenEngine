#pragma once

#include "GraphicsObject.h"
#include "TextureMan.h"

//class Game;

class GraphicsObject_SkinShapeDefo :public GraphicsObject
{
public:

	GraphicsObject_SkinShapeDefo(Model *model, ShaderObject *pShaderObj, TextureName name, bool transparent=false);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;


	void SetBoneArray(Matrix* ptr, unsigned int size)
	{
		pBoneArray = ptr;
		boneArraySize = size;
	}

	//TODO: set this up to only go once. kinda irellavant as it's all going on the gpu eventually
	void setBindArray(Matrix* bindPoseArray, unsigned int size)
	{
		pBindArray = bindPoseArray;
		boneArraySize = size;
	}

	//void setWeightsArray(float* weightsArray, unsigned int size)
	//{
	//	shapeweightsArray = weightsArray;
	//	shapeweightsArraySize = size;
	//	//glUniform1i(pShaderObj->GetLocation("weightcount"), (int)size);
	//}

	//bad cuz It breaks decoupling but I'm using it for debugging my compute shaders
	GLuint getProgramObj()
	{
		return this->pShaderObj->getProgramObject();
	}

	GLint getProgramLoc(const char* locName)
	{
		return pShaderObj->GetLocation(locName);
	}

	// data:  place uniform instancing here
	//Game* pGame;
	Matrix* pBoneArray;
	Matrix* pBindArray;
	unsigned int boneArraySize;
	
	//unsigned int shapeweightsArraySize;
	//float* shapeweightsArray;
	

	TextureName textName;
	GLuint textureID;
	GLint projMatrix;

	GLint boneArray;
	GLuint bindArray;
	GLint world_matrix;
	GLint view_matrix;
	GLint ShapeWeightsArray;
	bool transparent;
	bool pad[3];
	//GLint mvMatrix;

};

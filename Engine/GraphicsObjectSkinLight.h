#pragma once

#include "GraphicsObject.h"
#include "TextureMan.h"

//class Game;

class GraphicsObjectSkinLight :public GraphicsObject
{
public:

	GraphicsObjectSkinLight(Model *model, ShaderObject *_pShaderObj, TextureName name, Vect &LightColor, Vect&LightPos);

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

	void disableBFC()
	{
		bfc = false;
	}

	void enableBFC()
	{
		bfc = true;
	}

	//TODO: set this up to only go once. kinda irellavant as it's all going on the gpu eventually
	void setBindArray(Matrix* bindPoseArray, unsigned int size)
	{
		pBindArray = bindPoseArray;
		boneArraySize = size;
	}

	// data:  place uniform instancing here
	//Game* pGame;
	Matrix* pBoneArray;
	Matrix* pBindArray;
	unsigned int boneArraySize;

	TextureName textName;
	GLuint textureID;
	GLint projMatrix;

	GLint boneArray;
	GLuint bindArray;
	//GLint world_matrix;
	GLint view_matrix;
	//GLint mvMatrix;
	GLuint vLightPos;
	GLuint lightUBO;

	GLint vColor;
	Vect *pLightColor;
	Vect *pLightPos;


	Vect* t;
	bool bfc;
	bool pad[3];
};

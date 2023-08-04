#pragma once

#include "GraphicsObject.h"
#include "TextureMan.h"

//class Game;

class GraphicsObject_FlatTexture :public GraphicsObject
{
public:
	GraphicsObject_FlatTexture(Model *model, ShaderObject *pShaderObj, TextureName name);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	//Game* pGame;
	TextureName textName;
	GLuint textureID;
	GLint projMatrix;
	GLint world_matrix;
	GLint view_matrix;
	//GLint mvMatrix;

};

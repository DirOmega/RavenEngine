#pragma once

#include "GraphicsObject.h"

//class Game;

class GraphicsObject_ColorByPosition : public GraphicsObject
{
public:
	GraphicsObject_ColorByPosition(Model *model, ShaderObject *pShaderObj);

	//Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	//data: place uniform instance data here
	//TODO:: delete game ptr;
	//Game* pGame;
	GLint projMatrix;
	GLint world_matrix;
	GLint view_matrix;
	//GLint mvMatrix;
};
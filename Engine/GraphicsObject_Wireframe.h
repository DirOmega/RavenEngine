#pragma once

#include "GraphicsObject.h"

class Game;

class GraphicsObject_Wireframe :public GraphicsObject
{
public:
	GraphicsObject_Wireframe(Model *pModel, ShaderObject *pShaderObj);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	Game* pGame; //TODO: refactor the backpointer to the game needed until I can get a camera class working.
	GLint projMatrix;
	GLint world_matrix;
	GLint view_matrix;
	//GLint mvMatrix;

};
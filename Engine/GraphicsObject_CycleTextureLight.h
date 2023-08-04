#pragma once

#include "GraphicsObject.h"
#include "TextureMan.h"

class GraphicsObject_CycleTextureLight : public GraphicsObject
{
public:

	GraphicsObject_CycleTextureLight(Model *model, ShaderObject *pShaderObj, TextureName  name0, TextureName name1, TextureName name2, TextureName name3, Vect &LightColor, Vect &LightPos);
	GraphicsObject_CycleTextureLight() = delete;
	~GraphicsObject_CycleTextureLight();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

	//TextureName textName;
	int cycleTime;
	GLuint currText;
	GLuint textureID0;
	GLuint textureID1;
	GLuint textureID2;
	GLuint textureID3;
	GLint projMatrix;
	GLint world_matrix;
	GLint view_matrix;
	GLint vLightPos;
	GLint vColor;
	Vect *pLightColor;
	Vect *pLightPos;

};

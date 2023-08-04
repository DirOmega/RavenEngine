#pragma once

//todo: buy the memory engine stuff in a manager
#include "MathEngine.h"

#include "ShaderObject.h"

class Model;

class RenderMaterial
{
protected:
	//RenderMaterial Contract that all graphics objects must fulfill
	//virtual ~RenderMaterial();
	virtual	void SetState() = 0;
	virtual void SetDataGPU() = 0;
	virtual void Draw() = 0;
	virtual void RestoreState() = 0;
};

class GraphicsObject : private RenderMaterial
{
public:

	GraphicsObject(Model *pModel, ShaderObject *pShaderObj);

	
	~GraphicsObject();

	void deleteShader()
	{
		//delete pShaderObj;
	}
	void Render();
	void SetWorld(Matrix &_world);

	Model *GetModel() const;
	Matrix &GetWorld();

protected:

	GraphicsObject() = delete;

	Model		 *pModel;
	Matrix		 *pWorld;
	ShaderObject *pShaderObj;

};
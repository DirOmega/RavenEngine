#include <assert.h>

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Model.h"


GraphicsObject::GraphicsObject(Model* _pModel, ShaderObject* _pShaderObj)
	: pModel(_pModel), pShaderObj(_pShaderObj)
{	

	//this->pWorld = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Matrix(MatrixSpecialType::IDENTITY); 
	//TODO:: double check that I'm not leaking this matrix because I want to update the pointer inside this object and not assign the matrix every time

	this->pWorld = new Matrix(MatrixSpecialType::IDENTITY);

	assert(_pModel);
	assert(_pShaderObj);
}

GraphicsObject::~GraphicsObject()
{
	delete this->pWorld;
	//can't delete shader or Model as they are flyweighted
}

void GraphicsObject::Render()
{
	this->SetState();
	this->SetDataGPU();
	this->Draw();
	this->RestoreState(); //TODO:: bring restore state into base class so the default state can be set in one location for all graphics objects
}

Model *GraphicsObject::GetModel() const
{
	return this->pModel;
}

Matrix& GraphicsObject::GetWorld()
{
	return *this->pWorld;
}

void GraphicsObject::SetWorld(Matrix &_world)
{
	*this->pWorld = _world;
}
#include <assert.h>
#include "MathEngine.h"
#include "Model.h"
#include "GraphicsObject_ColorByPosition.h"
#include "Game.h"
#include "CameraManager.h" //TODO: decouple from camera and get the projection matrix from a shader manager or something

GraphicsObject_ColorByPosition::GraphicsObject_ColorByPosition(Model* _pModel, ShaderObject* _pShaderObj)
	: GraphicsObject(_pModel, _pShaderObj),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	world_matrix(_pShaderObj->GetLocation("world_matrix")),
	view_matrix(_pShaderObj->GetLocation("view_matrix"))
{
}

//TODO:: investigate polygon modes I think if I do GL_Front I get backface culling for free via the GPU
void GraphicsObject_ColorByPosition::SetState()
{
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CW);
}

void GraphicsObject_ColorByPosition::SetDataGPU()
{
	// set the vao

	glBindVertexArray(this->GetModel()->vao);

	// use this shader
	this->pShaderObj->SetActive();

	//TODO:: refactor out the world temp and also put the camera matrix grabs directly into the uniform calls
	/*Matrix world = this->GetWorld();
	Matrix view = CameraManager::GetActiveCamera()->getViewMatrix();
	Matrix proj = CameraManager::GetActiveCamera()->getProjMatrix();*/
	
	//world *= *CameraManager::GetActiveCamera()->getViewMatrix();
	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *) &CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *) &CameraManager::GetActiveCamera()->getViewMatrix());
	glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *) &this->GetWorld());
}

void GraphicsObject_ColorByPosition::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_ColorByPosition::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}
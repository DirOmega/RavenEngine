#include "GraphicsObject_Wireframe.h"

#include <assert.h>

#include "Model.h"
#include "ShaderObject.h"
#include "Game.h"
#include "CameraManager.h"

GraphicsObject_Wireframe::GraphicsObject_Wireframe(Model *pModel, ShaderObject *_pShaderObj)
	: GraphicsObject(pModel, _pShaderObj),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")), 
	world_matrix(_pShaderObj->GetLocation("world_matrix")),
	view_matrix(_pShaderObj->GetLocation("view_matrix"))

{
	assert(pModel);
	assert(_pShaderObj);
}

void GraphicsObject_Wireframe::SetState()
{
	// Set State
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
}

void GraphicsObject_Wireframe::SetDataGPU()
{
	// Draw Me: -----------------------------------------

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// Set state
	
	// use this shader
	this->pShaderObj->SetActive();

	/*Matrix world = this->GetWorld();
	Matrix view = CameraManager::GetActiveCamera()->getViewMatrix();
	Matrix proj = CameraManager::GetActiveCamera()->getProjMatrix();
	Matrix mvMat = world * view;*/
	//world *= *CameraManager::GetActiveCamera()->getViewMatrix();

	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());
	glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());
}

void GraphicsObject_Wireframe::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_Wireframe::RestoreState()
{
	// Restore state
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

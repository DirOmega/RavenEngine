#include <assert.h>
#include "GraphicsObject_TextureLight.h"
#include "Model.h"
#include "Texture.h"
#include "CameraManager.h"

//extern Camera *pCam;

GraphicsObject_TextureLight::GraphicsObject_TextureLight(Model *model, ShaderObject *_pShaderObj, TextureName name, Vect &LightColor, Vect &LightPos)
	: GraphicsObject(model, _pShaderObj),
	textName(name),
	textureID(TextureMan::find(textName)),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	world_matrix(_pShaderObj->GetLocation("world_matrix")),
	view_matrix(_pShaderObj->GetLocation("view_matrix")),
	vLightPos(_pShaderObj->GetLocation("vLightPos")),
	vColor(_pShaderObj->GetLocation("vColor"))
{
	//this->pLightColor = new(_pHeap, Mem::Align::Byte_16,__FILE__,__LINE__)   Vect(LightColor);
	//this->pLightPos   = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Vect(LightPos);

	this->pLightColor = new   Vect(LightColor);
	this->pLightPos = new Vect(LightPos);
}

GraphicsObject_TextureLight::~GraphicsObject_TextureLight()
{
	delete this->pLightColor;
	delete this->pLightPos;
}

void GraphicsObject_TextureLight::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void GraphicsObject_TextureLight::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());
	glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());

	glUniform3fv(this->vLightPos, 1, (float *)this->pLightPos);
	glUniform4fv(this->vColor, 1, (float *)this->pLightColor);
}

void GraphicsObject_TextureLight::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_TextureLight::RestoreState()
{
	// undo the states you corrupted
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

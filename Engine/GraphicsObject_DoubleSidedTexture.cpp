#include <assert.h>

#include "GraphicsObject_DoubleSidedTexture.h"
#include "TextureMan.h"
#include "Model.h"
#include "Game.h"
#include "CameraManager.h"

//extern Matrix *pProjectionMatrix;

GraphicsObject_DoubleSidedTexture::GraphicsObject_DoubleSidedTexture(Model *model, ShaderObject *_pShaderObj, TextureName name)
	: GraphicsObject(model, _pShaderObj),
	textName(name),
	textureID(TextureMan::find(textName)),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	boneArray(_pShaderObj->GetLocation("boneMats")),
	bindArray(_pShaderObj->GetBufferLocation("bindArray")),
	view_matrix(_pShaderObj->GetLocation("view_matrix"))
{
	pBoneArray = nullptr;
	assert(pModel);
	assert(_pShaderObj);
}

void GraphicsObject_DoubleSidedTexture::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	//GLuint textureID = TextureMan::find(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void GraphicsObject_DoubleSidedTexture::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();


	assert(pBoneArray);
	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	/*Matrix world = this->GetWorld();
	Matrix view = CameraManager::GetActiveCamera()->getViewMatrix();
	Matrix proj = CameraManager::GetActiveCamera()->getProjMatrix();*/
	//Matrix mvMat = view * world;
	//Matrix mvMat = world * view;
	//world *= *CameraManager::GetActiveCamera()->getViewMatrix();

	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());
	glUniformMatrix4fv(this->boneArray, 200, GL_FALSE, (float *)pBoneArray); //TODO:: offset all bones by 1 to accomodate the game root

																			 //	glUniformMatrix4fv(this->bindArray, 256, GL_FALSE, (float *)pBindArray);

																			 //glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());
}

void GraphicsObject_DoubleSidedTexture::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_DoubleSidedTexture::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

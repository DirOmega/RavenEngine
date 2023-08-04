#include <assert.h>

#include "GraphicsObject_SkinShapeDefo.h"
#include "TextureMan.h"
#include "Model.h"
#include "Game.h"
#include "CameraManager.h"

//extern Matrix *pProjectionMatrix;

GraphicsObject_SkinShapeDefo::GraphicsObject_SkinShapeDefo(Model *model, ShaderObject *_pShaderObj, TextureName name, bool transparent)
	: GraphicsObject(model, _pShaderObj),
	textName(name),
	textureID(TextureMan::find(textName)),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	boneArray(_pShaderObj->GetLocation("boneMats")),
	bindArray(_pShaderObj->GetBufferLocation("bindArray")),
	view_matrix(_pShaderObj->GetLocation("view_matrix")),
	transparent(transparent)
	//ShapeWeightsArray(_pShaderObj->GetLocation("shapeWeights"))
{
	pBoneArray = nullptr;
	assert(pModel);
	assert(_pShaderObj);
}

void GraphicsObject_SkinShapeDefo::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	//GLuint textureID = TextureMan::find(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glBlendFunc(GL_FUNC_ADD, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);

	//if (transparent)
	//{
	//	glDisable(GL_DEPTH_TEST);
	//}
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//glBlendFunc(GL_SRC_COLOR, GL_FUNC_ADD);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void GraphicsObject_SkinShapeDefo::SetDataGPU()
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


	//glUniform1fv(this->ShapeWeightsArray, 16, shapeweightsArray);

	//Trace::out("ShapeWeights array in GO = %f %f %f\n", shapeweightsArray[0], shapeweightsArray[1], shapeweightsArray[2]);

	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());
	glUniformMatrix4fv(this->boneArray, 200, GL_FALSE, (float *)pBoneArray); //TODO:: offset all bones by 1 to accomodate the game root
																	 //glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());
}

void GraphicsObject_SkinShapeDefo::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_SkinShapeDefo::RestoreState()
{
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
}

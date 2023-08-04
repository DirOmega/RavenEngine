#include <assert.h>

#include "GraphicsObjectSkinLight.h"
#include "TextureMan.h"
#include "Model.h"
#include "Game.h"
#include "CameraManager.h"

//extern Matrix *pProjectionMatrix;

GraphicsObjectSkinLight::GraphicsObjectSkinLight(Model *model, ShaderObject *_pShaderObj, TextureName name,Vect &LightColor, Vect&LightPos)
	: GraphicsObject(model, _pShaderObj),
	textName(name),
	textureID(TextureMan::find(textName)),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	boneArray(_pShaderObj->GetLocation("boneMats")),
	//boneArray(_pShaderObj->GetBufferLocation("boneArray")),
	bindArray(_pShaderObj->GetBufferLocation("bindArray")),
	//boneArray(_pShaderObj->GetLocation("boneMats")),
	//bindArray(_pShaderObj->GetLocation("bindPose")),
	view_matrix(_pShaderObj->GetLocation("view_matrix")),
//	vLightPos(_pShaderObj->GetLocation("vLightPos")),
	vLightPos(_pShaderObj->GetBufferLocation("light"))
//	vColor(_pShaderObj->GetLocation("vColor"))

{
	pBoneArray = nullptr;
	assert(pModel);
	bfc = true;
	assert(_pShaderObj);

	//this->GetModel()->vao;

	this->pLightColor = new Vect(LightColor);
	this->pLightPos   = new Vect(LightPos);

	t = new Vect[2];
	t[0] = *pLightColor;
	t[1] = *pLightPos;

	glGenBuffers(1, &lightUBO);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(Vect), pLightPos, GL_STATIC_DRAW);
//	glBindBuffer(GL_UNIFORM_BUFFER, lightUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUBO);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(Vect) * 2, t, GL_STATIC_DRAW);

	glUniformBlockBinding(pShaderObj->getProgramObject(), vLightPos, 1);

}

void GraphicsObjectSkinLight::SetState()
{
	//glActiveTexture(GL_TEXTURE0);

	//// Bind the texture
	////GLuint textureID = TextureMan::find(this->textName);
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//if (bfc)
	//{
	//	glEnable(GL_CULL_FACE);
	//}
	//else
	//{
	//	glDisable(GL_CULL_FACE);
	//}

	//
	//glFrontFace(GL_CCW);

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
	//glEnable(GL_CULL_FACE);

	if (bfc)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}


	glFrontFace(GL_CCW);

}

void GraphicsObjectSkinLight::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();


	assert(pBoneArray);
	assert(pBindArray);
	// set the vao
	glBindVertexArray(this->GetModel()->vao);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightUBO);
	/*Matrix world = this->GetWorld();
	Matrix view = CameraManager::GetActiveCamera()->getViewMatrix();
	Matrix proj = CameraManager::GetActiveCamera()->getProjMatrix();*/
	//Matrix mvMat = view * world;
	//Matrix mvMat = world * view;
	//world *= *CameraManager::GetActiveCamera()->getViewMatrix();

	glUniformMatrix4fv(this->projMatrix,  1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());

	glUniformMatrix4fv(this->boneArray, 200, GL_FALSE, (float *)pBoneArray); //TODO:: offset all bones by 1 to accomodate the game root

	//glUniformMatrix4fv(this->bindArray, 80, GL_FALSE, (float *)pBindArray);

	//glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());

	//glUniform3fv(this->vLightPos, 1, (float *)this->pLightPos);
	//glUniform4fv(this->vColor, 1, (float *)this->pLightColor);

	/*glBindBuffer(GL_UNIFORM_BUFFER, bindArray);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, pBindArray, sizeof(Matrix)*80); 
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	
	glBindBuffer(GL_UNIFORM_BUFFER, boneArray);
	p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, pBoneArray, sizeof(Matrix) * 80);
	glUnmapBuffer(GL_UNIFORM_BUFFER);*/

}

void GraphicsObjectSkinLight::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObjectSkinLight::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

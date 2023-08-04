#include <assert.h>

#include "GraphicsObject_CycleTextureLight.h"
#include "Model.h"
#include "Texture.h"
#include "CameraManager.h"

//extern Camera *pCam;


GraphicsObject_CycleTextureLight::GraphicsObject_CycleTextureLight(Model *model, ShaderObject *_pShaderObj, TextureName  name0, TextureName name1, TextureName name2, TextureName name3, Vect &LightColor, Vect &LightPos)
	: GraphicsObject(model, _pShaderObj),
	//textName(name),
	textureID0(TextureMan::find(name0)),
	textureID1(TextureMan::find(name1)),
	textureID2(TextureMan::find(name2)),
	textureID3(TextureMan::find(name3)),
	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
	world_matrix(_pShaderObj->GetLocation("world_matrix")),
	view_matrix(_pShaderObj->GetLocation("view_matrix")),
	vLightPos(_pShaderObj->GetLocation("vLightPos")),
	vColor(_pShaderObj->GetLocation("vColor"))
{
	cycleTime = 240;
	currText = textureID0;
	/*this->pLightColor = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Vect(LightColor);
	this->pLightPos   = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Vect(LightPos);*/

	this->pLightColor = new Vect(LightColor);
	this->pLightPos = new Vect(LightPos);
}

GraphicsObject_CycleTextureLight::~GraphicsObject_CycleTextureLight()
{
	delete this->pLightColor;
	delete this->pLightPos;
}

void GraphicsObject_CycleTextureLight::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	//TODO:: remove all this crap it's just for the demo
	cycleTime--;
	if (cycleTime <= 0)
	{
		cycleTime = 240;
		
		if (currText == textureID0)
		{
			currText = textureID1;
		}
		else if (currText == textureID1)
		{
			currText = textureID2;
		}
		else if (currText == textureID2)
		{
			currText = textureID3;
		}
		else if (currText == textureID3)
		{
			currText = textureID0;
		}
	}

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, currText);

	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_CycleTextureLight::SetDataGPU()
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

void GraphicsObject_CycleTextureLight::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_CycleTextureLight::RestoreState()
{
	// undo the states you corrupted
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

//#include <assert.h>
//
//#include "ToonShader.h"
//#include "Model.h"
//#include "Texture.h"
//#include "CameraManager.h"
//
////extern Camera *pCam;
//
//GraphicsObject_Toon::GraphicsObject_Toon(Model *model, ShaderObject *_pShaderObj, TextureName name,  Vect &LightPos, Heap* _pHeap)
//	: GraphicsObject(model, _pShaderObj, _pHeap),
//	textName(name),
//	textureID(TextureMan::find(textName)),
//	projMatrix(_pShaderObj->GetLocation("proj_matrix")),
//	
////mvMatrix(_pShaderObj->GetLocation("mv_matrix")),
//	world_matrix(_pShaderObj->GetLocation("world_matrix")),
//	view_matrix(_pShaderObj->GetLocation("view_matrix")),
//	vLightPos(_pShaderObj->GetLocation("vLightPos"))
//{
//	this->pLightPos = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Vect(LightPos);
//}
//
//GraphicsObject_Toon::~GraphicsObject_Toon()
//{
//	delete this->pLightPos;
//}
//
//void GraphicsObject_Toon::SetState()
//{
//	glActiveTexture(GL_TEXTURE0);
//
//	// Bind the texture
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	//glEnable(GL_CULL_FACE);
//	glDisable(GL_CULL_FACE);
//	glFrontFace(GL_CW);
//}
//
//void GraphicsObject_Toon::SetDataGPU()
//{
//	// use this shader
//	this->pShaderObj->SetActive();
//
//	// set the vao
//	glBindVertexArray(this->GetModel()->vao);
//
//	//Matrix mvMat = CameraManager::GetActiveCamera()->getProjMatrix() * *pWorld;
//
//	glUniformMatrix4fv(this->projMatrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getProjMatrix());
//	//glUniformMatrix4fv(this->mvMatrix, 1, GL_FALSE,   (float*)&mvMat);
//	glUniformMatrix4fv(this->view_matrix, 1, GL_FALSE, (float *)&CameraManager::GetActiveCamera()->getViewMatrix());
//	glUniformMatrix4fv(this->world_matrix, 1, GL_FALSE, (float *)&this->GetWorld());
//
//	glUniform3fv(this->vLightPos, 1, (float *)this->pLightPos);
//	//glUniform4fv(this->vColor, 1, (float *)this->pLightColor);
//}
//
//void GraphicsObject_Toon::Draw()
//{
//	// draw
//	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));
//
//	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
//}
//
//void GraphicsObject_Toon::RestoreState()
//{
//	// undo the states you corrupted
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CW);
//}

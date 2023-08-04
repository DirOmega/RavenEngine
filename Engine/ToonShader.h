//#pragma once
//
//#include "GraphicsObject.h"
//#include "TextureMan.h"
//
//class GraphicsObject_Toon : public GraphicsObject
//{
//public:
//
//	GraphicsObject_Toon(Model *model, ShaderObject *pShaderObj, TextureName  name, Vect &LightPos, Heap* _pHeap);
//	GraphicsObject_Toon() = delete;
//	~GraphicsObject_Toon();
//
//	// Rendermaterial contract
//	virtual void SetState() override;
//	virtual void SetDataGPU() override;
//	virtual void Draw() override;
//	virtual void RestoreState() override;
//
//	// data:  place uniform instancing here
//
//	TextureName textName;
//	GLuint textureID;
//		GLint projMatrix;
//	//	GLint mvMatrix;
//
//	GLint world_matrix;
//	GLint view_matrix;
//	GLint vLightPos;
//	Vect *pLightPos;
//
//};

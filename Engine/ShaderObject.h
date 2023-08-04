#pragma once

#include "GLibSetup.h"
//#include "DLink.h"

class ShaderObject //: public DLink
{
public:

	enum class ShaderName
	{
		NULL_SHADER,
		DEFAULT,
		TextureFlat,
		TextureFlatSkin,
		TextureSkinLight,
		TextureSkinLight2,
		TexturePointLight,
		TextureFlatSkinWBT,
		TextureFlatSkinShapeDefo,
		TextureFlatSkinShapeDefo2,
		ShapeDefoCompute,
		SkinShapeDefoWBT,
		ShapeDefoLight,
		ShapeDefoLight2,
		Toon,

	};

	ShaderObject(ShaderName shaderName, const char * const shaderBaseFileName);
	ShaderObject();
	virtual ~ShaderObject();

	void Set(ShaderName shaderName, const char* const baseFileName); // on an active shader will free it

	void SetActive();
	GLint GetLocation(const char * uniformName);
	GLuint GetBufferLocation(const char * uboName);
	GLuint getProgramObject();
	ShaderName GetName();
	void SetName(ShaderName _name);
	//void dClean() override;

private:
	bool privCreateShader(GLuint &programObject, const char * const shaderBaseFileName);
	bool privLoadNCompile(GLuint &shaderObject, const char * const shaderFileName, GLenum shader_type);

	// data
	ShaderName  name;
	GLuint		programObject;

};
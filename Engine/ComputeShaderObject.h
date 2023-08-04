#pragma once

#include "GLibSetup.h"
#include "ShaderObject.h"

class ComputeShaderObject : public ShaderObject
{//TODO:: slim this down to rely more on the base shader object
public:

	ComputeShaderObject(ShaderName shaderName, const char * const shaderBaseFileName);
	ComputeShaderObject();
	virtual ~ComputeShaderObject();

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
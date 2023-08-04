#include "ShaderObject.h"
#include <assert.h>
#include "File.h"
#include "ScratchSpace.h"
#include "Trace.h"

ShaderObject::ShaderObject(ShaderName shaderName, const char * const shaderBaseFileName)
	: name(shaderName), programObject(0)
{
	assert(shaderBaseFileName != 0);
	this->privCreateShader(this->programObject, shaderBaseFileName);
}

void ShaderObject::Set(ShaderName shaderName, const char* const baseFileName)
{
	name = shaderName;
	this->privCreateShader(this->programObject, baseFileName);
}

ShaderObject::ShaderObject() 
	: name(ShaderName::NULL_SHADER) , programObject(0)
{}

ShaderObject::~ShaderObject()
{
	//glDeleteProgram(this->programObject);
	if (this->name != ShaderName::NULL_SHADER)
	{
		glUseProgram(0);//TODO:: check if this line should go here
		glDeleteProgram(this->programObject);
	}
}

GLint ShaderObject::GetLocation(const char * uniformName)
{
	assert(uniformName != 0);
	GLint loc = glGetUniformLocation(this->programObject, uniformName);
	assert(loc != -1);

	return loc;
}

GLuint ShaderObject::GetBufferLocation(const char * uboName)
{
	assert(uboName != 0);
	GLuint loc = glGetUniformBlockIndex(this->programObject, uboName);
	assert(loc != GL_INVALID_INDEX);
	assert(loc != GL_INVALID_OPERATION);
	return loc;
}

void ShaderObject::SetActive()
{
	// Setup for shader
	glUseProgram(this->programObject);
}

bool ShaderObject::privLoadNCompile(GLuint &shaderObject, const char * const shaderFileName, GLenum shader_type)
{

	File::Handle fh;
	File::Error  ferror;

	ferror = File::Open(fh, shaderFileName, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Seek(fh, File::Location::END, 0);
	assert(ferror == File::Error::SUCCESS);

	unsigned int numBytesInFile;
	ferror = File::Tell(fh, numBytesInFile);
	assert(ferror == File::Error::SUCCESS);

	ferror = File::Seek(fh, File::Location::BEGIN, 0);
	assert(ferror == File::Error::SUCCESS);

	//char *data = new(ScratchSpace::GetShdrManHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) char[numBytesInFile + 1];
	char *data = new char[numBytesInFile + 1];
	assert(data != 0);

	ferror = File::Read(fh, data, numBytesInFile);
	assert(ferror == File::Error::SUCCESS);

	// null termination character
	data[numBytesInFile] = 0;

	ferror = File::Close(fh);
	assert(ferror == File::Error::SUCCESS);

	// create a shader object
	shaderObject = glCreateShader(shader_type);
	assert(shaderObject != 0);

	//load shader
	glShaderSource(shaderObject, 1, &data, 0);
	delete[] data;

	glCompileShader(shaderObject);

	// check for errors
	GLint status = 0;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);

	// found one
	if (!status)
	{
		char buffer[4096];
		glGetShaderInfoLog(shaderObject, 4096, NULL, buffer);

		Trace::out("Error(ShaderLoadNCompile.cpp) %s: %s\n", shaderFileName, buffer);
		//printf("Error(ShaderLoadNCompile.cpp) %s: %s\n", shaderFileName, buffer);

		glDeleteShader(shaderObject);

		assert(false);
		return false;
	}
	else
	{
		return true;
	}
}

bool ShaderObject::privCreateShader(GLuint &inProgramObject, const char * const shaderBaseFileName)
{
	bool status;
	char nameBuff[128];

	assert(shaderBaseFileName != 0);

	strcpy_s(nameBuff, 128, shaderBaseFileName);
	strcat_s(nameBuff, 128, ".fs.glsc");

	GLuint fs;
	status = privLoadNCompile(fs, nameBuff, GL_FRAGMENT_SHADER);
	assert(status == true);

	strcpy_s(nameBuff, 128, shaderBaseFileName);
	strcat_s(nameBuff, 128, ".vs.glsc");

	GLuint vs;
	status = privLoadNCompile(vs, nameBuff, GL_VERTEX_SHADER);
	assert(status == true);

	// Creates an empty program object
	inProgramObject = glCreateProgram();

	//Attaches a shader object to a program object
	glAttachShader(inProgramObject, vs);
	glAttachShader(inProgramObject, fs);

	// Links a program object
	glLinkProgram(inProgramObject);

	glDeleteShader(fs);
	glDeleteShader(vs);

	return true;
}


ShaderObject::ShaderName ShaderObject::GetName()
{
	return this->name;
}

void ShaderObject::SetName(ShaderObject::ShaderName _name)
{
	this->name = _name;
}

GLuint ShaderObject::getProgramObject()
{
	return this->programObject;
}
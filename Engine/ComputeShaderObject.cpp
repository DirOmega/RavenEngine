#include "ComputeShaderObject.h"
#include <assert.h>
#include "File.h"
#include "ScratchSpace.h"
#include "Trace.h"

ComputeShaderObject::ComputeShaderObject(ShaderName shaderName, const char * const shaderBaseFileName)
	: name(shaderName), programObject(0)
{
	assert(shaderBaseFileName != 0);
	this->privCreateShader(this->programObject, shaderBaseFileName);
	assert(programObject);
}

void ComputeShaderObject::Set(ShaderName shaderName, const char* const baseFileName)
{
	name = shaderName;
	this->privCreateShader(this->programObject, baseFileName);
}

ComputeShaderObject::ComputeShaderObject()
	: name(ShaderName::NULL_SHADER), programObject(0)
{}

ComputeShaderObject::~ComputeShaderObject()
{
	//glDeleteProgram(this->programObject);
	if (this->name != ShaderName::NULL_SHADER)
	{
		glUseProgram(0);//TODO:: check if this line should go here
		glDeleteProgram(this->programObject);
	}
}

GLint ComputeShaderObject::GetLocation(const char * uniformName)
{
	assert(uniformName != 0);
	GLint loc = glGetUniformLocation(this->programObject, uniformName);
	assert(loc != -1);

	return loc;
}

GLuint ComputeShaderObject::GetBufferLocation(const char * uboName)
{
	assert(uboName != 0);
	GLuint loc = glGetUniformBlockIndex(this->programObject, uboName);

	assert(loc != GL_INVALID_INDEX);
	assert(loc != GL_INVALID_OPERATION);
	return loc;
}

void ComputeShaderObject::SetActive()
{
	// Setup for shader
	glUseProgram(this->programObject);
}

bool ComputeShaderObject::privLoadNCompile(GLuint &shaderObject, const char * const shaderFileName, GLenum shader_type)
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

bool ComputeShaderObject::privCreateShader(GLuint &inProgramObject, const char * const shaderBaseFileName)
{
	bool status;
	char nameBuff[128];

	assert(shaderBaseFileName != 0);

	strcpy_s(nameBuff, 128, shaderBaseFileName);
	strcat_s(nameBuff, 128, ".comp.glsc");

	GLuint compute;
	status = privLoadNCompile(compute, nameBuff, GL_COMPUTE_SHADER);
	assert(status == true);

	/*strcpy_s(nameBuff, 128, shaderBaseFileName);
	strcat_s(nameBuff, 128, ".vs.glsc");

	GLuint vs;
	status = privLoadNCompile(vs, nameBuff, GL_VERTEX_SHADER);
	assert(status == true);
*/
	// Creates an empty program object
	inProgramObject = glCreateProgram();

	//Attaches a shader object to a program object
	//glAttachShader(inProgramObject, vs);
	glAttachShader(inProgramObject, compute);

	// Links a program object
	glLinkProgram(inProgramObject);

	glDeleteShader(compute);

	return true;
}


ShaderObject::ShaderName ComputeShaderObject::GetName()
{
	return this->name;
}

void ComputeShaderObject::SetName(ShaderObject::ShaderName _name)
{
	this->name = _name;
}

GLuint ComputeShaderObject::getProgramObject()
{
	return this->programObject;
}
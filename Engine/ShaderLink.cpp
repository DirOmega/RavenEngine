#include "ShaderLink.h"

ShaderLink::ShaderLink() : shdr(0)
{
}

ShaderLink::~ShaderLink()
{
	if (shdr != nullptr)
	{
		delete shdr;
	}
}

void ShaderLink::dClean()
{
	
	if (shdr != nullptr)
	{
		delete shdr;
		shdr = 0;
	}
}

void ShaderLink::set(ShaderObject* _shdr)
{
	shdr = _shdr;
}

void ShaderLink::setName(ShaderObject::ShaderName _name)
{
	shdr->SetName(_name);
}

ShaderObject* ShaderLink::getShader()
{
	return shdr;
}

#pragma once

#include "DLink.h"
#include "ShaderObject.h"


//TODO:: get rid of shader links it's a tempoary fix to accomodate the mem system tracking
// the mem system is being destroyed before the static managers so it's claiming the shaders are leaked.
//however because they are not calling the destructors the program on the gpu never gets released
class ShaderLink : public DLink
{

public:

	ShaderLink();
	virtual ~ShaderLink();
	ShaderLink(const ShaderLink&) = delete;
	ShaderLink(const ShaderLink&&) = delete;
	ShaderLink operator = (const ShaderLink&) = delete;
	ShaderLink operator = (const ShaderLink&&) = delete;

	void dClean() override;
	void set(ShaderObject* mdl);

	void setName(ShaderObject::ShaderName _name);
	ShaderObject* getShader();

private:
	ShaderObject* shdr;

};
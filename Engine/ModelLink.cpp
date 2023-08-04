#include "ModelLink.h"

//move tex link's goodies down to texture
ModelLink::ModelLink()
	: name(ModelName::NOT_INITIALIZED),
	mdl(0)
{
}

ModelLink::~ModelLink()
{
	if (mdl != nullptr)
	{
		delete mdl;
	}
}

void ModelLink::dClean()
{
	name = ModelName::NOT_INITIALIZED;
	if (mdl != nullptr)
	{
		delete mdl;
		mdl = 0;
	}
}

void ModelLink::set(ModelName _name, Model* _mdl)
{
	name = _name;
	mdl = _mdl;
}

void ModelLink::setName(ModelName _name)
{
	name = _name;
}

Model* ModelLink::getModel()
{
	return mdl;
}

#include <assert.h>

#include "NullModel.h"

#define UNUSED_VAR(x) (x)

NullModel::NullModel(const char * const modelFileName)
	: Model()
{
	UNUSED_VAR(modelFileName);
}

NullModel::~NullModel()
{
	// remove anything dynamic here
}

void NullModel::privCreateVAO(const char * const, const char * const)
{

}

void NullModel::privCreateVAO(const char * const modelFileName)
{
	UNUSED_VAR(modelFileName);
}


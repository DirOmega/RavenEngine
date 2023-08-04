#pragma once

#include "Model.h"

class BoundingBoxModel : public Model
{
public:

	BoundingBoxModel(const char * const pModelFileName);
	BoundingBoxModel(const BoundingBoxModel &) = delete;
	BoundingBoxModel &operator=(const BoundingBoxModel &) = delete;
	virtual ~BoundingBoxModel();

private:

	BoundingBoxModel() = delete;

	void privCreateVAO(const char * const modelFileName) override; 

};
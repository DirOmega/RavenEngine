#pragma once

#include "Model.h"




class ModelFromFile : public Model
{
public:

	ModelFromFile(const char * const pModelFileName);
	ModelFromFile(const char * const pPackageFileName, const char * const pModelFileName);
	ModelFromFile(const ModelFromFile &) = delete;
	ModelFromFile &operator=(const ModelFromFile &) = delete;
	virtual ~ModelFromFile();

private:




	ModelFromFile() = delete;
	void privCreateVAO(const char * const modelFileName) override;
	void privCreateVAO(const char * const pPackageFileName, const char * const pModelName) override;



};
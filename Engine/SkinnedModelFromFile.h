#pragma once

#include "Model.h"
#include "MathEngine.h"

class SkinnedModelFromFile : public Model
{
public:

	SkinnedModelFromFile(const char * const pModelFileName);
	SkinnedModelFromFile(const char * const pPackageFileName, const char * const pModelFileName);
	SkinnedModelFromFile(const SkinnedModelFromFile &) = delete;
	SkinnedModelFromFile &operator=(const SkinnedModelFromFile &) = delete;
	virtual ~SkinnedModelFromFile();



	//GLuint bindBuffer;
	//GLuint boneBuffer;

	//Matrix* bindArray;
	//Matrix* boneArray;


private:

	Vert_Weights* boneWeights;
	Bone_Index*	  boneIndices;



	GLuint vWi;
	GLuint bIi;
	//GLuint uboBindData;
	//GLuint uboboneMats;

	SkinnedModelFromFile() = delete;
	void privCreateVAO(const char * const modelFileName) override;
	void privCreateVAO(const char * const pPackageFileName, const char * const pModelName) override;



};
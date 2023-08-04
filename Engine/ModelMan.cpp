#include "ModelMan.h"
#include "Model.h"
#include "ModelFromFile.h"

#include "PyramidModel.h"
#include "NullModel.h"
#include "ScratchSpace.h"

#include "SkinnedModelFromFile.h"

ModelMan * ModelMan::pInstance = 0;

#define UNUSED_VAR(x) (x)

ModelMan::ModelMan()
	: baseManager(3)
{

	//this->active = 0;
};

ModelMan::ModelMan(int numStart, int deltaAdd)
	: baseManager(deltaAdd)
{
	generateReserves(numStart);
	//poRefLink = new ModelLink();
}

ModelMan::~ModelMan()
{

};

ModelMan* ModelMan::privGetInstance()
{
	if (pInstance == nullptr)
	{
		ModelMan::Create();
	}
	return pInstance;
};

void ModelMan::loadDefaults()
{

	ModelMan* pModMan = ModelMan::privGetInstance();
	assert(pModMan != nullptr);

	

	ModelLink *pNode = (ModelLink*)pModMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it

	PyramidModel* pMode = new PyramidModel("notused");

	pNode->set(ModelName::Pryamid, pMode);

	pNode = (ModelLink*)pModMan->baseAdd();
	assert(pNode != nullptr);

	NullModel* pNullMod = new NullModel("NotUSed");

	pNode->set(ModelName::NullMod, pNullMod);

	//return pNode->getModel();
}

void ModelMan::Create(int numStart, int deltaAdd)
{
	assert(numStart > 0); //makes sure that it always starts with at least one
	assert(deltaAdd > 0); // makes sure that it's usable because if this is 0 or negative the manager would not create additonal resources when it needs to.
	assert(pInstance == nullptr);

	if (pInstance == nullptr)
	{
		pInstance = new ModelMan(numStart, deltaAdd);
		loadDefaults();
		//ModelMan::addModel("HotPink.tga", ModelName::NOT_INITIALIZED); //adds the hot pink Model with the default name so if a object can't find it's Model it will show as pink.
	}
}

void ModelMan::deleteManager()
{
	delete ModelMan::privGetInstance();
}

void ModelMan::deleteModels()
{
	ModelMan *pTextMan = ModelMan::privGetInstance();

	//ModelLink *pNode = (ModelLink *)pTextMan->active;
	ModelLink *pNode = (ModelLink*)pTextMan->getActiveHead();
	while (pNode != 0)
	{
		// bye bye
		//glDeleteModels(1, &pNode->ModelID);

		pTextMan->Remove((DLink&)*pNode);
		pNode = (ModelLink*)pTextMan->getActiveHead();

		// next
		//pNode = (ModelLink *)pNode->pNext;
	}
};

DLink* ModelMan::dCreateNode()
{
	DLink* newNode = new ModelLink();
	assert(newNode != nullptr);
	return newNode;
};

void ModelMan::dClearNode(DLink* pLink)
{
	pLink->dClean();
};

bool ModelMan::dCompareNodes(DLink& pLinkA, DLink& pLinkB)
{
	UNUSED_VAR(pLinkA);
	UNUSED_VAR(pLinkB);
	return false;
};


Model* ModelMan::addModel(const char * const _fileName, const char * const _name, ModelName _nameE)
{
	assert(_fileName);
	assert(_name);
	
	ModelMan* pModMan = ModelMan::privGetInstance();
	assert(pModMan != nullptr);

	Model* pMdl = new ModelFromFile(_fileName, _name);
	assert(pMdl);

	ModelLink *pNode = (ModelLink*)pModMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_nameE, pMdl);

	return pNode->getModel();
}


Model* ModelMan::addModelSkinned(const char * const _assetName, const ModelName _name)
{
	assert(_assetName);

	// Get the instance to the manager
	ModelMan *pModMan = ModelMan::privGetInstance();
	assert(pModMan != nullptr);

	// Load the Model and get the ModelID
	//pModMan->privLoadModel(_assetName, pModelID);
	Model* pMdl = new SkinnedModelFromFile(_assetName);
	assert(pMdl);
	//Model* pCubeModel = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) ModelFromFile("cubeModel.azul");

	// get a node from the Modelman
	ModelLink *pNode = (ModelLink*)pModMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_name, pMdl);

	return pNode->getModel();

};

Model* ModelMan::addModel(const char * const _assetName, const ModelName _name)
{
	assert(_assetName);

	// Get the instance to the manager
	ModelMan *pModMan = ModelMan::privGetInstance();
	assert(pModMan != nullptr);

	// Load the Model and get the ModelID
	//pModMan->privLoadModel(_assetName, pModelID);
	Model* pMdl = new ModelFromFile(_assetName);
	assert(pMdl);
	//Model* pCubeModel = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) ModelFromFile("cubeModel.azul");

	// get a node from the Modelman
	ModelLink *pNode = (ModelLink*)pModMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_name, pMdl);

	return pNode->getModel();

};

//Model* ModelMan::addModel(const char * const _assetName, const ModelName _name)
//{
//	assert(_assetName);
//
//	// Get the instance to the manager
//	ModelMan *pModMan = ModelMan::privGetInstance();
//	assert(pModMan != nullptr);
//
//	// Load the Model and get the ModelID
//	//pModMan->privLoadModel(_assetName, pModelID);
//	Model* pMdl = new ModelFromFile(_assetName);
//	assert(pMdl);
//	//Model* pCubeModel = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) ModelFromFile("cubeModel.azul");
//
//	// get a node from the Modelman
//	ModelLink *pNode = (ModelLink*)pModMan->baseAdd();
//	assert(pNode != nullptr);
//
//	// initialize it
//	pNode->set(_name, pMdl);
//
//	return pNode->getModel();
//
//};

Model* ModelMan::find(const ModelName _name)
{
	// Get the instance to the manager
	ModelMan *pTextMan = ModelMan::privGetInstance();

	//ModelLink *pNode = (ModelLink *)pTextMan->active;
	ModelLink *pNode = (ModelLink *)pTextMan->getActiveHead();
	while (pNode != 0)
	{
		if (pNode->name == _name)
		{
			// found it
			break;
		}

		pNode = (ModelLink *)pNode->pNext;
	}

	return pNode->getModel();
};

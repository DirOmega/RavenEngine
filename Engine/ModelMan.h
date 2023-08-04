#pragma once

#include "GLibSetup.h"
#include "baseManager.h"
#include "ModelLink.h"


class Model;

// Singleton
class ModelMan : public baseManager
{
public:

	static Model* addModel(const char * const _fileName, const char * const _name, ModelName _nameE);

	static Model* addModel(const char * const _assetName, const ModelName _name);

	static Model* addModelSkinned(const char * const _assetName, const ModelName _name);


	static void deleteModels();
	static void deleteManager();
	static Model* find(const ModelName _name);

	static void Create(int numStart = 5, int deltaAdd = 3);


private:  // methods

	ModelMan();
	ModelMan(int numStart, int deltaAdd);
	virtual ~ModelMan();
	ModelMan(const ModelMan &) = delete;
	ModelMan(const ModelMan &&) = delete;
	ModelMan operator = (const  ModelMan &) = delete;
	ModelMan operator = (const ModelMan &&) = delete;

	static ModelMan *privGetInstance();

	DLink* dCreateNode() override;
	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) override;
	void dClearNode(DLink* pLink) override;

	static void loadDefaults();

	// helper private methods
	//Model* privLoadModel(const char * const _assetName, ModelName _name);
	//void privLoadModel(const char * const _assetName, GLuint *&ModelID);
	//bool privLoadTGAModel(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode);
	//void privAddToFront(TexLink *node, TexLink *&head);
	//GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

private:  // add

	static ModelMan* pInstance;
	//static Model* poRefTex; //for searching
	//TexLink *active;

};

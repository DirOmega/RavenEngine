#pragma once

#include "baseManager.h"
#include "ShaderObject.h"

class ShaderManager : public baseManager
{
public:

	static ShaderObject* addShader(const ShaderObject::ShaderName name, const char * const baseFileName);
	//static void addShader(ShaderObject* shader); //this is so I can still use my mem system.
	static void deleteShaders();
	static void deleteManager();
	static ShaderObject* find(const ShaderObject::ShaderName cameraName);
	
	static void Create(int numStart = 3, int deltaAdd = 3);

private:

	ShaderManager() = delete;
	ShaderManager(int numStart, int deltaAdd);
	virtual ~ShaderManager();

	ShaderManager(const ShaderManager &) = delete;
	ShaderManager(const ShaderManager &&) = delete;
	ShaderManager operator = (const ShaderManager &) = delete;
	ShaderManager operator = (const ShaderManager &&) = delete;

	static ShaderManager *privGetInstance();

	DLink* dCreateNode() override;
	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) override;
	void dClearNode(DLink* pLink) override;

	//DATA
	static ShaderManager* pInstance;
	ShaderObject* poRefLink;

};
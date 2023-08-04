#include "ShaderManager.h"
#include "ShaderLink.h"

#include "ScratchSpace.h"

#define UNUSED_VAR(x) (x)

ShaderManager* ShaderManager::pInstance = 0;


ShaderManager::ShaderManager(int numStart, int  deltaAdd)
	: baseManager(deltaAdd)
{

	generateReserves(numStart);
	poRefLink = new ShaderObject();
}

ShaderManager::~ShaderManager()
{
	delete poRefLink;
	//deleteShaders(); handled by base manager
}

ShaderManager* ShaderManager::privGetInstance()
{
	if (pInstance == nullptr)
	{
		ShaderManager::Create();

		addShader(ShaderObject::ShaderName::NULL_SHADER, "nullRender");
	}

	return pInstance;
}

//Todo:: with proper Mem allocation
ShaderObject* ShaderManager::addShader(ShaderObject::ShaderName name, const char * const baseFileName)
{

	ShaderObject* pNode = ShaderManager::find(name);
	if(pNode != nullptr)
	{
		 //pNode; //do nothing cuz I already have it.
	}
	else
	{
		assert(baseFileName);

		ShaderManager* pShadMan = ShaderManager::privGetInstance();
		assert(pShadMan);

	    ShaderLink* pLink = (ShaderLink*)pShadMan->baseAdd();
		assert(pLink != nullptr);
		
		if (pLink->getShader() == nullptr)
		{
			pNode = new ShaderObject();
		}
		
		pNode->Set(name, baseFileName);
		pLink->set(pNode);
	}

	return pNode;
}

//TODO:: remove this because it bypasses the recycling system
//void ShaderManager::addShader(ShaderObject* shader)
//{
//	assert(shader);
//
//	ShaderManager* pShadMan = ShaderManager::privGetInstance();
//	assert(pShadMan);
//
//	ShaderLink* pNode = (ShaderLink*)pShadMan->baseAdd();
//
//	pNode->set(shader);
//}

void ShaderManager::deleteManager()
{
	delete ShaderManager::privGetInstance();
}

void ShaderManager::deleteShaders()
{
	ShaderManager *pCamMan = ShaderManager::privGetInstance();

	//TexLink *pNode = (TexLink *)pTextMan->active;
	ShaderLink *pNode = (ShaderLink*)pCamMan->getActiveHead();
	while (pNode != 0)
	{
		// bye bye

		pCamMan->Remove((DLink&)*pNode);
		pNode = (ShaderLink*)pCamMan->getActiveHead();
	}
}

//TODO:: use basefind and implement the dCompare but I would need to add a refrence version of the objects
//TODO:: last minute discoveries of issues with the shader manager had me comment this out for now.
ShaderObject* ShaderManager::find(const ShaderObject::ShaderName _name)
{
	UNUSED_VAR(_name);

	// Get the instance to the manager
	ShaderManager *pSMan = ShaderManager::privGetInstance();

	//ModelLink *pNode = (ModelLink *)pTextMan->active;
	ShaderLink *pNode = (ShaderLink *)pSMan->getActiveHead();
	while (pNode != 0)
	{
		if (pNode->getShader()->GetName() == _name)
		{
			// found it
			break;
		}

		pNode = (ShaderLink *)pNode->pNext;
	}
	if (pNode == nullptr)
	{
		return nullptr;
	}

	return pNode->getShader();

	//ShaderManager *pCamMan = ShaderManager::privGetInstance();

	//ShaderObject *pNode = (ShaderObject *)pCamMan->getActiveHead();
	//while (pNode != 0)
	//{
	//	if (pNode->GetName() == _name)
	//	{
	//		// found it
	//		break;
	//	}

	//	pNode = (ShaderObject *)pNode->pNext;
	//}

	//return pNode->GetShader();
	//ShaderObject* target = ShaderManager::privGetInstance()->poRefLink;
	//target->SetName(_name);
	//return (ShaderObject*)ShaderManager::privGetInstance()->baseFind((DLink&)*target);
	//assert(false);
	//return nullptr;

}

//removed because I don't want the user to leak memory by setting cameras that aren't managed.
//but I want to add it back in somehow proabbly with handles so I don't have to do a search over cameras to get the active one.
//void ShaderManager::setActive(Shader& camera)
//{
//	privGetInstance()->pActiveCam = &camera;
//}

void ShaderManager::Create(int numStart, int deltaAdd)
{

	assert(numStart > 0); //makes sure that it always starts with at least one
	assert(deltaAdd > 0); // makes sure that it's usable because if this is 0 or negative the manager would not create additonal resources when it needs to.
	assert(pInstance == nullptr);

	if (pInstance == nullptr)
	{
		pInstance = new ShaderManager(numStart, deltaAdd); //TODO:: add a default camera that's auto added
	}

}

DLink* ShaderManager::dCreateNode()
{
	DLink* newNode = (DLink*) new ShaderLink();
	assert(newNode != nullptr);
	return newNode;
}

bool ShaderManager::dCompareNodes(DLink& pLinkA, DLink& pLinkB)
{
	bool retval = false;
	if (((ShaderLink*)&pLinkA)->getShader()->GetName() == ((ShaderLink*)&pLinkB)->getShader()->GetName())
	{
		retval = true;
	}

	return retval;

}

void ShaderManager::dClearNode(DLink* pLink)
{
	pLink->dClean();
}


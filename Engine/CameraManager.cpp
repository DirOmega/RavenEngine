#include "CameraManager.h"
#include <assert.h>
#include "CameraLink.h"

#include "ScratchSpace.h"

#define UNUSED_VAR(x) (x)

CameraManager* CameraManager::pInstance = 0;

CameraManager::CameraManager(int numStart, int  deltaAdd)
	: baseManager(deltaAdd)
{
	generateReserves(numStart);
	poRefLink = new CameraLink();
}

CameraManager::~CameraManager()
{
	delete poRefLink;
}

CameraManager* CameraManager::privGetInstance()
{
	if (pInstance == nullptr)
	{
		CameraManager::Create();
	}

	return pInstance;
}

void CameraManager::addCamera(Camera::CameraName name, Camera* c , bool setActive)
{
	assert(c);

	CameraManager* pCamMan = CameraManager::privGetInstance();
	assert(pCamMan);

	CameraLink* pNode = (CameraLink*)pCamMan->baseAdd();
	assert(pNode != nullptr);

	pNode->Set(name, c);

	if (setActive)
	{
		pCamMan->pActiveCam = c; //TODO:: setup camera manager link 
		pCamMan->pActiveLink = pNode;
	}
}

void CameraManager::deleteManager()
{
	delete CameraManager::privGetInstance();
}

void CameraManager::deleteCameras()
{
	CameraManager *pCamMan =CameraManager::privGetInstance();

	//TexLink *pNode = (TexLink *)pTextMan->active;
	CameraLink *pNode = (CameraLink*)pCamMan->getActiveHead();
	while (pNode != 0)
	{
		// bye bye
		pCamMan->Remove((DLink&) *pNode);
		pNode = (CameraLink*)pCamMan->getActiveHead();
	}
}

//TODO:: use base find and implement the dCompare but I would need to add a refrence version of the objects
CameraLink* CameraManager::find(const Camera::CameraName _name)
{
	//CameraManager *pCamMan = CameraManager::privGetInstance();

	//CameraLink *pNode = (CameraLink *)pCamMan->getActiveHead();
	//while (pNode != 0)
	//{
	//	if (pNode->GetName() == _name)
	//	{
	//		// found it
	//		break;
	//	}

	//	pNode = (CameraLink *)pNode->pNext;
	//}

	//return pNode->GetCamera();
	CameraLink* target = CameraManager::privGetInstance()->poRefLink;
	target->SetName(_name);
	return (CameraLink*) CameraManager::privGetInstance()->baseFind((DLink&)*target);
}

void CameraManager::setActive(const Camera::CameraName cameraName)
{
	CameraManager *pCamMan = CameraManager::privGetInstance();

	pCamMan->pActiveLink = find(cameraName);
	pCamMan->pActiveCam = pCamMan->pActiveLink->GetCamera();
	//pCamMan->pActiveCam = find(cameraName)->GetCamera();
}

//removed because I don't want the user to leak memory by setting cameras that aren't managed.
//but I want to add it back in somehow proabbly with handles so I don't have to do a search over cameras to get the active one.
//void CameraManager::setActive(Camera& camera)
//{
//	privGetInstance()->pActiveCam = &camera;
//}

void CameraManager::Create(int numStart, int deltaAdd)
{
	assert(numStart > 0); //makes sure that it always starts with at least one
	assert(deltaAdd > 0); // makes sure that it's usable because if this is 0 or negative the manager would not create additonal resources when it needs to.
	assert(pInstance == nullptr);

	if (pInstance == nullptr)
	{
		pInstance = new CameraManager(numStart, deltaAdd); //TODO:: add a default camera that's auto added
	}
}

Matrix& CameraManager::GetProjMatrix()
{
	assert(privGetInstance()->pActiveCam);
	return privGetInstance()->pActiveCam->getProjMatrix();
}

CameraLink* CameraManager::GetActiveCameraLink()
{
	return privGetInstance()->pActiveLink;
}

Camera* CameraManager::GetActiveCamera()
{
	return privGetInstance()->pActiveCam;
}

DLink* CameraManager::dCreateNode()
{
	DLink* newNode = (DLink*) new CameraLink();
	assert(newNode != nullptr);
	return newNode;
}

bool CameraManager::dCompareNodes(DLink& pLinkA, DLink& pLinkB)
{
	bool retval = false;
	if (((CameraLink*)&pLinkA)->GetName() == ((CameraLink*)&pLinkB)->GetName())
	{
		retval = true;
	}

	return retval;
}

void CameraManager::dClearNode(DLink* pLink)
{
	pLink->dClean();
}

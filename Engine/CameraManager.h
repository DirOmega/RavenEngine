#pragma once

#include "baseManager.h"
#include "MathEngine.h"
#include "Camera.h"
#include "CameraLink.h"

class CameraManager : public baseManager
{
public:

	static void addCamera(Camera::CameraName name, Camera* c, bool setActive = false);
	static void deleteCameras();
	static void deleteManager();
	static CameraLink* find(const Camera::CameraName cameraName);
	static void setActive(const Camera::CameraName cameraName);
	//static void setActive(Camera& cameraName);
	static void Create(int numStart = 1, int deltaAdd = 1);

	static Matrix& GetProjMatrix();
	static Camera* GetActiveCamera();
	static CameraLink* GetActiveCameraLink();

private:

	CameraManager() = delete;
	CameraManager(int numStart, int deltaAdd);
	virtual ~CameraManager();
	
	CameraManager(const CameraManager &) = delete;
	CameraManager(const CameraManager &&) = delete;
	CameraManager operator = (const  CameraManager &) = delete;
	CameraManager operator = (const CameraManager &&) = delete;

	static CameraManager *privGetInstance();

	DLink* dCreateNode() override;
	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) override;
	void dClearNode(DLink* pLink) override;

	//DATA
	static CameraManager* pInstance;
	CameraLink* poRefLink;
	CameraLink* pActiveLink;
	Camera* pActiveCam;


};


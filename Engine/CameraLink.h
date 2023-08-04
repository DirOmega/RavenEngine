#pragma once

#include "DLink.h"
#include "Camera.h"
#include "CamVals.h"

//Camera link's don't do the work of a camera because Cameras need alignment and a vptr would throw off that alignment
class CameraLink : public DLink
{

public:

	CameraLink();
	~CameraLink();

	void Set(Camera::CameraName name,Camera* cam);
	Camera* GetCamera();
	Camera::CameraName GetName();
	void SetName(Camera::CameraName name);

	//void PushAnimVals(CamVals* camVals);

	void PushAnimVals(CamVals* pcamVals)
	{
		//camera data is in z a differnt coordinate system so r and z are flipped here and in set position
		Matrix rotX(RotType::ROT_X, pcamVals->rx);//(pcamVals->rx * MATH_PI) / 180.0f);
		Matrix rotY(RotType::ROT_Y, pcamVals->rz);//(pcamVals->ry * MATH_PI) / 180.0f);
		Matrix rotZ(RotType::ROT_Z, pcamVals->ry);//(pcamVals->rz * MATH_PI) / 180.0f);

		Matrix RotMatrix = rotX * rotY * rotZ;
		
		//Calculate new upVector
		Vect vUpDir = *DefaultUp * RotMatrix;
		
		LinkPos->set(pcamVals->px, pcamVals->pz, pcamVals->py);

		Vect vTmp = *LinkPos;
		vTmp.y() += pcamVals->distance;
		
		Vect vPos = *LinkPos + (vTmp - *LinkPos) * RotMatrix;
		
		camera->setOrientAndPosition(vUpDir, *LinkPos, vPos);
		camera->setFieldOfView(pcamVals->fov);

		//Old versions
		//Vect vUpDir = *DefaultUp;
		////camera->getUp(vUpDir);
		//
		////Matrix rotX(RotType::ROT_X, pcamVals->rx);//(pcamVals->rx * MATH_PI) / 180.0f);
		////Matrix rotY(RotType::ROT_Y, pcamVals->rz);//(pcamVals->ry * MATH_PI) / 180.0f);
		////Matrix rotZ(RotType::ROT_Z, pcamVals->ry);//(pcamVals->rz * MATH_PI) / 180.0f);
		//
		//
		//Matrix rotX(RotType::ROT_X, pcamVals->rx);//(pcamVals->rx * MATH_PI) / 180.0f);
		//Matrix rotY(RotType::ROT_Y, pcamVals->rz);//(pcamVals->ry * MATH_PI) / 180.0f);
		//Matrix rotZ(RotType::ROT_Z, pcamVals->ry);//(pcamVals->rz * MATH_PI) / 180.0f);
		//
		//
		//vUpDir *= (rotX * rotY * rotZ);
		//
		////vUpDir =  vUpDir * (rotX * rotY * rotZ);
		//
		////camera->getDir(vDir);
		//
		////*LinkPos = pcamVals->camPos;
		////LinkPos->x() = pcamVals->camPos.x();
		////LinkPos->y() = pcamVals->camPos.z();
		////LinkPos->z() = pcamVals->camPos.y();
		//
		//LinkPos->set(pcamVals->px, pcamVals->pz, pcamVals->py);
		//
		//
		//Vect vLookAt;
		//vLookAt = *LinkPos;
		//
		////
		////if (pcamVals->distance > 0.0f)
		////{
		////	vDir.y() += pcamVals->distance;
		////}
		//
		////Vect vPos = *LinkPos;
		////vPos.y() += pcamVals->distance;
		////vPos *= (rotX  * rotY * rotZ);
		//
		//
		//Vect vTmp = *LinkPos;
		//vTmp.y() += pcamVals->distance;
		//
		//Vect vTmpDir =  vTmp - *LinkPos;
		//
		//vTmpDir *= rotX * rotY * rotZ;
		//
		//Vect vPos = *LinkPos + vTmpDir;
		//
		//camera->setOrientAndPosition(vUpDir, vLookAt, vPos);
		//camera->setFieldOfView(pcamVals->fov);
		//
	}

	void dClean() override;

private:

	Camera::CameraName name;
	Camera * camera;
	Vect* DefaultUp;
	Vect* LinkPos;
	Vect* LinkRot;
	float dist;

	//Vect * aPosAndRot;
	//float dist;

};


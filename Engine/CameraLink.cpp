#include "CameraLink.h"

void CameraLink::SetName(Camera::CameraName _name)
{
	name = _name;
}

void CameraLink::Set(Camera::CameraName n, Camera* cam)
{
	name = n;
	assert(cam != nullptr);
	camera = cam;
	camera->getUp(*DefaultUp);
}

void CameraLink::dClean()
{
	name = Camera::CameraName::Not_Initalized;
	if (camera != nullptr)
	{
		delete camera;
		camera = nullptr;
	}
}

Camera* CameraLink::GetCamera()
{
	return camera;
}

Camera::CameraName CameraLink::GetName()
{
	return name;
}

CameraLink::CameraLink() : name(Camera::CameraName::Not_Initalized), camera(nullptr)
{
	DefaultUp = new Vect();
	LinkPos = new Vect();
	LinkRot = new Vect();
}

CameraLink::~CameraLink()
{
	if (camera != nullptr)
		delete camera;

	delete DefaultUp;
	delete LinkPos;
	delete LinkRot;
}
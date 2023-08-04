#include <assert.h>
#include <math.h>

#include "PCSNode.h"
#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Anim.h"
#include "FrameBucket.h"
#include "GameObjectRigid.h"

GameObjectRigid::GameObjectRigid(GraphicsObject  *graphicsObject)
	: GameObject(graphicsObject)
{
	pQRot = new Quat(IDENTITY);
}

GameObjectRigid::~GameObjectRigid()
{
//	delete pScale;
//	delete pPos;
}

Matrix &GameObjectRigid::GetWorld()
{
	return *this->pWorld;
}

void GameObjectRigid::Update(const Time&)
{
	Matrix T = Matrix(TRANS, *this->pPos);
	Matrix S = Matrix(SCALE, *this->pScale);
	Quat R = *this->pQRot;
	

	//Quat R = *this->pQRot;

	//static float angle = 0.0f;
	//angle += 0.03f;
	//Matrix R = Matrix(ROT_Z, angle);

	Matrix M = S * R * T;

	*this->pWorld = M;
}
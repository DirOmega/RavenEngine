#include "GameObject.h"

#include "MathEngine.h"
#include <math.h>
#include "GraphicsObject.h"
#include "Time.h"
#include "CollisionObject.h"
#include "Anim.h"

#define UNUSED_VAR(x) (x)

GameObject::GameObject(GraphicsObject* _pGO) : pGraphicsObject(_pGO)
{
	assert(_pGO);

	this->pWorld = new Matrix(IDENTITY);
	this->pScale = new Vect(1.0f, 1.0f, 1.0f);

	assert(this->pScale);

	this->pPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(this->pPos);
	assert(pWorld);	
}

//TODO:: optimze this I don't need to check anymore if theese are null they are now managed by this object.
//other than the shader object
GameObject::~GameObject()
{

	delete pPos;
	delete pScale;
	if (pWorld != nullptr)
	{
		delete pWorld;
	}
	if (pGraphicsObject != nullptr)
	{
		delete pGraphicsObject; //TODO: move out of here becuase this will be flyweighted
	}
	if (poColObj != nullptr)
	{
		delete poColObj;
	}

}

Matrix *GameObject::GetWorld()
{
	assert(this->pWorld != nullptr);
	return this->pWorld;
}


void GameObject::Update(const Time& currentTime)
{
	//Default behavior, real behavior should be done in a subclass
	UNUSED_VAR(currentTime);

	Matrix M(IDENTITY);
	
	*this->pWorld = M;
}


//void GameObject::Update(Time currentTime)
//{
//	UNUSED_VAR(currentTime);
//
//	this->rotY += deltaRot;
//	this->rotZ += deltaRot * 0.5f;
//
//	//update the world matrix
//	Matrix Scale(SCALE, *this->pScale);
//	Matrix TransA(TRANS, *this->pPos);
//	Matrix RotY(ROT_Y, this->rotY);
//	Matrix RotZ(ROT_Z, this->rotZ);
//
//	*this->pWorld = Scale * RotY* RotZ* TransA;
//}

void GameObject::Process(const Time& currentTime)
{
	//update
	this->Update(currentTime);

	//push to graphics object 
	//TODO:: this seems like it should go in draw instead if I were instancing the Graphics Object and or the game object
	//assert(pGraphicsObject != nullptr);
	//this->pGraphicsObject->SetWorld(*this->pWorld);
	//Matrix mTmp(*this->pBoneOrientation * *this->pWorld);

	
	//GetGraphicsObject()->SetWorld(mTmp);


//	this->poColObj->SetWorld(mTmp);
	this->pGraphicsObject->SetWorld(*this->pWorld);
	this->poColObj->SetWorld(*this->pWorld);

}


//void GameObject::Update(float currentTime)
//{
//	UNUSED_VAR(currentTime);
//
//	this->rotY += deltaRot;
//	this->rotZ += deltaRot * 0.5f;
//
//	//update the world matrix
//	Matrix Scale(SCALE, *this->pScale);
//	Matrix TransA(TRANS, *this->pTrans);
//	Matrix RotY(ROT_Y, this->rotY);
//	Matrix RotZ(ROT_Z, this->rotZ);
//
//	*this->pWorld = Scale * RotY* RotZ* TransA;
//}
//
//void GameObject::Process(float currentTime)
//{
//	//update
//	this->Update(currentTime);
//
//	//push to graphics object 
//	//TODO:: this seems like it should go in draw instead if I were instancing the Graphics Object and or the game object
//	//assert(pGraphicsObject != nullptr);
//	//this->pGraphicsObject->SetWorld(*this->pWorld);
//	GetGraphicsObject()->SetWorld(*this->pWorld);
//	this->poColObj->SetWorld(*this->pWorld);
//}


GraphicsObject* GameObject::GetGraphicsObject()
{
	assert(this->pGraphicsObject != nullptr);
	return this->pGraphicsObject;
}

void GameObject::SetWorld(Matrix *_pWorld)
{
	assert(_pWorld != nullptr);
	*this->pWorld = *_pWorld;
}

void GameObject::Draw()
{
	assert(this->pGraphicsObject);
	this->pGraphicsObject->Render();
}

void GameObject::Draw(bool)
{
	assert(this->pGraphicsObject);
	this->pGraphicsObject->Render();
	assert(this->poColObj);
	this->poColObj->Render();
}

void GameObject::SetPos(const Vect &pos)
{
	*this->pPos = pos;
	//wthis->poColObj->SetPos(pos);
}

void GameObject::SetScale(const Vect &inScale)
{
	*this->pScale = inScale;
	//this->poColObj->setScale(inScale);
}

void GameObject::getScale(Vect& vOut)
{
	vOut = *pScale;
}

void GameObject::setCollisionObject(CollisionObject* cObj)
{
	this->poColObj = cObj;
}

CollisionObject* GameObject::GetCollisionObject()
{
	return poColObj;
}


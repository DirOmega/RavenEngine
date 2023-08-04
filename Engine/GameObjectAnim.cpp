#include <assert.h>

#include "Anim.h"
#include "GameObject.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"
#include "CollisionObject.h"

//extern Frame_Bucket *pHead;

GameObjectAnim::GameObjectAnim(GraphicsObject* _pGO)
	: GameObject(_pGO)
{

	//this->pLocal = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Matrix(IDENTITY);
	//this->pBoneOrientation = new(_pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Matrix(IDENTITY);

	this->pLocal = new Matrix(IDENTITY);
	this->pBoneOrientation = new Matrix(IDENTITY);

	assert(pLocal);
	assert(pBoneOrientation);


	this->indexBoneArray = 0;

}

GameObjectAnim::~GameObjectAnim()
{
	//if (pPos != nullptr)
	//{
	//	delete pPos;
	//}
	//if (pScale != nullptr)
	//{
	//	delete pScale;
	//}
	if (pLocal != nullptr)
	{
		//bool freetest = Mem::FreeFromMyMem(pLocal);
		//assert(freetest);
		delete pLocal;
	}
	if (pBoneOrientation != nullptr)
	{
		delete pBoneOrientation;
	}
}

void GameObjectAnim::Update(const Time&)
{
	GameObject *pBoneParent = (GameObject *)this->GetParent();
	assert(pBoneParent != 0);

	Matrix ParentWorld = *pBoneParent->GetWorld();

	/*if (this->pBoneParent == 0)
	{
	ParentWorld.set(IDENTITY);
	}
	else
	{
	ParentWorld = *this->pBoneParent->GetWorld();
	}*/

	// REMEMBER this is for Animation and hierachy, you need to handle models differently
	// Get the result bone array, from there make the matrix
	Bone *bResult = result;

	Matrix T = Matrix(TRANS, bResult[indexBoneArray].T);
	Matrix S = Matrix(SCALE, bResult[indexBoneArray].S);
	Quat   Q = bResult[indexBoneArray].Q;

	// Isn't it awesome that we can multiply Quat with matrices!
	Matrix M = S * Q * T;
	//Matrix M = S * T;
	*this->pLocal = M;

	// Goal: update the world matrix
	*this->pWorld = *this->pLocal * ParentWorld;
}

void GameObjectAnim::Process(const Time& currentTime)
{
	this->Update(currentTime);

	Matrix mTmp = *this->pBoneOrientation;

	//Matrix mTmp = *this->pBoneOrientation * *this->pWorld;

	this->pGraphicsObject->SetWorld(mTmp);
	this->poColObj->SetWorld(*this->pWorld);
}

void GameObjectAnim::SetIndex(int val)
{
	this->indexBoneArray = val;
}

void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
{
	*this->pBoneOrientation = tmp;
}

Matrix GameObjectAnim::GetBoneOrientation(void) const
{
	return Matrix(*this->pBoneOrientation);
}
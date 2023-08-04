#pragma once

#include "GameObject.h"
#include "FrameBucket.h"


class GameObjectAnim : public GameObject
{
public:

	GameObjectAnim(GraphicsObject* _pGO);
	
	//Big Four
	GameObjectAnim() = delete;
	GameObjectAnim(const GameObjectAnim &copy) = delete;
	GameObjectAnim & operator = (const GameObjectAnim &pTmp) = delete;

	//virtual toa llow for derived
	virtual ~GameObjectAnim();

	void Process(const Time& currentTime);
	void Update(const Time& currentTime);

	void SetIndex(int val);
	void SetBoneOrientation(const Matrix &tmp);
	Matrix GetBoneOrientation() const;

	//HACK
	void SetResult(Bone* _result)
	{
		result = _result;
	}


public:

	Matrix *pLocal;
	Matrix *pBoneOrientation;
	int indexBoneArray;
	
	Vect *pDof;
	Vect *pUp;

	Bone* result;




};
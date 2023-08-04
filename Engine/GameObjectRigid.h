#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "Time.h"

class GameObjectRigid : public GameObject
{
public:
	GameObjectRigid(GraphicsObject  *graphicsObject);

	// Big Four
	GameObjectRigid() = delete;
	GameObjectRigid(GameObjectRigid &) = delete;
	GameObjectRigid & operator = (const GameObjectRigid &) = delete;
	virtual ~GameObjectRigid() override;

	virtual void Update(const Time& currentTime) override;

	Matrix &GetWorld();

public:

	Quat *pQRot;
	//Vect *pScale;
	//Vect *pPos;


};

#endif

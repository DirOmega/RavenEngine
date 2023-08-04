#pragma once

#include "PCSNode.h"
//foreward Declares

class Matrix;
class Vect;
class Heap;
class Time;
class GraphicsObject;
class CollisionObject;

//TODO:: move Trace.h into the PCS library under PCSTrace so there's no conflict down the line 
class GameObject : public PCSNode
{
public:

	GameObject() = delete;
	GameObject(const GameObject &) = delete;
	GameObject &operator=(GameObject &) = delete;
	GameObject(GraphicsObject* _pGO);
	virtual ~GameObject();

	virtual void Process(const Time& currentTime);
	virtual void Update(const Time& currentTime);
	

	void Draw();
	void Draw(bool debug);

	//void Process(float t);
	//void Update(float t);

	CollisionObject *GetCollisionObject();
	void setCollisionObject(CollisionObject* cObj);
	GraphicsObject *GetGraphicsObject();
	Matrix* GetWorld();
	
	//void setModel(Model* model);
	//void setTex(TGATex * tptr);

	void SetWorld(Matrix *pWorld);
	void SetPos(const Vect &pos);
	void SetScale(const Vect &scale);
	
	void getScale(Vect& vOut);

	//float deltaRot;
	bool Abort;
	bool pad[3];
	//Model* GetModel();
	
	Vect *pPos;
	Vect *pScale;
	//Quat *pROT;


	//GameObject* pBoneParent;



protected:

	Matrix *pWorld;
	CollisionObject*  poColObj;
	GraphicsObject* pGraphicsObject;

private:




	//float rotX;
	//float rotY;
	//float rotZ;
	//Game* pGame;
	
	//Model * pModel;
	//TGATex *pTex;

};

#pragma once

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "BoundingSphere.h"

#pragma warning( push )
#pragma warning( disable : 5045)//I don't care about spectre attacks

class CullingVolume
{
public:

	CullingVolume()
	{
		pTrans = new Vect();
		pScale = new Vect();

	}

	//Matrix& setWorld();

	virtual Vect* getScale()
	{
		return pScale;
	}

	virtual Vect* getTrans()
	{
		return pTrans;
	}

	virtual ~CullingVolume()
	{
		delete pTrans;
		delete pScale;
	}

	//bool Collide(CollisionBox* b, CollisionSphere* s);
	//bool Collide(CollisionSphere* s1, CollisionSphere* s2);

protected:

	Vect *pTrans;
	Vect *pScale;
	float radius;

};

//class CollisionBox : public CollisionVolume
//{};

class CullingSphere : public CullingVolume
{
public:

	CullingSphere(Sphere* s) : CullingVolume()
	{
		pTrans->set(s->cntr);
		pScale->set(s->rad * 2.0f, s->rad *2.0f, s->rad *2.0f);
		radius = s->rad;
	}


	void setScale(const Vect& scale)
	{
		float tmp = -10000.0f;

		if (scale.x() > tmp)
		{
			tmp = scale.x();
		}

		if (scale.y() > tmp)
		{
			tmp = scale.y();
		}

		if (scale.z() > tmp)
		{
			tmp = scale.z();
		}

		radius *= tmp;

		//pScale->set(radius*2.0f, radius*2.0f, radius * 2.0f);

	}

	float getRadius()
	{
		return radius;
	}

};

class CollisionObject
{
public:

	CollisionObject(GraphicsObject* gObj, CullingSphere* s)
	{
		poGraphicsObject = gObj;
		poColVol = s;
		//poColSphere = s;
	}

	~CollisionObject()
	{
		poGraphicsObject->deleteShader();
		delete poGraphicsObject;
		delete poColVol;
	}

	void SetWorld(Matrix& world)
	{

		//Matrix Scale(SCALE,Vect( poColSphere->rad*2, poColSphere->rad*2, poColSphere->rad*2));
		//Matrix Transform(TRANS, poColSphere->cntr);

		Matrix Scale(SCALE, *poColVol->getScale());
		Matrix Transform(TRANS, *poColVol->getTrans());

		Matrix Tmp = Scale * Transform * world;

		//Matrix Tmp = Transform * world;
		//Matrix Tmp = Scale * world;
		//pGraphicsObject->SetWorld(Tmp);

		poGraphicsObject->SetWorld(Tmp);
	}

	void Render()
	{
		poGraphicsObject->Render();
	}

	CullingSphere* getCullSphere()
	{
		return poColVol;
	}

	//void setScale(const Vect& scale)
	//{
	//	poColVol->setScale(scale);
	//}

	//find the max of the x y z scale for the bounding sphere
	

private:

	GraphicsObject * poGraphicsObject;
	CullingSphere * poColVol;
	//Sphere* poColSphere;
};
#pragma warning ( pop )
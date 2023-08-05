#pragma once

#include "MathEngine.h"
#include "CollisionObject.h"
//use my mem system with a camera manager and get rid of alignas(16)
//has to be final so no one adds a virtual method and breaks the alignment
class Camera final : public Align16
{

public:

	enum class CameraName
	{
		Not_Initalized,
		DEFAULT,
		View1,
		View2,
		View3
	};

	//default constructor
	//Camera(Heap* pHeap);
	Camera();
	~Camera();

	//setup camera
	void setPerspective(const float FieldOfView_Degs, 
						const float AspectRatio, 
						const float NearClipDistance,
						const float FarClipDistnace);

	void setViewport(const int inX, 
					 const int inY,
					 const int width,
					 const int height);

	void setOrientAndPosition(const Vect &UpVect,
							  const Vect &inLookAt_pt, 
							  const Vect& inPos);

	//update camera system
	void updateCamera(void);

	//Matricies for rendering
	Matrix &getViewMatrix();
	Matrix &getProjMatrix();

	//accessors
	void getPos(Vect &outPos) const;
	void getDir(Vect &outDir) const;
	void getUp(Vect &outUp) const;
	void getLookAt(Vect &outLookAt) const;
	void getRight(Vect &outRight) const;

	int getScreenWidth() const;
	int getScreenHeight() const;

	//No sets for pos dir up look at right because they have to be adjusted together in set and orient position
	void getFieldOfView(float &Value) const;
	void setFieldOfView(const float Value);

	void getNearDist(float &Value) const;
	void setNearDist(const float Value);

	//Helper functions

	void getNearTopLeft(Vect &vOut) const;
	void getNearTopRight(Vect &vOut) const;
	void getNearBottomLeft(Vect &vOut) const;
	void getNearBottomRight(Vect &vOut) const;
	
	void getFarTopLeft(Vect& vOut) const;
	void getFarTopRight(Vect& vOut) const;
	void getFarBottomLeft(Vect& vOut) const;
	void getFarBottomRight(Vect& vOut) const;

	void getFrontNorm(Vect& vOut) const;
	void getBackNorm(Vect& vOut) const;
	void getRightNorm(Vect& vOut) const;
	void getLeftNorm(Vect& vOut) const;
	void getTopNorm(Vect& vOut) const;
	void getBottomNorm(Vect& vOut) const;

	bool cullTest(CullingSphere* cSphere, Matrix* world);

private:

	void privSetViewState();
	void privCalcPlaneHeightWidth();
	void privCalcFrustumVerts();
	void privCalcFrustumCollisionNormals();
	void privUpdateProjectionMatrix();
	void privUpdateViewMatrix();

	//keep data private users cannot be allowed to mess with any of this
	//a lot of it is derived from other things so it has to be changed in a certain way
	//--------------------------------------------------------
	//ALIGNED DATA ONLY BELOW THIS POINT
	//--------------------------------------------------------
	//Projection matricies
	Matrix projMatrix;
	Matrix viewMatrix;

	//camera unit vectors (up, dir right)
	Vect vUp;
	Vect vDir;
	Vect vRight; //derived by up and dir so maybe delete and just do that math on demand? idk have to benchmark
	Vect vPos;

	Vect vLookAt;

	//World space coords for viewFrustum
	Vect nearTopLeft;
	Vect nearTopRight;
	Vect nearBottomLeft;
	Vect nearBottomRight;

	Vect farTopLeft;
	Vect farTopRight;
	Vect farBottomLeft;
	Vect farBottomRight;

	//Normals of the frustum
	Vect frontNorm;
	Vect backNorm;
	Vect rightNorm;

	Vect leftNorm;
	Vect topNorm;
	Vect bottomNorm;

	//--------------------------------------------------------
	//UNALIGNED DATA BELOW THIS POINT
	//--------------------------------------------------------

	//define frustum inputs
	float nearDist;
	float farDist;
	float fovy;
	float aspectRatio;

	//Screensize in world space
	float near_height;
	float near_width;
	float far_height;
	float far_width;

	//to organize
	int viewport_x;
	int viewport_y;
	int viewport_width;
	int viewport_height;
};

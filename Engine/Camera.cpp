#include <assert.h>
#include <math.h>
#include "Camera.h"
#include "MathEngine.h"
#include "GLibSetup.h"

#include "CollisionObject.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

//critical informational knobs for perspective matrix
//Field of View Y is in degrees (because OPenGL is in degrees) eventually switch to radians
//TODO:: switch from degrees to radians hopefully when switching to vulkan or something
//TODO:: SIMD BELOW possibble this line
void Camera::setPerspective(const float _fovy, const float _aspect, const float _nearDist, const float _farDist)
{
	this->aspectRatio = _aspect;
	this->fovy = _fovy;
	this->nearDist = _nearDist;
	this->farDist = _farDist;
}

int Camera::getScreenWidth() const
{
	return this->viewport_width;
}

int Camera::getScreenHeight() const
{
	return this->viewport_height;
}

//Just a pass through to setup the view port screen sub window
void Camera::setViewport(const int inX, const int inY, const int inWidth, const int inHeight)
{
	//updates the viewport variables
	this->viewport_x = inX;
	this->viewport_y = inY;
	this->viewport_width = inWidth;
	this->viewport_height = inHeight;

	//updates on hte GPU
	this->privSetViewState();
}

void Camera::privSetViewState()
{
	glViewport(this->viewport_x, this->viewport_y, this->viewport_width, this->viewport_height);
}

//TODO:: complete this later
//Calculate the near and far heigh width pairs
void Camera::privCalcPlaneHeightWidth()
{
	this->near_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->nearDist;
	this->near_width = this->near_height * this->aspectRatio;

	this->far_height = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->farDist;
	this->far_width = this->far_height * this->aspectRatio;
}

void Camera::setOrientAndPosition(const Vect& inUp, const Vect& inLookAt,const Vect& inPos)
{
	//Remember the up, lookAt and right are unit and are perpendicular.
	//Treat lookAT as king and find right vect then correct Up to insure perpendicular
	//make sure that all vectors are unit vectors
	this->vLookAt = inLookAt;

	//point out of the screen into your eye
	this->vDir = inPos - inLookAt;
	this->vDir.norm();

	this->vRight = inUp.cross(this->vDir);
	this->vRight.norm();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.norm();

	this->vPos = inPos;
}
//TODO:: make sure this is optimized it's already SIMD under the hood
void Camera::privCalcFrustumVerts(void)
{
	// Top Left corner and so forth.  In this form to see the pattern
	// Might be confusing (remember the picture) vDir goes from screen into your EYE
	// so distance from the eye is "negative" vDir
	this->nearTopLeft = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearTopRight = this->vPos - this->vDir * this->nearDist + this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->nearBottomLeft = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f - this->vRight * this->near_width * 0.5f;
	this->nearBottomRight = this->vPos - this->vDir * this->nearDist - this->vUp * this->near_height * 0.5f + this->vRight * this->near_width * 0.5f;
	this->farTopLeft = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farTopRight = this->vPos - this->vDir * this->farDist + this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
	this->farBottomLeft = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f - this->vRight * this->far_width * 0.5f;
	this->farBottomRight = this->vPos - this->vDir * this->farDist - this->vUp * this->far_height * 0.5f + this->vRight * this->far_width * 0.5f;
};


inline bool is_aligned(const void * ptr, unsigned int alignment) 
{
	const unsigned int iptr = reinterpret_cast<const unsigned int>(ptr);
	return !(iptr % alignment);
}

void Camera::privCalcFrustumCollisionNormals(void)
{
	// Normals of the frustum around nearTopLeft
	Vect A = this->nearBottomLeft - this->nearTopLeft;
	Vect B = this->nearTopRight - this->nearTopLeft;
	Vect C = this->farTopLeft - this->nearTopLeft;

	assert(is_aligned(this, 16u));

	assert(is_aligned(&A, 16u));
	assert(is_aligned(&B, 16u));
	assert(is_aligned(&C, 16u));

	assert(is_aligned(&nearBottomLeft, 16u));
	assert(is_aligned(&nearTopLeft, 16u));
	assert(is_aligned(&nearTopRight, 16u));

	assert(is_aligned(&frontNorm, 16u));

	this->frontNorm = A.cross(B);
	this->frontNorm.norm();

	this->leftNorm = C.cross(A);
	this->leftNorm.norm();

	this->topNorm = B.cross(C);
	this->topNorm.norm();

	// Normals of the frustum around farBottomRight
	A = this->farBottomLeft - this->farBottomRight;
	B = this->farTopRight - this->farBottomRight;
	C = this->nearBottomRight - this->farBottomRight;

	this->backNorm = A.cross(B);
	this->backNorm.norm();

	this->rightNorm = B.cross(C);
	this->rightNorm.norm();

	this->bottomNorm = C.cross(A);
	this->bottomNorm.norm();
};

//TODO: SIMD
void Camera::privUpdateProjectionMatrix(void)
{
	this->projMatrix[m0] = 2.0f * this->nearDist / this->near_width;
	this->projMatrix[m1] = 0.0f;
	this->projMatrix[m2] = 0.0f;
	this->projMatrix[m3] = 0.0f;

	this->projMatrix[m4] = 0.0f;
	this->projMatrix[m5] = 2.0f * this->nearDist / this->near_height;
	this->projMatrix[m6] = 0.0f;
	this->projMatrix[m7] = 0.0f;

	this->projMatrix[m8] = 0.0f;
	this->projMatrix[m9] = 0.0f;
	this->projMatrix[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
	this->projMatrix[m11] = -1.0f;

	this->projMatrix[m12] = 0.0f;
	this->projMatrix[m13] = 0.0f;
	this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
	this->projMatrix[m15] = 0.0f;
};


//TODO:: SIMD
void Camera::privUpdateViewMatrix(void)
{
	// This functions assumes the your vUp, vRight, vDir are still unit
	// And perpendicular to each other
	//  view = Rot(orient) * trans(-(eye.basis) )

	this->viewMatrix[m0] = this->vRight[x];
	this->viewMatrix[m1] = this->vUp[x];
	this->viewMatrix[m2] = this->vDir[x];
	this->viewMatrix[m3] = 0.0f;

	this->viewMatrix[m4] = this->vRight[y];
	this->viewMatrix[m5] = this->vUp[y];
	this->viewMatrix[m6] = this->vDir[y];
	this->viewMatrix[m7] = 0.0f;

	this->viewMatrix[m8] = this->vRight[z];
	this->viewMatrix[m9] = this->vUp[z];
	this->viewMatrix[m10] = this->vDir[z];
	this->viewMatrix[m11] = 0.0f;

	// Change of basis (dot with the basis vectors)
	this->viewMatrix[m12] = -vPos.dot(vRight);
	this->viewMatrix[m13] = -vPos.dot(vUp);
	this->viewMatrix[m14] = -vPos.dot(vDir);
	this->viewMatrix[m15] = 1.0f;
};

// Update everything (make sure it's consistent)
void Camera::updateCamera(void)
{
	// First find the near height/width, far height/width
	this->privCalcPlaneHeightWidth();

	// Find the frustum physical verts
	this->privCalcFrustumVerts();

	// find the frustum collision normals
	this->privCalcFrustumCollisionNormals();

	// update the projection matrix
	this->privUpdateProjectionMatrix();

	// update the view matrix
	this->privUpdateViewMatrix();
}

// Accessor mess:
Matrix & Camera::getViewMatrix(void)
{
	return this->viewMatrix;
}

Matrix & Camera::getProjMatrix(void)
{
	return this->projMatrix;
}

void Camera::getPos(Vect &outPos) const
{
	outPos = this->vPos;
}

void  Camera::getDir(Vect &outDir) const
{
	outDir = this->vDir;
}

void  Camera::getUp(Vect &outUp) const
{
	outUp = this->vUp;
}

void Camera::getLookAt(Vect &outLookAt) const
{
	outLookAt = this->vLookAt;
}

void Camera::getRight(Vect &outRight) const
{
	outRight = this->vRight;
}

void Camera::getFieldOfView(float &Value) const
{
	//Value = this->aspectRatio;
	Value = this->fovy;
};

void Camera::setFieldOfView(const float Value)
{
	//this->aspectRatio = Value;
	this->fovy = Value;
};

void Camera::getNearDist(float &Value) const
{
	Value = this->nearDist;
}

void Camera::setNearDist(const float Value)
{
	this->nearDist = Value;
}

void Camera::getNearTopLeft(Vect &vOut) const
{
	vOut = this->nearTopLeft;
}

void Camera::getNearTopRight(Vect &vOut) const
{
	vOut = this->nearTopRight;
}

void Camera::getNearBottomLeft(Vect &vOut)const
{
	vOut = this->nearBottomLeft;
}

void Camera::getNearBottomRight(Vect &vOut) const
{
	vOut = this->nearBottomRight;
}

void Camera::getFarTopLeft(Vect &vOut) const
{
	vOut = this->farTopLeft;
}

void Camera::getFarTopRight(Vect &vOut) const
{
	vOut = this->farTopRight;
}

void Camera::getFarBottomLeft(Vect &vOut) const
{
	vOut = this->farBottomLeft;
}

void Camera::getFarBottomRight(Vect &vOut)const
{
	vOut = this->farBottomRight;
}

void Camera::getFrontNorm(Vect& vOut) const
{
	vOut = this->frontNorm;
}

void Camera::getBackNorm(Vect& vOut) const
{
	vOut = this->backNorm;
}

void Camera::getRightNorm(Vect& vOut) const
{
	vOut = this->rightNorm;
}

void Camera::getLeftNorm(Vect& vOut) const
{
	vOut = this->leftNorm;
}

void Camera::getTopNorm(Vect& vOut) const
{
	vOut = this->topNorm;
}

void Camera::getBottomNorm(Vect& vOut) const
{
	vOut = this->bottomNorm;
}

//culling stuff
float plane_distance(Vect& planePoint, Vect& planeNormal, Vect& testPoint)
{
	return Vect(testPoint - planePoint).dot(planeNormal);
	//return planeNormal.dot(Vect(testPoint - planePoint));
}

bool sphere_inside_plane(Vect& planePoint, Vect& planeNormal, Vect& testPoint, float radius)
{
	return -plane_distance(planePoint, planeNormal, testPoint) > radius;
}

bool sphere_outside_plane(Vect& planePoint, Vect& planeNormal, Vect& testPoint, float radius)
{
	return plane_distance(planePoint, planeNormal, testPoint) > radius;
}

bool sphere_intersects_plane(Vect& planePoint, Vect& planeNormal, Vect& testPoint, float radius)
{
	return fabs(plane_distance(planePoint, planeNormal, testPoint)) <= radius;
}

//TODO::: optimize the shit out of this this is horribble debugging code with shitloads of unecessary tempoarires for each object tested
//maybe pass the game object into the camera? Idk that's bad.
//maybe make a class that the game object manager holds that has the workspace so it doesn't have to be constructed/destructed each object

bool Camera::cullTest(CullingSphere* cSphere, Matrix* world)
{
	bool retval = true;

	Vect ctr = *cSphere->getTrans() * *world;

	float radius = cSphere->getRadius();

	//check for complete containment
	if (sphere_inside_plane(nearBottomLeft, frontNorm, ctr, radius))
	{
		if (sphere_inside_plane(farTopLeft, leftNorm, ctr, radius))
		{
			if (sphere_inside_plane(farTopLeft, topNorm, ctr, radius))
			{
				if (sphere_inside_plane(farTopLeft, backNorm, ctr, radius))
				{
					if (sphere_inside_plane(farTopRight, rightNorm, ctr, radius))
					{
						if (sphere_inside_plane(farBottomLeft, bottomNorm, ctr, radius))
						{
							retval = false;
						}
					}
				}
			}
		}
	}

	//check for intersection if it's not contained in
	if (retval == true)
	{
		bool in_front = !sphere_outside_plane(nearBottomLeft, frontNorm, ctr, radius);
		bool in_left = !sphere_outside_plane(farTopLeft, leftNorm, ctr, radius);
		bool in_top = !sphere_outside_plane(farTopLeft, topNorm, ctr, radius);

		bool in_right = !sphere_outside_plane(farBottomRight, rightNorm, ctr, radius);
		bool in_back = !sphere_outside_plane(farBottomRight, backNorm, ctr, radius);
		bool in_bottom = !sphere_outside_plane(farBottomRight, bottomNorm, ctr, radius);

		if (sphere_intersects_plane(farTopLeft, topNorm, ctr, radius) &&
			in_left && in_right && in_front && in_back) {
			retval = false;
		}
		else if (sphere_intersects_plane(farBottomLeft, bottomNorm, ctr, radius) &&
			in_left && in_right && in_front && in_back) {
			retval = false;
		}
		else if (sphere_intersects_plane(farBottomLeft, leftNorm, ctr, radius) &&
			in_top && in_bottom && in_front && in_back) {
			retval = false;
		}
		else if (sphere_intersects_plane(farBottomRight, rightNorm, ctr, radius) &&
			in_top && in_bottom && in_front && in_back) {
			retval = false;
		}
		else if (sphere_intersects_plane(nearTopRight, frontNorm, ctr, radius) &&
			in_top && in_bottom && in_left && in_right) {
			retval = false;
		}
		else if (sphere_intersects_plane(farTopRight, backNorm, ctr, radius) &&
			in_top && in_bottom && in_left && in_right) {
			retval = false;
		}
	}

	return retval;
}
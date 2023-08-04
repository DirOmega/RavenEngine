#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "MathEngine.h"
#include "Model.h"
//move bounding sphere.h and bounding sphere.cpp to model converter
struct Sphere : public Align16
{
	Vect cntr;
	float rad;
	float pad[3];  // love and hate this new compiler
};

void RitterSphere(Sphere &s, Vect *pt, int numPts);
void RitterSphere(Sphere &s, Vert_xyzuvn* verts, int numPts);

#endif

#include "MathEngine.h"
#include "BoundingSphere.h"
#include "Model.h"
#include <math.h>

#pragma warning( push )
#pragma warning( disable : 5045)//I don't care about spectre attacks

void MostSeparatedPointsOnAABB(int &min, int &max, Vect *pt, int numPts)
{
	// First find most extreme points along principal axes
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;

	for (int i = 1; i < numPts; i++)
	{
		if (pt[i][x] < pt[minx][x])
			minx = i;
		if (pt[i][x] > pt[maxx][x])
			maxx = i;
		if (pt[i][y] < pt[miny][y])
			miny = i;
		if (pt[i][y] > pt[maxy][y])
			maxy = i;
		if (pt[i][z] < pt[minz][z])
			minz = i;
		if (pt[i][z] > pt[maxz][z])
			maxz = i;
	}

	// Given 6pts minx,miny,minz,maxx,maxy,maxz find the greatest distance
	struct tmpNode
	{
		Vect v;
		int index;
		int pad[3];  // padding
	};

	tmpNode data[6];

	data[0].v.set(pt[minx]);
	data[1].v.set(pt[miny]);
	data[2].v.set(pt[minz]);
	data[3].v.set(pt[maxx]);
	data[4].v.set(pt[maxy]);
	data[5].v.set(pt[maxz]);

	data[0].index = minx;
	data[1].index = miny;
	data[2].index = minz;
	data[3].index = maxx;
	data[4].index = maxy;
	data[5].index = maxz;


	float maxDist = (data[0].v - data[1].v).mag();
	int maxA = data[0].index;
	int maxB = data[1].index;
	float dist = maxDist;

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<6; j++)
		{
			dist = (data[i].v - data[j].v).mag();
			//out("dist(%d,%d) = %f\n",i,j,dist);
			if (dist > maxDist)
			{
				//out("--->maxdist(%d,%d) = %f\n",i,j,dist);
				// new max
				maxDist = dist;
				maxA = data[i].index;
				maxB = data[j].index;
			}
		}
	}

	min = maxA;
	max = maxB;
}

void SphereOfSphereAndPt(Sphere &s, Vect &p)
{
	// Compute squared distance between point and sphere center
	Vect d = p - s.cntr;

	float dist2 = d.dot(d);
	// Only update s if point p is outside it
	if (dist2 > s.rad * s.rad)
	{
		float dist = sqrtf(dist2);
		float newRadius = (s.rad + dist) * 0.5f;
		float k = (newRadius - s.rad) / dist;
		s.rad = newRadius;
		s.cntr += d * k;
	}
}

void SphereFromDistantPoints(Sphere &s, Vect *pt, int numPts)
{
	// Find the most separated point pair defining the encompassing AABB
	int min, max;
	MostSeparatedPointsOnAABB(min, max, pt, numPts);

	// Set up sphere to just encompass these two points
	s.cntr = (pt[min] + pt[max]) * 0.5f;
	s.rad = (pt[max] - s.cntr).dot(pt[max] - s.cntr);
	s.rad = sqrtf(s.rad);
}


 
void RitterSphere(Sphere &s, Vect *pt, int numPts)
{
	// Get sphere encompassing two approximately most distant points
	SphereFromDistantPoints(s, pt, numPts);

	// Grow sphere to include all points
	for (int i = 0; i < numPts; i++)
	{
		SphereOfSphereAndPt(s, pt[i]);
	}
}

void MostSeparatedPointsOnAABB(int &min, int &max, Vert_xyzuvn *pt, int numPts)
{
	// First find most extreme points along principal axes
	int minx = 0;
	int maxx = 0;
	int miny = 0;
	int maxy = 0;
	int minz = 0;
	int maxz = 0;

	for (int i = 1; i < numPts; i++)
	{
		if (pt[i].x < pt[minx].x)
			minx = i;
		if (pt[i].x > pt[maxx].x)
			maxx = i;
		if (pt[i].y < pt[miny].y)
			miny = i;
		if (pt[i].y > pt[maxy].y)
			maxy = i;
		if (pt[i].z < pt[minz].z)
			minz = i;
		if (pt[i].z > pt[maxz].z)
			maxz = i;
	}

	// Given 6pts minx,miny,minz,maxx,maxy,maxz find the greatest distance
	struct tmpNode
	{
		Vect v;
		int index;
		int pad[3];  // padding
	};

	tmpNode data[6];

	//Vect vMin(pt[min].x, pt[min].y, pt[min].z);
	//Vect vMax(pt[max].x, pt[max].y, pt[max].z);

	Vect vMinx(pt[minx].x, pt[minx].y, pt[minx].z);
	Vect vMiny(pt[miny].x, pt[miny].y, pt[miny].z);
	Vect vMinz(pt[minz].x, pt[minz].y, pt[minz].z);

	Vect vMaxx(pt[maxx].x, pt[maxx].y, pt[maxx].z);
	Vect vMaxy(pt[maxy].x, pt[maxy].y, pt[maxy].z);
	Vect vMaxz(pt[maxz].x, pt[maxz].y, pt[maxz].z);

	data[0].v.set(vMinx);
	data[1].v.set(vMiny);
	data[2].v.set(vMinz);

	data[3].v.set(vMaxx);
	data[4].v.set(vMaxy);
	data[5].v.set(vMaxz);

	data[0].index = minx;
	data[1].index = miny;
	data[2].index = minz;

	data[3].index = maxx;
	data[4].index = maxy;
	data[5].index = maxz;

	float maxDist = (data[0].v - data[1].v).mag();
	int maxA = data[0].index;
	int maxB = data[1].index;
	float dist = maxDist;

	for (int i = 0; i<6; i++)
	{
		for (int j = 0; j<6; j++)
		{
			dist = (data[i].v - data[j].v).mag();
			//out("dist(%d,%d) = %f\n",i,j,dist);
			if (dist > maxDist)
			{
				//out("--->maxdist(%d,%d) = %f\n",i,j,dist);
				// new max
				maxDist = dist;
				maxA = data[i].index;
				maxB = data[j].index;
			}
		}
	}

	min = maxA;
	max = maxB;
}


void SphereFromDistantPoints(Sphere &s, Vert_xyzuvn *verts, int numPts)
{
	// Find the most separated point pair defining the encompassing AABB
	int min, max;

	MostSeparatedPointsOnAABB(min, max, verts, numPts);

	Vect vMin(verts[min].x,verts[min].y,verts[min].z);
	Vect vMax(verts[max].x, verts[max].y, verts[max].z);

	// Set up sphere to just encompass these two points
	s.cntr = (vMin + vMax) * 0.5f;
	s.rad = (vMax - s.cntr).dot(vMax - s.cntr);
	s.rad = sqrtf(s.rad);
}


void SphereOfSphereAndPt(Sphere &s, Vert_xyzuvn &p)
{
	// Compute squared distance between point and sphere center
	Vect d = Vect(p.x,p.y,p.z) - s.cntr;

	float dist2 = d.dot(d);
	// Only update s if point p is outside it
	if (dist2 > s.rad * s.rad)
	{
		float dist = sqrtf(dist2);
		float newRadius = (s.rad + dist) * 0.5f;
		float k = (newRadius - s.rad) / dist;
		s.rad = newRadius;
		s.cntr += d * k;
	}
}

void RitterSphere(Sphere &s, Vert_xyzuvn* verts, int numPts)
{
	if (numPts > 0)
	{
		SphereFromDistantPoints(s, verts, numPts);

		// Grow sphere to include all points
		for (int i = 0; i < numPts; i++)
		{
			SphereOfSphereAndPt(s, verts[i]);
		}
	}
}
#pragma warning( pop )

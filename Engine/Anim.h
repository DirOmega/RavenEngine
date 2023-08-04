#ifndef ANIM_H
#define ANIM_H

#include "Timer.h"
#include "FrameBucket.h"
#include "Vect.h"
#include "Quat.h"

#include "MathEngine.h"
#include "GameObject.h"

#include <string.h>

#define NUM_BONES 20
//
//struct Bone //: public Align16
//{
//	Vect  T;
//	Quat  Q;
//	Vect  S;
//};
//
//struct Frame_Bucket
//{
//	Frame_Bucket *nextBucket;  // 4
//	Frame_Bucket *prevBucket;  // 4
//	Time		  KeyTime;     // 8 bytes  <--- dictates alignment
//	Bone		  *pBone;      // 4
//	int           pad;
//};

class SkellyBone
{
public:

	SkellyBone(int _index, int _parentIndex, const char* nameBuffer)
		: index(_index), parentIndex(_parentIndex)
	{
		strcpy_s(name, nameBuffer);

		for (int i = 0; i < 24; i++)
		{
			if (32 == name[i])
			{//convert spaces in the name to underscores
				name[i] = 95;
			}
		}
	}

	SkellyBone()
	{
	}

	~SkellyBone()
	{//do nothing no dynamic data
	}

	SkellyBone(const SkellyBone& rhs)
		: index(rhs.index), parentIndex(rhs.parentIndex)
	{//copy constructor
		strcpy_s(name, rhs.name);
	}

	SkellyBone& operator = (const SkellyBone& rhs)
	{
		index = rhs.index;
		parentIndex = rhs.parentIndex;
		strcpy_s(name, rhs.name);
		return *this;
	}

	int index;
	int parentIndex;
	char name[24];

};


void SetAnimationHierarchy(Heap* pHeap);
void SetAnimationData(Heap* pHeap);
//void SetAnimationPose(Heap* pHeap, GameObject* pNode);
void SetAnimationPose(GameObject *root, const Time& tCurr);


void ProcessAnimation(const Time& t);
void FindMaxTime(Time &tMax);



#endif
#ifndef FRAME_BUCKET
#define FRAME_BUCKET

#include "RvnTimer.h"
#include "Vect.h"
#include "Quat.h"
#include "DLink.h"

//#include "Align16.h"

struct Bone : Align16
{
	Vect  T;
	Quat  Q;
	Vect  S;

	bool operator !=(const Bone& rhs)
	{
		return this->T.x() != rhs.T.x() ||
			this->T.y() != rhs.T.y() ||
			this->T.z() != rhs.T.z() ||
			this->Q.qx() != rhs.Q.qx() ||
			this->Q.qy() != rhs.Q.qy() ||
			this->Q.qz() != rhs.Q.qz() ||
			this->Q.real() != rhs.Q.real() ||
			this->S.x() != rhs.S.x() ||
			this->S.y() != rhs.S.y() ||
			this->S.z() != rhs.S.z();
	}
};

struct Frame_Bucket 
{
	Frame_Bucket *nextBucket;
	Frame_Bucket *prevBucket;
	Time		  KeyTime;
	Bone		  *pBone;
	char		  pad[4];

	Frame_Bucket() : nextBucket(0), prevBucket(0), pBone(0)
	{}

	~Frame_Bucket()
	{
		if (pBone != 0)
		{
			delete[] pBone;
		}
	}
};

#endif

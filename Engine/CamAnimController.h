#pragma once

#include "baseManager.h"
#include "MathEngine.h"

#include "PCSTreeForwardIterator.h"
#include "GraphicsObject_Null.h"
#include "NullModel.h"
#include "GameObjectRigid.h"

#include "File.h"
#include "CamVals.h"


struct CamFrame_Bucket : Align16
{
	CamFrame_Bucket *nextBucket;
	CamFrame_Bucket *prevBucket;
	Time		  KeyTime;
	CamVals       camValues;
	//char		  pad[4];

	CamFrame_Bucket() : nextBucket(0), prevBucket(0)
	{}

	~CamFrame_Bucket()
	{}
};

class CamClip : public DLink
{
public:

	~CamClip()
	{
		dClean();
	}

	//unsigned int clipID;
	//GraphicsObject_SkinShapeDefo* target;
	bool pad[4];
	Time maxTime;
	int numShapes;
	CamFrame_Bucket* pHead;

	//bool pad2[4];

	void dClean()
	{
		//clipID = 0;
		maxTime = Time(Time::ZERO);

		if (pHead != 0)
		{
			CamFrame_Bucket* tmp = pHead->nextBucket;
			while (pHead != 0)
			{
				tmp = pHead->nextBucket;
				delete pHead;
				pHead = tmp;
			}
		}
		//delete pHead; 
	}

};

class CamAnimController : public baseManager
{
public:

	bool	pad1[4];
	Time    internalTime;

	CamClip*   pCurClip;
	int		curAnims;
	//bool pad2[4];


	CamAnimController(const char* camFileName)
		: internalTime(Time::ZERO), pCurClip(0)// pClipHead(0), pCurClip(0)
	{
		camTrackFromFile(camFileName);
		Trace::out("end CamAnim Constructor\n");
	}

	CamAnimController(const CamAnimController&) = delete;
	CamAnimController& operator = (const CamAnimController&) = delete;

	~CamAnimController()
	{
		//delete pTestcompute;
	}

	void nextAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (CamClip*)pActiveHead;
		}
		else
		{
			pCurClip = (CamClip*)pCurClip->pNext;
		}
	}

	void prevAnim()
	{
		if (pCurClip->pNext == nullptr)
		{
			pCurClip = (CamClip*)pActiveHead;
		}
		else
		{
			pCurClip = (CamClip*)pCurClip->pNext;
		}
	}

	void camTrackFromFile(const char* fileName)
	{

		File::Handle fh;
		if (File::Error::SUCCESS == File::Open(fh, fileName, File::Mode::READ))
		{

			pCurClip = new CamClip();

			addToActive((DLink&)*pCurClip);

			unsigned int frameCount = 0;

			File::Read(fh, &frameCount, sizeof(unsigned int));
			assert(frameCount != 0);

			/*pCurClip->maxTime = ((int)frameCount) * Time(Time::NTSC_30_FRAME);*/

			// --------  Result Frame  ----------------------

			pCurClip->pHead = new CamFrame_Bucket();
			pCurClip->pHead->prevBucket = 0;
			pCurClip->pHead->nextBucket = 0;
			pCurClip->pHead->KeyTime = Time(Time::ZERO);
			//pCurClip->pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[boneCount];
			//pCurClip->pHead->pBone = new Bone[boneCount];

			CamFrame_Bucket* prev = pCurClip->pHead;
			Time MaxTime = Time(Time::ZERO);

			unsigned int keyFrameIndex = 0;
			//HACKY signed unsigned mix here because there's no unsigned operator in the TIME lib yet.
			for (int i = 0; i < (int)frameCount; i++)
			{
				CamFrame_Bucket *pTmp = new CamFrame_Bucket();
				pTmp->prevBucket = prev;
				pTmp->nextBucket = 0;

				File::Read(fh, &keyFrameIndex, sizeof(unsigned int));

				pTmp->KeyTime = Time(Time::NTSC_30_FRAME) * keyFrameIndex;

				//pTmp->pBone = new(pMyHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[boneCount];
				//pTmp->pBone = new Bone[boneCount];

				prev->nextBucket = pTmp;

				prev = pTmp;

				File::Read(fh, &pTmp->camValues, sizeof(CamVals));

				//TODO:: reorder the file so I can read the entire struct in one call
				//File::Read(fh, &pTmp->camValues.distance, sizeof(float));
				//File::Read(fh, &pTmp->camValues.px, sizeof(float) * 3); //reads px,py,pz
				//
				//File::Read(fh, &pTmp->camValues.camAngles, sizeof(float) * 3);
				//File::Read(fh, &pTmp->camValues.fov, sizeof(float));
			}

			assert(keyFrameIndex != 0);
			pCurClip->maxTime = Time(Time::NTSC_30_FRAME) * keyFrameIndex;

			File::Close(fh);

			//processAnimHelper(Time(Time::ZERO), pCurClip);
		}
		else
		{
			assert(false);
		}
	}

	void FindMaxTime(Time &tMax)
	{
		CamFrame_Bucket *pTmp = pCurClip->pHead->nextBucket;

		while (pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;
	}

	float fLerp(const float a, const float b, float t)
	{
		return a + t * (b - a);
	}

	void processAnimHelper(const Time& tCurr, CamClip* pClip)
	{

		CamFrame_Bucket* pHead = pClip->pHead;
		// Note: these static should be tied into an animation system

		CamFrame_Bucket *pTmp = pHead->nextBucket;

		// Get the result bone array
		// Remember the firs Frame is the result
		//Bone *bResult = pHead->pBone;

		// First one is the result, skip it
		pTmp = pHead->nextBucket;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime  && pTmp->nextBucket != 0)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		CamFrame_Bucket *pA = pTmp->prevBucket;
		CamFrame_Bucket *pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		if (tS > 1.0f)
		{
			tS = 1.0f;
		}

		//TODO:: figure out what interp data I need where also maybe i need to do the slope formula for the interp data
		//TODO:: probably move to the converter and sample each keyframe and then compress down to a series of linear curves.
		//I think I have to do more math here, because I need to get 
		//pHead->camValues.px = bezLerp(pA->camValues.px, pB->camValues.px, pA->camValues.interpData[0], pA->camValues.interpData[2],tS);
		//pHead->camValues.py = bezLerp(pA->camValues.py, pB->camValues.py, pA->camValues.interpData[1], pA->camValues.interpData[3],tS);
		//pHead->camValues.pz = bezLerp(pA->camValues.pz, pB->camValues.pz, pA->camValues.interpData[0], pA->camValues.interpData[2],tS);

		pHead->camValues.px = fLerp(pA->camValues.px, pB->camValues.px, bezLerp(0,0, pA->camValues.interpData[0], pA->camValues.interpData[1], pA->camValues.interpData[2], pA->camValues.interpData[3], 127,127,  tS));
		pHead->camValues.py = fLerp(pA->camValues.py, pB->camValues.py, bezLerp(0,0, pA->camValues.interpData[4], pA->camValues.interpData[5], pA->camValues.interpData[6], pA->camValues.interpData[7], 127,127,  tS));
		pHead->camValues.pz = fLerp(pA->camValues.pz, pB->camValues.pz, bezLerp(0,0, pA->camValues.interpData[8], pA->camValues.interpData[9], pA->camValues.interpData[10], pA->camValues.interpData[11], 127,127,tS));

		/*pHead->camValues.rx = bezLerp(pA->camValues.rx, pB->camValues.rx, pA->camValues.interpData[12], pA->camValues.interpData[14], tS);
		pHead->camValues.ry = bezLerp(pA->camValues.ry, pB->camValues.ry, pA->camValues.interpData[13], pA->camValues.interpData[15], tS);
		pHead->camValues.rz = bezLerp(pA->camValues.rz, pB->camValues.rz, pA->camValues.interpData[12], pA->camValues.interpData[14], tS);*/

		pHead->camValues.rx = fLerp(pA->camValues.rx, pB->camValues.rx, bezLerp(0,0,pA->camValues.interpData[12],pA->camValues.interpData[13], pA->camValues.interpData[14], pA->camValues.interpData[15],127,127, tS));
		pHead->camValues.ry = fLerp(pA->camValues.ry, pB->camValues.ry, bezLerp(0,0,pA->camValues.interpData[12],pA->camValues.interpData[13], pA->camValues.interpData[14], pA->camValues.interpData[15],127,127, tS));
		pHead->camValues.rz = fLerp(pA->camValues.rz, pB->camValues.rz, bezLerp(0,0,pA->camValues.interpData[12],pA->camValues.interpData[13], pA->camValues.interpData[14], pA->camValues.interpData[15],127,127, tS));

		//pHead->camValues.distance = fLerp(pA->camValues.distance, pB->camValues.distance, tS);
		//pHead->camValues.fov =      fLerp(pA->camValues.fov, pB->camValues.fov, tS);

		//pHead->camValues.distance = bezLerp(pA->camValues.distance, pB->camValues.distance, pA->camValues.interpData[16], pA->camValues.interpData[18], tS);
		
		//pHead->camValues.distance = fLerp(pA->camValues.distance, pB->camValues.distance, bezLerp(0, 0, pA->camValues.interpData[16], pA->camValues.interpData[17], pB->camValues.interpData[18], pB->camValues.interpData[19], 127, 127, tS));

		pHead->camValues.distance = fLerp(pA->camValues.distance, pB->camValues.distance, bezLerp(0, 0, pA->camValues.interpData[16], pA->camValues.interpData[17], pB->camValues.interpData[18], pB->camValues.interpData[19], 127, 127, tS));

		pHead->camValues.fov =      fLerp(pA->camValues.fov, pB->camValues.fov,bezLerp(0,0,pA->camValues.interpData[20], pA->camValues.interpData[21], pA->camValues.interpData[22], pA->camValues.interpData[23], 127,127, tS));
		
		// interpolate ahoy!
		//VectApp::Lerp(pHead->camValues.camPos, pA->camValues.camPos, pB->camValues.camPos, tS);
		//VectApp::Lerp(pHead->camValues.camAngles, pA->camValues.camAngles, pB->camValues.camAngles, tS);
		//pHead->camValues.distance = fLerp(pA->camValues.distance, pB->camValues.distance, tS);
		//pHead->camValues.fov = fLerp(pA->camValues.fov, pB->camValues.fov, tS);
		
		CameraManager::GetActiveCameraLink()->PushAnimVals(&pHead->camValues);
	}

	//TODO:: optimize the shit out of this I made it super messy so I can make sure I know what's going on;
	//I could be using vectors and doing this in simd as well.
	//TODO:: it might be worth noting that i may need to change the ordering for the CP's becuase it stated online that P0 is start and P3 is last.
	float bezLerp(char P0x, char P0y, char P1x, char P1y, char P2x, char P2y, char P3x, char P3y, float t)
	{

		UNUSED_VAR(P3x);
		UNUSED_VAR(P2x);
		UNUSED_VAR(P0x);
		UNUSED_VAR(P1x);


		float x0 = P0x;
		float y0 = P0y;
		float cx = 3.0f*(P1x - P0x);
		float bx = 3.0f*(P2x - P1x) - cx;
		float ax = P3x - P0x - cx - bx;
		float cy = 3.0f*(P1y - P0y);
		float by = 3.0f*(P2y - P1y) - cy;
		float ay = P3y - P0y - cy - by;

		float t2 = t * t;
		float t3 = t2 * t;

		float out = ax * t3 + bx * t2 + cx * t + x0;
		out+= ay * t3 + by * t2 + cy * t + y0;

		//float out = ay * t3 + by * t2 + cy * t + y0;
		//out /= 127;
		out /= 254;
		//I might need to sum these
		return out;

		//return ax * t3 + bx * t2 + cx * t + x0;

		//return powf((1 - t), 3) * P0 + 3 * t* powf((1 - t), 2) * P2 + 3 * powf(t, 2) * (1 - t)*P3 + powf(t, 3) * P1;

		//return powf((1 - t), 3) * P0 + 3 * t* powf((1 - t), 2) * P1 + 3 * powf(t, 2) * (1 - t)*P2 + powf(t, 3) * P3;
	}


	//void CProcessAnimation(const Time&)
	void CProcessAnimation(const float deltaT)
	{
		//shapeBlend(internalTime, pCurShapeClip);
		//		pTestcompute->Dispatch();
		processAnimHelper(internalTime, pCurClip);

		//processAnimHelper(Time(Time::ZERO), pCurClip);

		Time deltaTime = deltaT * Time(Time::NTSC_30_FRAME);
		Time maxTime;

		internalTime += deltaTime;

		FindMaxTime(maxTime);

		static const Time tZero(Time::ZERO);

		// protection for time values for looping
		if (internalTime < tZero)
		{
			internalTime = maxTime;
		}
		else if (internalTime > maxTime)
		{
		//	internalTime = tZero;
		}
		else
		{
			// do nothing
		}

	}

	DLink* dCreateNode()
	{
		DLink* newNode = (DLink*) new Clip();
		assert(newNode != nullptr);
		return newNode;
	}

	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB)
	{
		bool retval = false;
		if (((CameraLink*)&pLinkA)->GetName() == ((CameraLink*)&pLinkB)->GetName())
		{
			retval = true;
		}

		return retval;
	}

	void dClearNode(DLink* pLink)
	{
		pLink->dClean();
	}
};
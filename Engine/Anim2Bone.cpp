#include <assert.h>

#include "PyramidModel.h"
#include "GraphicsObject_TextureLight.h"
#include "GameObjectMan.h"

#include "Anim.h"
#include "FrameBucket.h"
#include "ShaderObject.h"
#include "PCSTreeForwardIterator.h"

#include "File.h"

#define BONE_WIDTH 8.0f

//void setBonePose(GameObjectAnim *node);
//void walk_anim_node(GameObject* node);

//Frame_Bucket *pHead = 0;
//
//GameObjectAnim* pFirstBone;
//
//PCSNode *root = 0;

//class SkellyBone
//{
//public:
//
//	SkellyBone(int _index, int _parentIndex, const char* nameBuffer)
//		: index(_index), parentIndex(_parentIndex)
//	{
//		strcpy_s(name, nameBuffer);
//
//		for (int i = 0; i < 24; i++)
//		{
//			if (32 == name[i])
//			{//convert spaces in the name to underscores
//				name[i] = 95;
//			}
//		}
//	}
//
//	SkellyBone() 
//	{ 
//	}
//
//	~SkellyBone() 
//	{//do nothing no dynamic data
//	}
//
//	SkellyBone(const SkellyBone& rhs)
//		: index(rhs.index), parentIndex(rhs.parentIndex)
//	{//copy constructor
//		strcpy_s(name, rhs.name);
//	}
//
//	SkellyBone& operator = (const SkellyBone& rhs)
//	{
//		index = rhs.index;
//		parentIndex = rhs.parentIndex;
//		strcpy_s(name, rhs.name);
//	}
//
//	int index;
//	int parentIndex;
//	char name[24];
//
//};

//void SetAnimationHierarchy(Heap* goHeapPtr)
//{
//	// Load the model
//	PyramidModel *pPyramidModel = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) PyramidModel("pyramidModel.azul");
//	assert(pPyramidModel);
//
//	// Create/Load Shader 
//	ShaderObject *pShaderObject_textureLight = new( goHeapPtr , Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
//	assert(pShaderObject_textureLight);
//
//	// GraphicsObject for a specific instance
//	GraphicsObject_TextureLight *pGraphics_TextureLight;
//
//	// Create GameObject
//	Vect color(1.50f, 1.50f, 1.50f, 1.5f);
//	Vect pos(-1.0f, -1.0f, -1.0f);
//
//	PCSTree *pTree = GameObjectMan::GetPCSTree();
//	root = pTree->GetRoot();
//
//	File::Handle fh;
//
//	SkellyBone* boneArray;
//
//	GameObjectAnim* pBObj;
//
//	GameObjectAnim** GameObjectList = 0;
//
//	if (File::Error::SUCCESS == File::Open(fh, "teddy.skl", File::Mode::READ))
//	{
//
//		unsigned int BoneCount = 0;
//
//		File::Read(fh, &BoneCount, sizeof(int));
//
//		boneArray = new SkellyBone[BoneCount];
//		GameObjectList = new GameObjectAnim*[BoneCount];
//
//		File::Read(fh, boneArray, BoneCount * sizeof(SkellyBone));
//
//		pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, goHeapPtr);
//		pBObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, goHeapPtr);
//		pBObj->SetIndex(boneArray[0].index);
//		pBObj->SetName(boneArray[0].name);
//		GameObjectMan::Add(pBObj, GameObjectMan::GetRoot());
//
//		GameObjectList[0] = pBObj;
//		pFirstBone = pBObj;
//
//		for (unsigned int i = 1; i < BoneCount; i++)
//		{
//
//			pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, goHeapPtr);
//			pBObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectAnim(pGraphics_TextureLight, goHeapPtr);
//			pBObj->SetIndex(boneArray[i].index);
//			pBObj->SetName(boneArray[i].name);
//			GameObjectMan::Add(pBObj, GameObjectList[boneArray[i].parentIndex]);
//
//			GameObjectList[i] = pBObj;
//
//		}
//		
//		File::Close(fh);
//
//	}
//
//	if (GameObjectList != 0)
//	{
//		delete GameObjectList;
//	}
//	
//	pTree->Print();
//
//}

//void SetAnimationPose(GameObject* inNode, const Time&)
//{
//
//	// REMEMBER:
//	//    that the bone pose are WORLD not GLOBAL
//	//    Animations are relative!
//
//	// First thing, get the first frame of animation
//	//Time tCurr(Time::ZERO);
//
//	//ProcessAnimation(tCurr);
//
//	//calls update on every object //TODO: optimize this away there has to be a better way.
//	//GameObjectMan::Update(tCurr);
//
//	PCSTreeForwardIterator pItr(inNode);
//	PCSNode *pNode = pItr.First();
//	GameObjectAnim *pGameObj = 0;
//
//	while (!pItr.IsDone())
//	{
//		assert(pNode);
//
//		pGameObj = (GameObjectAnim *)pNode;
//		setBonePose(pGameObj);
//
//		pNode = pItr.Next();
//	}
//
//	//GameObject *pChildNode = (GameObject *)pNode->GetChild();
//	//GameObjectAnim*pParentNode = (GameObjectAnim*)pNode;
//
//	//walk_anim_node(pNode);
//}

//void setBonePose(GameObjectAnim *node)
//{
//	// Now get the world matrices
//	GameObjectAnim * childNode  = (GameObjectAnim *)node;
//	GameObjectAnim * parentNode = (GameObjectAnim *)node->GetParent();
//
//	if (parentNode == root)
//		return;
//
//	if (parentNode != 0 && childNode != 0)
//	{
//		// starting point
//		Vect start(0.0f, 0.0f, 0.0f);
//
//		//  At this point, Find the two bones initial positions in world space
//		//  Now get the length and directions
//		Vect ptA = start * *parentNode->GetWorld();
//		Vect ptB = start * *childNode->GetWorld();
//
//		// direction between the anchor points of the respective bones
//		Vect dir = (ptA - ptB);
//
//		// length of the bone 0
//		float mag = dir.mag();
//
//		Matrix S(SCALE, BONE_WIDTH, BONE_WIDTH, mag);
//		Quat Q(ROT_ORIENT, dir.getNorm(), Vect(0.0f, 1.0f, 0.0f) * *parentNode->GetWorld() );
//		Matrix T(TRANS, ptB);
//		Matrix BoneOrient = S * Q * T;
//		
//
//		//Matrix BoneOrient = S;
//		childNode->SetBoneOrientation(BoneOrient);
//	}
//
//	//// deal with last node, when there isn't a terminal node
//	//if (parentNode != 0 && childNode == 0)
//	//{
//	//	// get the parent's parent  -> grandParent
//	//	GameObjectAnim*grandParentNode = (GameObjectAnim*)parentNode->GetParent();
//	//	//parentNode->SetBoneOrientation(grandParentNode->GetBoneOrientation);
//
//	//	Matrix BoneOrient = grandParentNode->GetBoneOrientation();
//
//	//	parentNode->SetBoneOrientation(BoneOrient);
//	//}
//
//}


//void SetAnimationData(Heap* pHeap)
//{
//	File::Handle fh;
//	if (File::Error::SUCCESS == File::Open(fh, "walk0.ani", File::Mode::READ))
//	{
//
//		unsigned int frameCount = 0;
//		unsigned int boneCount = 0;
//		File::Read(fh, &frameCount, sizeof(unsigned int));
//		File::Read(fh, &boneCount, sizeof(unsigned int));
//		assert(frameCount != 0);
//		assert(boneCount != 0);
//
//
//		// --------  Result Frame  ----------------------
//
//		pHead = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//		pHead->prevBucket = 0;
//		pHead->nextBucket = 0;
//		pHead->KeyTime = Time(Time::ZERO);
//		pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[boneCount];
//
//		Frame_Bucket* prev = pHead;
//
//		//HACKY signed unsigned mix here because there's no unsigned operator in the TIME lib yet.
//		for (int i = 0; i < (int) frameCount; i++)
//		{
//
//			Frame_Bucket *pTmp = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//			pTmp->prevBucket = prev;
//			pTmp->nextBucket = 0;
//			pTmp->KeyTime = i * Time(Time::NTSC_30_FRAME);
//			pTmp->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[boneCount];
//			prev->nextBucket = pTmp;
//
//			prev = pTmp;
//
//
//
//			File::Read(fh, pTmp->pBone, sizeof(Bone) * boneCount);
//
//			for (unsigned int j = 0; j < boneCount; j++)
//			{
//				pTmp->pBone[j].S = Vect(1.0f, 1.0f, 1.0f);
//			}
//
//		}
//
//
//	}
//	else
//	{
//		assert(false);
//	}
//
//
//
//
//
//	//// --------  Frame 0  ----------------------------
//
//	//Frame_Bucket *pTmp = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	//pTmp->prevBucket = pHead;
//	//pTmp->nextBucket = 0;
//	//pTmp->KeyTime = 0 * Time(Time::NTSC_30_FRAME);
//	//pTmp->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[NUM_BONES];
//	//pHead->nextBucket = pTmp;
//
//	////------- Key Time: 0  ms: 0 ------------------ 
//
//	////     Node Name: RootNode
//	////pTmp->pBone[-1].T = Vect( 0.0f, 0.0f, 0.0f );
//	////pTmp->pBone[-1].Q = Quat( ROT_XYZ, 0.0f, 0.0f, 0.0f );
//	////pTmp->pBone[-1].S = Vect( 1.0f, 1.0f, 1.0f );
//
//	////     Node Name: Bip01
//	//pTmp->pBone[0].T = Vect(0.970236f, -2.711161f, 1.899864f);
//	//pTmp->pBone[0].Q = Quat(ROT_XYZ, 1.624573f, -1.264373f, 3.121366f);
//	//pTmp->pBone[0].S = Vect(0.100000f, 0.100000f, 0.100000f);
//
//	//// Keenan HACK - set to 1.0f
//	//pTmp->pBone[0].S = Vect(1.00000f, 1.00000f, 1.00000f);
//
//	////     Node Name: Bip01 Pelvis
//	//pTmp->pBone[1].T = Vect(0.0f, 0.0f, 0.0f);
//	//pTmp->pBone[1].Q = Quat(ROT_XYZ, 0.0f, 0.0f, 0.0f);
//	//pTmp->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//	////     Node Name: Bip01 Spine
//	//pTmp->pBone[2].T = Vect(6.016934f, -0.014115f, -0.000028f);
//	//pTmp->pBone[2].Q = Quat(ROT_XYZ, -0.047765f, 0.020883f, 0.010792f);
//	//pTmp->pBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Thigh
//	//pTmp->pBone[3].T = Vect(-6.300546f, -0.567644f, 13.531080f);
//	//pTmp->pBone[3].Q = Quat(ROT_XYZ, 3.060995f, 0.095672f, -3.015697f);
//	//pTmp->pBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Calf
//	//pTmp->pBone[4].T = Vect(16.961559f, -0.000002f, -0.000004f);
//	//pTmp->pBone[4].Q = Quat(ROT_XYZ, -0.348718f, -0.021303f, -0.650505f);
//	//pTmp->pBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Foot
//	//pTmp->pBone[5].T = Vect(13.263550f, 0.000003f, 0.000002f);
//	//pTmp->pBone[5].Q = Quat(ROT_XYZ, -0.103780f, -0.201808f, 0.767066f);
//	//pTmp->pBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Thigh
//	//pTmp->pBone[6].T = Vect(-5.729567f, 0.737561f, -13.774520f);
//	//pTmp->pBone[6].Q = Quat(ROT_XYZ, 3.118021f, 0.054987f, 2.213239f);
//	//pTmp->pBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Calf
//	//pTmp->pBone[7].T = Vect(16.961571f, 0.000003f, -0.000003f);
//	//pTmp->pBone[7].Q = Quat(ROT_XYZ, -0.023962f, 0.004592f, -0.865707f);
//	//pTmp->pBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Foot
//	//pTmp->pBone[8].T = Vect(13.263530f, -0.000000f, 0.000002f);
//	//pTmp->pBone[8].Q = Quat(ROT_XYZ, -0.001414f, 0.038321f, 0.268111f);
//	//pTmp->pBone[8].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Spine1
//	//pTmp->pBone[9].T = Vect(17.730431f, -0.017629f, 0.000002f);
//	//pTmp->pBone[9].Q = Quat(ROT_XYZ, -0.095342f, 0.042221f, 0.018989f);
//	//pTmp->pBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Neck
//	//pTmp->pBone[10].T = Vect(22.140881f, -0.008821f, 0.000003f);
//	//pTmp->pBone[10].Q = Quat(ROT_XYZ, 0.204811f, 0.010239f, -0.060075f);
//	//pTmp->pBone[10].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 L Clavicle
//	//pTmp->pBone[11].T = Vect(-0.056449f, 1.119313f, 5.345238f);
//	//pTmp->pBone[11].Q = Quat(ROT_XYZ, 1.012052f, -1.036376f, 2.183302f);
//	//pTmp->pBone[11].S = Vect(0.999999f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L UpperArm
//	//pTmp->pBone[12].T = Vect(13.067860f, 0.000004f, -0.000004f);
//	//pTmp->pBone[12].Q = Quat(ROT_XYZ, -0.141163f, 0.660794f, 0.057318f);
//	//pTmp->pBone[12].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Forearm
//	//pTmp->pBone[13].T = Vect(15.099380f, -0.012400f, 0.002757f);
//	//pTmp->pBone[13].Q = Quat(ROT_XYZ, 0.645437f, 0.044597f, -0.613745f);
//	//pTmp->pBone[13].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Hand
//	//pTmp->pBone[14].T = Vect(15.099410f, 0.000082f, 0.000072f);
//	//pTmp->pBone[14].Q = Quat(ROT_XYZ, -1.583676f, 0.166629f, -0.123964f);
//	//pTmp->pBone[14].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Clavicle
//	//pTmp->pBone[15].T = Vect(0.055393f, -1.102072f, -5.348833f);
//	//pTmp->pBone[15].Q = Quat(ROT_XYZ, -0.402011f, 1.128657f, 2.673232f);
//	//pTmp->pBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R UpperArm
//	//pTmp->pBone[16].T = Vect(13.067840f, -0.000001f, 0.000001f);
//	//pTmp->pBone[16].Q = Quat(ROT_XYZ, -0.103288f, -0.809167f, 0.449669f);
//	//pTmp->pBone[16].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 R Forearm
//	//pTmp->pBone[17].T = Vect(15.099410f, -0.000577f, -0.000088f);
//	//pTmp->pBone[17].Q = Quat(ROT_XYZ, -0.818463f, -0.029294f, -0.231554f);
//	//pTmp->pBone[17].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Hand
//	//pTmp->pBone[18].T = Vect(15.099400f, 0.000180f, -0.000104f);
//	//pTmp->pBone[18].Q = Quat(ROT_XYZ, 1.566849f, -0.070789f, -0.100040f);
//	//pTmp->pBone[18].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Head
//	//pTmp->pBone[19].T = Vect(11.079190f, 0.000001f, 0.000001f);
//	//pTmp->pBone[19].Q = Quat(ROT_XYZ, 0.191348f, -0.036722f, 0.254348f);
//	//pTmp->pBone[19].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//
//	//// --------  Frame 3  ----------------------------
//
//	//Frame_Bucket *pTmp2 = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Frame_Bucket();
//	//pTmp2->prevBucket = pTmp;
//	//pTmp2->nextBucket = 0;
//	//pTmp2->KeyTime = 3 * Time(Time::NTSC_30_FRAME);
//	//pTmp2->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__)Bone[NUM_BONES];
//	//pTmp->nextBucket = pTmp2;
//
//	//// ------- Key Time: 3 ms: 100 ------------------ 
//
//	////     Node Name: RootNode
//	////pTmp2->pBone[-1].T = Vect( 0.0f, 0.0f, 0.0f );
//	////pTmp2->pBone[-1].Q = Quat( ROT_XYZ, 0.0f, 0.0f, 0.0f );
//	////pTmp2->pBone[-1].S = Vect( 1.0f, 1.0f, 1.0f );
//
//	////     Node Name: Bip01
//	//pTmp2->pBone[0].T = Vect(3.080650f, -5.935921f, 0.180111f);
//	//pTmp2->pBone[0].Q = Quat(ROT_XYZ, 1.702839f, -1.378956f, 3.065481f);
//	//pTmp2->pBone[0].S = Vect(0.100000f, 0.100000f, 0.100000f);
//
//	//// Keenan HACK - set to 1.0f
//	//pTmp2->pBone[0].S = Vect(1.00000f, 1.00000f, 1.00000f);
//
//	////     Node Name: Bip01 Pelvis
//	//pTmp2->pBone[1].T = Vect(0.0f, 0.0f, 0.0f);
//	//pTmp2->pBone[1].Q = Quat(ROT_XYZ, 0.0f, 0.0f, 0.0f);
//	//pTmp2->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//	////     Node Name: Bip01 Spine
//	//pTmp2->pBone[2].T = Vect(6.016934f, -0.014115f, -0.000028f);
//	//pTmp2->pBone[2].Q = Quat(ROT_XYZ, -0.068606f, 0.033833f, 0.012427f);
//	//pTmp2->pBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Thigh
//	//pTmp2->pBone[3].T = Vect(-6.475285f, -0.834019f, 13.434450f);
//	//pTmp2->pBone[3].Q = Quat(ROT_XYZ, 3.065515f, 0.107671f, -2.885550f);
//	//pTmp2->pBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Calf
//	//pTmp2->pBone[4].T = Vect(16.961559f, -0.000002f, -0.000004f);
//	//pTmp2->pBone[4].Q = Quat(ROT_XYZ, -0.200336f, -0.020123f, -0.659933f);
//	//pTmp2->pBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Foot
//	//pTmp2->pBone[5].T = Vect(13.263550f, 0.000003f, 0.000002f);
//	//pTmp2->pBone[5].Q = Quat(ROT_XYZ, -0.107715f, -0.156771f, 0.873369f);
//	//pTmp2->pBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Thigh
//	//pTmp2->pBone[6].T = Vect(-5.550339f, 1.039276f, -13.828350f);
//	//pTmp2->pBone[6].Q = Quat(ROT_XYZ, 3.073914f, 0.062401f, 2.204732f);
//	//pTmp2->pBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Calf
//	//pTmp2->pBone[7].T = Vect(16.961571f, 0.000003f, -0.000003f);
//	//pTmp2->pBone[7].Q = Quat(ROT_XYZ, -0.149287f, -0.004462f, -0.839591f);
//	//pTmp2->pBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Foot
//	//pTmp2->pBone[8].T = Vect(13.263530f, -0.000000f, 0.000002f);
//	//pTmp2->pBone[8].Q = Quat(ROT_XYZ, 0.011289f, 0.017234f, -0.026588f);
//	//pTmp2->pBone[8].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Spine1
//	//pTmp2->pBone[9].T = Vect(17.730431f, -0.017629f, 0.000002f);
//	//pTmp2->pBone[9].Q = Quat(ROT_XYZ, -0.136926f, 0.068386f, 0.020930f);
//	//pTmp2->pBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Neck
//	//pTmp2->pBone[10].T = Vect(22.140881f, -0.008821f, 0.000003f);
//	//pTmp2->pBone[10].Q = Quat(ROT_XYZ, 0.200706f, -0.015178f, -0.097583f);
//	//pTmp2->pBone[10].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 L Clavicle
//	//pTmp2->pBone[11].T = Vect(0.082038f, 1.097273f, 5.349479f);
//	//pTmp2->pBone[11].Q = Quat(ROT_XYZ, 1.013787f, -1.059423f, 2.220898f);
//	//pTmp2->pBone[11].S = Vect(0.999999f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L UpperArm
//	//pTmp2->pBone[12].T = Vect(13.067860f, 0.000004f, -0.000004f);
//	//pTmp2->pBone[12].Q = Quat(ROT_XYZ, -0.124427f, 0.695644f, 0.047606f);
//	//pTmp2->pBone[12].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Forearm
//	//pTmp2->pBone[13].T = Vect(15.099380f, -0.002248f, 0.000674f);
//	//pTmp2->pBone[13].Q = Quat(ROT_XYZ, 0.686527f, 0.053035f, -0.696319f);
//	//pTmp2->pBone[13].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Hand
//	//pTmp2->pBone[14].T = Vect(15.099410f, 0.000082f, 0.000072f);
//	//pTmp2->pBone[14].Q = Quat(ROT_XYZ, -1.588506f, 0.197356f, -0.141726f);
//	//pTmp2->pBone[14].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Clavicle
//	//pTmp2->pBone[15].T = Vect(-0.083741f, -1.080058f, -5.352953f);
//	//pTmp2->pBone[15].Q = Quat(ROT_XYZ, -0.363947f, 1.149070f, 2.761519f);
//	//pTmp2->pBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R UpperArm
//	//pTmp2->pBone[16].T = Vect(13.067840f, -0.000001f, 0.000001f);
//	//pTmp2->pBone[16].Q = Quat(ROT_XYZ, -0.077571f, -0.824910f, 0.421713f);
//	//pTmp2->pBone[16].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 R Forearm
//	//pTmp2->pBone[17].T = Vect(15.099410f, -0.000194f, 0.000024f);
//	//pTmp2->pBone[17].Q = Quat(ROT_XYZ, -0.807806f, -0.025578f, -0.135432f);
//	//pTmp2->pBone[17].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Hand
//	//pTmp2->pBone[18].T = Vect(15.099400f, 0.000180f, -0.000104f);
//	//pTmp2->pBone[18].Q = Quat(ROT_XYZ, 1.566043f, -0.064301f, -0.102912f);
//	//pTmp2->pBone[18].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Head
//	//pTmp2->pBone[19].T = Vect(11.079190f, 0.000001f, 0.000001f);
//	//pTmp2->pBone[19].Q = Quat(ROT_XYZ, 0.172725f, -0.056284f, 0.261669f);
//	//pTmp2->pBone[19].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	//// --------  Frame 20  ----------------------------
//
//	//Frame_Bucket *pTmp3 = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	//pTmp3->prevBucket = pTmp2;
//	//pTmp3->nextBucket = 0;
//	//pTmp3->KeyTime = 6 * Time(Time::NTSC_30_FRAME);
//	//pTmp3->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[NUM_BONES];
//	//pTmp2->nextBucket = pTmp3;
//
//	////------- Key Time: 6  ms: 200 ------------------ 
//
//	////     Node Name: RootNode
//	////pTmp3->pBone[-1].T = Vect( 0.0f, 0.0f, 0.0f );
//	////pTmp3->pBone[-1].Q = Quat( ROT_XYZ, 0.0f, 0.0f, 0.0f );
//	////pTmp3->pBone[-1].S = Vect( 1.0f, 1.0f, 1.0f );
//
//	////     Node Name: Bip01
//	//pTmp3->pBone[0].T = Vect(3.451018f, -5.464783f, 0.018024f);
//	//pTmp3->pBone[0].Q = Quat(ROT_XYZ, 2.236743f, -1.456398f, 2.515219f);
//	//pTmp3->pBone[0].S = Vect(0.100000f, 0.100000f, 0.100000f);
//
//	//// Keenan HACK - set to 1.0f
//	//pTmp3->pBone[0].S = Vect(1.00000f, 1.00000f, 1.00000f);
//
//	////     Node Name: Bip01 Pelvis
//	//pTmp3->pBone[1].T = Vect(0.0f, 0.0f, 0.0f);
//	//pTmp3->pBone[1].Q = Quat(ROT_XYZ, 0.0f, 0.0f, 0.0f);
//	//pTmp3->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//	////     Node Name: Bip01 Spine
//	//pTmp3->pBone[2].T = Vect(6.016934f, -0.014115f, -0.000028f);
//	//pTmp3->pBone[2].Q = Quat(ROT_XYZ, -0.073432f, 0.049660f, 0.003894f);
//	//pTmp3->pBone[2].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Thigh
//	//pTmp3->pBone[3].T = Vect(-6.688056f, -0.942405f, 13.322590f);
//	//pTmp3->pBone[3].Q = Quat(ROT_XYZ, 3.008768f, -0.027241f, -2.918211f);
//	//pTmp3->pBone[3].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Calf
//	//pTmp3->pBone[4].T = Vect(16.961559f, -0.000002f, -0.000004f);
//	//pTmp3->pBone[4].Q = Quat(ROT_XYZ, 0.082271f, -0.030508f, -1.020630f);
//	//pTmp3->pBone[4].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L Foot
//	//pTmp3->pBone[5].T = Vect(13.263550f, 0.000003f, 0.000002f);
//	//pTmp3->pBone[5].Q = Quat(ROT_XYZ, -0.072118f, -0.120664f, 0.647919f);
//	//pTmp3->pBone[5].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Thigh
//	//pTmp3->pBone[6].T = Vect(-5.330676f, 1.061123f, -13.912840f);
//	//pTmp3->pBone[6].Q = Quat(ROT_XYZ, 3.059350f, 0.060210f, 2.300501f);
//	//pTmp3->pBone[6].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Calf
//	//pTmp3->pBone[7].T = Vect(16.961571f, 0.000003f, -0.000003f);
//	//pTmp3->pBone[7].Q = Quat(ROT_XYZ, -0.179144f, 0.000921f, -1.038701f);
//	//pTmp3->pBone[7].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Foot
//	//pTmp3->pBone[8].T = Vect(13.263530f, -0.000000f, 0.000002f);
//	//pTmp3->pBone[8].Q = Quat(ROT_XYZ, 0.000910f, -0.153339f, 0.132482f);
//	//pTmp3->pBone[8].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Spine1
//	//pTmp3->pBone[9].T = Vect(17.730431f, -0.017629f, 0.000002f);
//	//pTmp3->pBone[9].Q = Quat(ROT_XYZ, -0.146976f, 0.099419f, 0.002539f);
//	//pTmp3->pBone[9].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Neck
//	//pTmp3->pBone[10].T = Vect(22.140881f, -0.008821f, 0.000003f);
//	//pTmp3->pBone[10].Q = Quat(ROT_XYZ, 0.217788f, -0.063335f, -0.081184f);
//	//pTmp3->pBone[10].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 L Clavicle
//	//pTmp3->pBone[11].T = Vect(0.344969f, 1.186275f, 5.319908f);
//	//pTmp3->pBone[11].Q = Quat(ROT_XYZ, 0.977715f, -1.059558f, 2.245525f);
//	//pTmp3->pBone[11].S = Vect(0.999999f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 L UpperArm
//	//pTmp3->pBone[12].T = Vect(13.067860f, 0.000004f, -0.000004f);
//	//pTmp3->pBone[12].Q = Quat(ROT_XYZ, -0.060241f, 0.801118f, 0.089059f);
//	//pTmp3->pBone[12].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Forearm
//	//pTmp3->pBone[13].T = Vect(15.099380f, -0.000849f, 0.000336f);
//	//pTmp3->pBone[13].Q = Quat(ROT_XYZ, 0.764798f, 0.046213f, -0.568218f);
//	//pTmp3->pBone[13].S = Vect(1.000000f, 1.000001f, 1.000001f);
//
//	////     Node Name: Bip01 L Hand
//	//pTmp3->pBone[14].T = Vect(15.099410f, 0.000082f, 0.000072f);
//	//pTmp3->pBone[14].Q = Quat(ROT_XYZ, -1.584537f, 0.170981f, -0.213083f);
//	//pTmp3->pBone[14].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Clavicle
//	//pTmp3->pBone[15].T = Vect(-0.346380f, -1.169084f, -5.323622f);
//	//pTmp3->pBone[15].Q = Quat(ROT_XYZ, -0.248313f, 1.117032f, 2.847506f);
//	//pTmp3->pBone[15].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R UpperArm
//	//pTmp3->pBone[16].T = Vect(13.067840f, -0.000001f, 0.000001f);
//	//pTmp3->pBone[16].Q = Quat(ROT_XYZ, -0.057036f, -0.807177f, 0.385402f);
//	//pTmp3->pBone[16].S = Vect(1.000000f, 1.000000f, 1.000001f);
//
//	////     Node Name: Bip01 R Forearm
//	//pTmp3->pBone[17].T = Vect(15.099410f, -0.000312f, -0.000006f);
//	//pTmp3->pBone[17].Q = Quat(ROT_XYZ, -0.877783f, -0.023913f, -0.032898f);
//	//pTmp3->pBone[17].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 R Hand
//	//pTmp3->pBone[18].T = Vect(15.099400f, 0.000180f, -0.000104f);
//	//pTmp3->pBone[18].Q = Quat(ROT_XYZ, 1.569602f, -0.100730f, -0.105726f);
//	//pTmp3->pBone[18].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//	////     Node Name: Bip01 Head
//	//pTmp3->pBone[19].T = Vect(11.079190f, 0.000001f, 0.000001f);
//	//pTmp3->pBone[19].Q = Quat(ROT_XYZ, 0.140443f, -0.041052f, 0.303770f);
//	//pTmp3->pBone[19].S = Vect(1.000000f, 1.000000f, 1.000000f);
//
//}


//void walk_anim_node(GameObject* pNode)
//{
//	assert(pNode);
//	// --------- Do pose stuff here -----------------------
//	setBonePose(pNode);
//
//	// --------- Do pose stuff here -----------------------
//	GameObjectAnim*child = 0;
//
//	// iterate through all of the active children 
//	if (pNode->GetChild() != 0)
//	{
//		child = (GameObjectAnim*)pNode->GetChild();
//		// make sure that allocation is not a child node 
//		while (child != 0)
//		{
//			walk_anim_node(child);
//			// goto next sibling
//			child = (GameObjectAnim*)child->GetNextSibling();
//		}
//	}
//	else
//	{
//		// bye bye exit condition
//	}
//}

//void SetAnimationData(Heap* pHeap)
//{
//	// --------  Result Frame  ----------------------
//
//	pHead = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	pHead->prevBucket = 0;
//	pHead->nextBucket = 0;
//	pHead->KeyTime = Time(Time::ZERO);
//	pHead->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[NUM_BONES];
//
//	// --------  Frame 0  ----------------------------
//
//	Frame_Bucket *pTmp = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	pTmp->prevBucket = pHead;
//	pTmp->nextBucket = 0;
//	pTmp->KeyTime = 0 * Time(Time::NTSC_30_FRAME);
//	pTmp->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[NUM_BONES];
//	pHead->nextBucket = pTmp;
//
//	// Bone 0 ---------------------------------------------------------
//
//	pTmp->pBone[0].T = Vect(-113.894875f, 0.000000f, 0.000000f);
//	pTmp->pBone[0].Q = Quat(ROT_XYZ, 0.0f* MATH_PI_180, 0.0f* MATH_PI_180, 0.005445f * MATH_PI_180);
//	pTmp->pBone[0].S = Vect(1.0f, 1.0f, 1.0f);
//
//	// Bone 1 ---------------------------------------------------------
//
//	pTmp->pBone[1].T = Vect(114.826065f, -0.000016f, 0.000000f);
//	pTmp->pBone[1].Q = Quat(ROT_XYZ, 0.000000f * MATH_PI_180, 0.000000f* MATH_PI_180, -0.005444f * MATH_PI_180);
//	pTmp->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//	// --------  Frame 35  ----------------------------
//
//	Frame_Bucket *pTmp2 = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	pTmp2->prevBucket = pTmp;
//	pTmp2->nextBucket = 0;
//	pTmp2->KeyTime = 35 * Time(Time::NTSC_30_FRAME);
//	pTmp2->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[NUM_BONES];
//	pTmp->nextBucket = pTmp2;
//
//	// Bone 0 ---------------------------------------------------------
//
//	pTmp2->pBone[0].T = Vect(-69.141525f, 0.000000f, 0.000000f);
//	pTmp2->pBone[0].Q = Quat(ROT_XYZ, 0.0f* MATH_PI_180, 0.0f* MATH_PI_180, 35.0f * MATH_PI_180);
//	pTmp2->pBone[0].S = Vect(1.0f, 1.0f, 1.0f);
//
//	// Bone 1 ---------------------------------------------------------
//
//	pTmp2->pBone[1].T = Vect(114.826065f, -0.000016f, 0.000000f);
//	pTmp2->pBone[1].Q = Quat(ROT_XYZ, 0.000000f* MATH_PI_180, 0.000000f* MATH_PI_180, -69.954391f * MATH_PI_180);
//	pTmp2->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//	// --------  Frame 70  ----------------------------
//
//	Frame_Bucket *pTmp3 = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Frame_Bucket();
//	pTmp3->prevBucket = pTmp2;
//	pTmp3->nextBucket = 0;
//	pTmp3->KeyTime = 70 * Time(Time::NTSC_30_FRAME);
//	pTmp3->pBone = new(pHeap, Mem::Align::Byte_16, __FILE__, __LINE__) Bone[NUM_BONES];
//	pTmp2->nextBucket = pTmp3;
//
//	// Bone 0 ---------------------------------------------------------
//
//	pTmp3->pBone[0].T = Vect(-39.924347f, 0.000000f, 0.000000f);
//	pTmp3->pBone[0].Q = Quat(ROT_XYZ, 0.0f* MATH_PI_180, 0.0f* MATH_PI_180, 0.000003f * MATH_PI_180);
//	pTmp3->pBone[0].S = Vect(1.0f, 1.0f, 1.0f);
//
//	// Bone 1 ---------------------------------------------------------
//
//	pTmp3->pBone[1].T = Vect(114.826065f, -0.000016f, 0.000000f);
//	pTmp3->pBone[1].Q = Quat(ROT_XYZ, 0.000000f * MATH_PI_180, 0.000000f* MATH_PI_180, 0.0f * MATH_PI_180);
//	pTmp3->pBone[1].S = Vect(1.0f, 1.0f, 1.0f);
//
//}


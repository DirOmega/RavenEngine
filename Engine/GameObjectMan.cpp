#include <assert.h>
#include "NullModel.h"
#include "GraphicsObject_Null.h"
#include "GraphicsObject_Wireframe.h"
#include "GameObject.h"
#include "GameObjectMan.h"
#include "PCSTreeForwardIterator.h"
#include "PCSTreeReverseIterator.h"
#include "ModelMan.h"
#include "BoundingSphere.h"
#include "ShaderManager.h"

#include "Camera.h"
#include "CameraManager.h"
#include "Time.h"

#include "ScratchSpace.h"

//TODO:: remove this extern I can do this when I do the manager Create() when I make this a proper baseManager derived.
//extern Heap* goHeapPtr;

void GameObjectMan::DeleteTree()
{
	GameObjectMan * pMan = GameObjectMan::privGetInstance();
	PCSNode * pRoot = pMan->pRootTree->GetRoot();
	PCSNode * toDel;
	if (pRoot != nullptr)
	{
		PCSTreeReverseIterator pRItr(pRoot);
		while (!pRItr.IsDone())
		{
			toDel = pRItr.CurrentItem();

			pRItr.Next();
			delete toDel;
		}
	}

	delete GameObjectMan::privGetInstance()->pRootTree;
}

void GameObjectMan::Add(GameObject *pObj, GameObject *pParent)
{
	assert(pObj != 0);
	assert(pParent != 0);

	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();


	Sphere* s = new Sphere();

	RitterSphere(*s, pObj->GetGraphicsObject()->GetModel()->modelData, pObj->GetGraphicsObject()->GetModel()->numVerts);

	//GraphicsObject_Wireframe* t = new(ScratchSpace::GetGOHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Wireframe(
	GraphicsObject_Wireframe* t = new GraphicsObject_Wireframe(
		pGOM->bSphereMdl,
		ShaderManager::addShader(ShaderObject::ShaderName::DEFAULT, "colorRender")
	);

	CullingSphere* cs = new CullingSphere(s);

	Vect scaleVect;
	pObj->getScale(scaleVect);
	cs->setScale(scaleVect);

	CollisionObject* colObj = new CollisionObject(t, cs);

	assert(colObj != nullptr);
	pObj->setCollisionObject(colObj);

	assert(pObj->GetCollisionObject() != nullptr);

	delete s;



	// insert object to root
	pGOM->pRootTree->Insert(pObj, pParent);

}

GameObject* GameObjectMan::GetRoot()
{

	// Get singleton
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	GameObject *pGameObj = (GameObject *)pGOM->pRootTree->GetRoot();
	assert(pGameObj);

	return pGameObj;

}

//void GameObjectMan::Add(GameObject *pObj)
//{
//	assert(pObj != 0);
//
//	// Get singleton
//	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
//
//	// Get root node
//	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
//
//	Sphere* s = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) Sphere();
//
//	RitterSphere(*s, pObj->GetGraphicsObject()->GetModel()->modelData, pObj->GetGraphicsObject()->GetModel()->numVerts);
//
//	GraphicsObject_Wireframe* t = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Wireframe(
//		pGOM->bSphereMdl,
//		new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::DEFAULT, "colorRender"),
//		goHeapPtr
//	);
//
//	CullingSphere* cs = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) CullingSphere(goHeapPtr, s);
//
//	Vect scaleVect;
//	pObj->getScale(scaleVect);
//	cs->setScale(scaleVect);
//
//	CollisionObject* colObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) CollisionObject(t,cs);
//	
//	assert(colObj != nullptr);
//		pObj->setCollisionObject(colObj);
//
//	assert(pObj->GetCollisionObject() != nullptr);
//
//	// insert object to root
//	pGOM->pRootTree->Insert(pObj, pRootNode);
//
//	delete s;
//
//}

//void GameObjectMan::Update(float currentTime, const CollisionObject*)
//{
//	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
//	assert(pGOM);
//
//	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
//	assert(pRootNode);
//
//	PCSTreeForwardIterator pForwardIter(pRootNode);
//	PCSNode *pNode = pForwardIter.First();
//
//	GameObject *pGameObj = 0;
//
//	while (!pForwardIter.IsDone())
//	{
//		assert(pNode);
//		// Update the game object
//		pGameObj = (GameObject *)pNode;
//		pGameObj->Process(currentTime);
//
//		//pGameObj->CullTest(pCullVolume);
//
//		pNode = pForwardIter.Next();
//	}
//}


void GameObjectMan::Update(const Time& currentTime)
{

		GameObjectMan *pGOM = GameObjectMan::privGetInstance();
		assert(pGOM);
	
		PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
		assert(pRootNode);
	
		PCSTreeForwardIterator pForwardIter(pRootNode); //skips the dummy node
		PCSNode *pNode = pForwardIter.First();
		pNode = pForwardIter.Next();

		
		GameObject *pGameObj = 0;
	
//		Camera* pCamera = CameraManager::GetActiveCamera();


		int loopCount = 0;
		//Trace::out("Update start\n");
		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject *)pNode;
			pGameObj->Process(currentTime);
			//Trace::out("Updating Item #%i @addr %x ",loopCount, pGameObj );
			//pGameObj->PrintNode();
			//Trace::out("\n");
			//pGameObj->Abort = pCamera->cullTest(pGameObj->GetCollisionObject()->getCullSphere(), pGameObj->GetWorld());
			pGameObj->Abort = false;
			pNode = pForwardIter.Next();
			loopCount++;
		}
		//Trace::out("End of Update\n");
}


//void GameObjectMan::Update(float currentTime)
//{
//	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
//	assert(pGOM);
//
//	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
//	assert(pRootNode);
//
//	PCSTreeForwardIterator pForwardIter(pRootNode);
//	PCSNode *pNode = pForwardIter.First();
//
//	GameObject *pGameObj = 0;
//
//	Camera* pCamera = CameraManager::GetActiveCamera();
//
//	while (!pForwardIter.IsDone())
//	{
//		assert(pNode);
//		// Update the game object
//		pGameObj = (GameObject *)pNode;
//		pGameObj->Process(currentTime);
//		//cullTest(pGameObj, pCamera);
//		pGameObj->Abort = pCamera->cullTest(pGameObj->GetCollisionObject()->getCullSphere(), pGameObj->GetWorld());
//		pNode = pForwardIter.Next();
//	}
//}

void GameObjectMan::Draw()
{
	GameObjectMan *pGOM = GameObjectMan::privGetInstance();
	assert(pGOM);

	PCSNode *pRootNode = pGOM->pRootTree->GetRoot();
	assert(pRootNode);

	PCSTreeForwardIterator pForwardIter(pRootNode);
	PCSNode *pNode = pForwardIter.First();

	GameObject *pGameObj = 0;

	if (pGOM->debugMode)
	{
		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject *)pNode;
			if (!pGameObj->Abort)
			{
				pGameObj->Draw(true);
			}
			else
			{
				if (pNode != pRootNode)
				{
					Trace::out("ModelAborted!\n");
				}
			}
			
			pNode = pForwardIter.Next();
		}
	}
	else
	{
		while (!pForwardIter.IsDone())
		{
			assert(pNode);
			// Update the game object
			pGameObj = (GameObject *)pNode;
			if (!pGameObj->Abort)
			{
				pGameObj->Draw();
			}
			else
			{
				if (pNode != pRootNode)
					Trace::out("ModelAborted!\n");
			}
			pNode = pForwardIter.Next();
		}
	}
}

//TODO:: figure out how to get Heap here
//TODO:: clean memory leaks
GameObjectMan::GameObjectMan() : bSphereMdl(0)
{
	// Create the root node (null object)
	//NullModel *pModel = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) NullModel(0);
	Model *pModel = ModelMan::find(ModelName::NullMod);

	bSphereMdl = ModelMan::find(ModelName::BSphere);
	assert(bSphereMdl);

	//ShaderObject *pShader = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::NULL_SHADER, "nullRender");
	ShaderObject* pShader = ShaderManager::find(ShaderObject::ShaderName::NULL_SHADER);
	GraphicsObject *pGraphicsObject = new GraphicsObject_Null(pModel, pShader);

	GameObject *pGameRoot = new GameObject(pGraphicsObject);
	pGameRoot->SetName("GameObject_Root");

	Sphere* s = new Sphere();

	RitterSphere(*s, pGameRoot->GetGraphicsObject()->GetModel()->modelData, pGameRoot->GetGraphicsObject()->GetModel()->numVerts);

	ShaderObject* shdr = ShaderManager::addShader(ShaderObject::ShaderName::DEFAULT, "colorRender");

	GraphicsObject_Wireframe* t = new GraphicsObject_Wireframe(
		bSphereMdl,
		shdr
	);


	
	CullingSphere* cs     = new CullingSphere(s);

	CollisionObject* colObj = new CollisionObject(t, cs);

	assert(colObj != nullptr);
	pGameRoot->setCollisionObject(colObj);

	// Create the tree
	this->pRootTree = new PCSTree();
	assert(this->pRootTree);

	// Attach the root node
	this->pRootTree->Insert(pGameRoot, this->pRootTree->GetRoot());

	delete s;
}

GameObjectMan * GameObjectMan::privGetInstance(void)
{
	// This is where its actually stored (BSS section)
	static GameObjectMan gom;
	return &gom;
}

void GameObjectMan::ToggleDebug()
{
	privGetInstance()->debugMode = !privGetInstance()->debugMode;
}

PCSTree* GameObjectMan::GetPCSTree()
{
	return privGetInstance()->pRootTree;
}
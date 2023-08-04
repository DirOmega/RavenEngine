#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "GameObjectAnim.h"

#include "PCSTree.h"
#include "CollisionObject.h"

// Singleton
class GameObjectMan //: public baseManager  
{

public:

	//static void Add(GameObject *pObj);
	static void Add(GameObject *pObj, GameObject* pParent);
	static void Draw(void);
	static void Update(const Time& currentTime);
	//static void Update(float currentTime);
	//static void Update(float currentTime, const CollisionObject* pCullVolume);
	static void DeleteTree();
	static void ToggleDebug();

	static GameObject* GetRoot();
	static PCSTree* GetPCSTree();

private:

	GameObjectMan();
	static GameObjectMan *privGetInstance();
	Model *bSphereMdl;
	PCSTree *pRootTree;
	bool debugMode;
	bool pad1;
	bool pad2;
	bool pad3;

};

#endif
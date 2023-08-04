//TODO:: clean up includes, once I get the managers setup I can remove the includes that are managed by that
#include "Game.h"

#include "MathEngine.h"
#include "TGALoader.h"
#include "ModelMan.h"
#include "ModelFromFile.h"

#include "ShaderObject.h"
#include "CubeModel.h"
#include "CubeModelDebugTex.h"
#include "TextureMan.h"
#include "ShaderManager.h"
#include "ToonShader.h"

//#include "GraphicsObject.h"
#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_CycleTextureLight.h"
#include "TexLink.h"
#include "InputManager.h"

#include "Camera.h"
#include "CameraManager.h"
#include "GameObjectMan.h"

#include "BoundingSphere.h"

#include "Time.h"
#include "Timer.h"
#include "PyramidModel.h"
#include "Anim.h"

#include "AnimController.h"
#include "CamAnimController.h"
#include "ScratchSpace.h"

//TODO:: move into model manager
#include "SkinnedModelFromFile.h"
//#include "AudioInclude.h"//Needs Lots of work

//#define new(inSize, heapPtr, alignment) (inSize, heapPtr, alignment, __FILE__ , __LINE__)

//TODO:: get rid of all gobals
//ShaderObject *pShaderObject_color = 0;
//ShaderObject *pShaderObject_texture = 0;

//GameObject *pGameObj = 0; //n 4
//GameObject *pGameObj2 = 0;
//GameObject *pGameObj3 = 0;
//GameObject *pGameObj4 = 0;
//GameObject *pGameObj5 = 0;
//GameObject *pGameObj6 = 0;
//GameObject *pGameObj7 = 0;

//Camera	   *pCam = 0;
//TODO:: eleminate externs and globals
//TODO:: convert all new calls to my mem system
//TODO:: give all managers thier own heaps to use
//TODO:: make a scratchSpace manager that has a heap for all methods to put new's into a default heap for temp storage.
//TODO:: once all news are converted to my mem system I'll be able to find any memory leaks.
//TODO:: move a lot of the functionality out of the game class and into the engine class 
//including the Mem::init() so I can init it first so it can be used by everything beyond the first new call.

Heap* StageHeapPtr;

//AnimController* pTeddyController;
CompositeAnimController* pTeddyController2;
//AnimController* pTeddyController3;

//AnimController* pHIGAD1;
//AnimController* pHIGAD2;
//AnimController* pHIGAD3;
//AnimController* pHIGAD4;

CompositeAnimController2 * pSIOL;
CompositeAnimController2 * pSIOR;
CompositeAnimController2* pFRIEND;
CamAnimController* pCamTrack;

float tDelta = 0;
//extern GameObjectAnim *pFirstBone;

//right now everything is 16Byte aligned so I could probably include the align type in the macro
//#define addModel(_assetName, _name, _pHeap, Align) addModel(_assetName, _name, _pHeap, Align,__FILE__,__LINE__ )
//#define addModel(_PackageName, _nameInPak, _nameE, _pHeap, Align) addModel(_PackageName, _nameInPak, _nameE, _pHeap, Align, __FILE__,__LINE__)
#define UNUSED_VAR(x) (x)
//IDEA:: create a camera manager singleton that keeps track of active cameras and the render can get the acitve camera to draw to it during the render call.

//TODO:: needs shader manager, model manager, texture manager to flyweight textures and models and shaders to game objects.

//TODO:: remove any global news going fully into my own memory system

//TODO:: random thought: double check the speed on the math library I think I should pull the vertical matricies into a mm128 and then use that for caluclates down the line, I think I'm hurting the compilers ability to optimze.

Game::Game(const char* pWindowName, const int Width, const int Height)
	: Engine(pWindowName, Width, Height)
{
	assert(pWindowName);
	//this->globalTimer.Tic();
	//this->intervalTimer.Tic();
}

void Game::Initialize()
{
	Engine::Initialize();
	//todo add debug texture
	//this->pProjMatrix = new(matHeapPtr, Mem::Align::Byte_16, __FILE__ , __LINE__) Matrix(IDENTITY);
	//this->onResize(info.windowWidth, info.windowHeight);
}

//#define Human
#define SIO 0

//TODO:: DEMO:: setup 4 cameras, one that showcases a different section then move 
void Game::LoadContent()
{
	//swap everything into managers and push the storage to my memory management sytem passing in __FILE__ and __LINE__ as arguments to the manager create call
#ifdef Human
	 //Camera setup
	//Note:: this camera points at where the human and teddy like to pop up
	Camera* pCam = new Camera();

	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(35.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 1.00f, 10000.0f);

	//pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 150.0f), Vect(0.0f, -400.0f, 150.0f));
	pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 150.0f), Vect(0.0f, -700.0f, 150.0f));
	pCam->updateCamera();


#elif SIO

	Camera* pCam = new Camera();

	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(35.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 1.00f, 10000.0f);

	//pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 150.0f), Vect(0.0f, -400.0f, 150.0f));
	pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 15.0f), Vect(0.0f, -50.0f, 15.0f));
	pCam->updateCamera();

	pCamTrack = new CamAnimController("./SIOL/camera.rcam");


#else // SIO

	Camera* pCam = new Camera();

	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(35.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 1.00f, 10000.0f);

	//pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 150.0f), Vect(0.0f, -400.0f, 150.0f));
	pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, -50.0f, 15.0f));
	pCam->updateCamera();

	pCamTrack = new CamAnimController("./FRIEND/camera.rcam");

#endif // Human

	int n = 0;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);

	Trace::out("Max Vertex Attribs = %i\n", n);

	//TODO:: setup a camera handle system
	CameraManager::addCamera(Camera::CameraName::DEFAULT, pCam, true);

	Model* bSphere = ModelMan::addModel("BSphere.mdl", ModelName::BSphere);

	//Model* StageRound = ModelMan::addModel("001_Round.mdl", ModelName::Round);
	//Model* StageLine = ModelMan::addModel("002_Line.mdl", ModelName::Line);
	//Model* StageFloor = ModelMan::addModel("003_Floor.mdl", ModelName::Floor);

	//Model* humanoid = ModelMan::addModel("Humanoid.smdl", ModelName::Astroboy, ScratchSpace::GetModManHeap(), Mem::Align::Byte_16, __FILE__, __LINE__);

//	Model* pHumanoid = ModelMan::addModelSkinned("Humanoid.smdl", ModelName::Astroboy);
//	Model* pTeddy =    ModelMan::addModelSkinned("teddy.smdl", ModelName::WarBear);

	//Model* pTreadMill = ModelMan::addModel("treadmill.spu", "Treadmill", ModelName::Treadmill, ScratchSpace::GetModManHeap(), Mem::Align::Byte_16, __FILE__, __LINE__);
	//Model* pMeiko_Mesh = ModelMan::addModel("MEIKO_mesh.mdl", ModelName::Astroboy);

	UNUSED_VAR(bSphere);
	//UNUSED_VAR(pBuggyTest);

	// Create/Load Shaders and program
	//TODO:: I've gotta get theese managed because I'm 99% sure theese are leaking because I have no way of knowing if they are safe to delete
	/*ShaderObject* pShaderObject_color = new(camHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::DEFAULT, "colorRender");
	assert(pShaderObject_color);*/

	ShaderObject* pShaderObject_texture = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlat, "textureRender");
	//assert(pShaderObject_texture);

//	ShaderObject *pShaderObject_textureLight = new(camHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
	ShaderObject *pShaderObject_textureLight = ShaderManager::addShader(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
	UNUSED_VAR(pShaderObject_textureLight);

	ShaderObject *pSkinShader = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlatSkin, "skinShader");
	ShaderObject *pSkinShaderWBT = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlatSkinWBT, "skinShaderWBT");
	ShaderObject *pSkinLightShader = ShaderManager::addShader(ShaderObject::ShaderName::TextureSkinLight, "skinLightShader");
	ShaderObject *pSkinLightShader2 = ShaderManager::addShader(ShaderObject::ShaderName::TextureSkinLight2, "skinLightShader");
	ShaderObject *pSkinShaderShapeDefo = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlatSkinShapeDefo, "skinShapeDefo");
	ShaderObject *pSkinShaderShapeDefoLit = ShaderManager::addShader(ShaderObject::ShaderName::ShapeDefoLight, "shapeDefoLight");
	ShaderObject *pSkinShadershapeDefoLit2 = ShaderManager::addShader(ShaderObject::ShaderName::ShapeDefoLight2, "shapeDefoLight");
	//	ShaderObject *pShapeDefoWBT = ShaderManager::addShader(ShaderObject::ShaderName::SkinShapeDefoWBT, "skinShapeDefoWBT");

	assert(pShaderObject_textureLight);

	UNUSED_VAR(pSkinLightShader);

	UNUSED_VAR(pSkinShaderWBT);
	UNUSED_VAR(pSkinShader);
	UNUSED_VAR(pSkinShaderShapeDefo);
	UNUSED_VAR(pSkinShaderShapeDefoLit);
	//I couldn't get the toon shader to compile properly 
	//ShaderObject *pShaderToon = new(camHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) ShaderObject(ShaderObject::ShaderName::Toon, "colorRender");
	//assert(pShaderToon);

	//ShaderManager::addShader(pShaderObject_color);
	//ShaderManager::addShader(pShaderObject_texture);
	//ShaderManager::addShader(pShaderObject_textureLight);
	//ShaderManager::addShader(pShaderToon);

	//Textures
	//TextureMan::addTexture("Rocks.tga", TextureName::ROCKS);
	//TextureMan::addTexture("Stone.tga", TextureName::STONES);
	//TextureMan::addTexture("RedBrick.tga", TextureName::RED_BRICK);w
	//TextureMan::addTexture("Duckweed.tga", TextureName::DUCKWEED);
	//TextureMan::addTexture("CubeTest.tga", TextureName::CUBETEST);
	//TextureMan::addTexture("space_frigate.tga", TextureName::FRIG);
	//TextureMan::addTexture("astroboy.tga", TextureName::FRIG);
	//TextureMan::addTexture("buggy_diff.tga", TextureName::FRIG);

	TextureMan::addTexture("Duckweed.tga", TextureName::DUCKWEED);
	TextureMan::addTexture("GrayTex.tga", TextureName::Gray);

	//TextureMan::addTexture("astroboy.spu", "astroboy", TextureName::Astroboy);

	/*TextureMan::addTexture("buggy.spu", "buggy_diff", TextureName::BuggyDiff);
	TextureMan::addTexture("buggy.spu", "buggy_nrm", TextureName::BuggyNrm);
	TextureMan::addTexture("buggy.spu", "buggy_spec", TextureName::BuggySpec);

	TextureMan::addTexture("bear.spu", "warbear", TextureName::Warbear);
	TextureMan::addTexture("bear.spu", "polarbear", TextureName::PolarBear);

	TextureMan::addTexture("geth.spu", "Geth_Trooper", TextureName::GethT);

	TextureMan::addTexture("frig.spu", "space_frigate", TextureName::FRIG);*/

	/*GraphicsObject_ColorByPosition *pGraphics_ColorByPosition = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_ColorByPosition(pCubeModel, pShaderObject_color,matHeapPtr,this);
	assert(pGraphics_ColorByPosition);*/

	/*GraphicsObject_Wireframe *pGraphics_Wireframe = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Wireframe(pCubeModel, pShaderObject_color,matHeapPtr,this);
	assert(pGraphics_Wireframe);
*/
/*GraphicsObject_FlatTexture *pGraphics_FlatTexture = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_FlatTexture(pCubeModel, pShaderObject_texture, TextureName::ROCKS,matHeapPtr,this);
assert(pGraphics_FlatTexture);*/

// Create GameObject
//GameObject *pGameObj;
//GameObject *p0;
//GameObject *p1;

//GraphicsObject_ColorByPosition *pGraphics_ColorByPosition;

//GraphicsObject_Wireframe *pGraphics_Wireframe;

//GraphicsObject_FlatTexture *pGraphics_FlatTexture;

//GraphicsObject_TextureLight *pGraphics_TextureLight;

//GraphicsObject_CycleTextureLight* pGraphics_CycleTextureLight;

////red bone-0
//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, matHeapPtr);
//p0 = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
//
////green -bone1
//color.set(1.5f, 1.5f, 1.5f, 1.5f);
//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pPyramidModel, pShaderObject_textureLight, TextureName::DUCKWEED, color, pos, matHeapPtr);
//p1 = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);

//p0->SetIndex(0);
//p1->SetIndex(1);

//p0->pBoneParent = 0;
//p1->pBoneParent = p0;

//GameObjectMan::Add(p1);
//GameObjectMan::Add(p0, GameObjectMan::GetRoot());

//GameObjectMan::Add(p1, p0);

//SetAnimationHierarchy(reservedHeapPtr);

//ProcessAnimation(Time(Time::ZERO));

//pTeddyController =
//	new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__)
//	AnimController();

//pTeddyController->setSkellyFromFile("teddy.skl");

//pTeddyController->addClipFromFile("walk0.ani", pTeddyController->pAnim2Heap);

//pTeddyController->addClipFromFile("idle0.ani", pTeddyController->pAnim3Heap);

//pTeddyController->Skeleton.pHeriarchyRoot->SetPos(Vect(30.0f, 100.0f, 0.0f));

//GameObjectRigid *pGameObj;
//GraphicsObject  *pGraphics_TextureLight;

//Vect color(1.50f, 1.50f, 1.50f, 1.5f);
//Vect pos(1.0f, 1.0f, 1.0f);

////Humanoid Skin
//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pHumanoid, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
//pGameObj->SetPos(Vect(141.0f, -25.0f, 55.0f));
////pGameObj->SetScale(Vect(.15f, .15f, .15f));

//Matrix RotX = Matrix(ROT_X, MATH_PI2);

//*pGameObj->pQRot = Quat(RotX);

//GameObjectMan::Add(pGameObj,GameObjectMan::GetRoot());

//pTeddyController =
//	new
//	AnimController();

//pTeddyController->setSkellyFromFile("teddy.skl");

//UNUSED_VAR(pHumanoid);
//UNUSED_VAR(pSkinShader);

//pTeddyController->setMesh(pTeddy, pSkinLightShader, "teddy.pose");

//pTeddyController2->setBindPoseFromFile("Humanoid.pose");

//pTeddyController->addClipFromFile("idle0.ani");

//pTeddyController->addClipFromFile("walk0.ani",pTeddyController->pAnim2Heap);

//pTeddyController->addClipFromFile("shot0.ani", pTeddyController->pAnim3Heap);

//*pGameObj->pQRot = Quat(RotX);

//pTeddyController->Skeleton.pHeriarchyRoot->SetPos(Vect(200.0f, 200.0f, 200.0f));

//Matrix RotX = Matrix(ROT_Z, MATH_PI2);

//*((GameObjectRigid*)pTeddyController->Skeleton.pHeriarchyRoot)->pQRot = RotX;

//pTeddyController->Skeleton.pHeriarchyRoot->SetScale(Vect(.15f, .15f, .15f));
//
//LBlouse,
//LBody,
//LEyeHi0,
//LEyeHi1,
//LFace0,
//LFace1,
//LHair0,
//LHair1,
//LLightAcc0,
//LLightAcc1,
//LOther,
//LSkin,
//LArms0,
//LArms1

#ifdef Human

	pTeddyController2 =
		new
		CompositeAnimController("humanoid.skl", "Humanoid.pose");

	//pTeddyController2->setSkellyFromFile("humanoid.skl");


	//UNUSED_VAR(pHumanoid);
	////UNUSED_VAR(pSkinShader);
	Model* pHumanoid = new SkinnedModelFromFile("Humanoid.smdl");
	pTeddyController2->addMesh(pHumanoid, pSkinLightShader);

	////pTeddyController2->setBindPoseFromFile("Humanoid.pose");

	pTeddyController2->addClipFromFile("run0.ani");

	pTeddyController2->addClipFromFile("punch0.ani");

	pTeddyController2->addClipFromFile("shot0.ani");

	pTeddyController2->Skeleton.pHeriarchyRoot->SetPos(Vect(141.0f, -25.0f, 55.0f));

	pTeddyController2->Skeleton.pHeriarchyRoot->SetScale(Vect(.15f, .15f, .15f));


#elif SIO

	//SIOL
	Model* LBlouse = ModelMan::addModelSkinned("./SIOL/Blousef.smdl", ModelName::LBlouse);
	Model* LBody = ModelMan::addModelSkinned("./SIOL/Bodyf.smdl", ModelName::LBody);
	Model* LEyeHi0 = ModelMan::addModelSkinned("./SIOL/eyeHi0f.smdl", ModelName::LEyeHi0);
	Model* LEyeHi1 = ModelMan::addModelSkinned("./SIOL/eyeHi1f.smdl", ModelName::LEyeHi1);
	Model* LFace0 = ModelMan::addModelSkinned("./SIOL/face0f.smdl", ModelName::LFace0);
	Model* LFace1 = ModelMan::addModelSkinned("./SIOL/face1f.smdl", ModelName::LFace1);
	Model* LHair0 = ModelMan::addModelSkinned("./SIOL/hair0f.smdl", ModelName::LHair0);
	Model* LHair1 = ModelMan::addModelSkinned("./SIOL/hair1f.smdl", ModelName::LHair1);
	Model* LAcc0 = ModelMan::addModelSkinned("./SIOL/Light0f.smdl", ModelName::LLightAcc0);
	Model* LAcc1 =   ModelMan::addModelSkinned("./SIOL/Light1f.smdl", ModelName::LLightAcc1);
	Model* LOther = ModelMan::addModelSkinned("./SIOL/Otherf.smdl", ModelName::LOther);
	Model* LSkin = ModelMan::addModelSkinned("./SIOL/Legsf.smdl", ModelName::LSkin);
	Model* LArms0 =  ModelMan::addModelSkinned("./SIOL/SkirtArmf.smdl", ModelName::LArms0);
	Model* LArms1 = ModelMan::addModelSkinned("./SIOL/SkirtArmAccf.smdl", ModelName::LArms1);

	TextureMan::addTexture("./SIOL/k_huku2.tga", TextureName::LBlouse);
	TextureMan::addTexture("./SIOL/k_Asi2.tga", TextureName::LBody);
	TextureMan::addTexture("./SIOL/k_face_MikuApE.tga", TextureName::LEyeHi0);
	TextureMan::addPNGTexture("./SIOL/k_face_MikuApE.rvni", TextureName::LEyeHi1);
	TextureMan::addTexture("./SIOL/k_face_MikuApE.tga", TextureName::LFace0);
	TextureMan::addTexture("./SIOL/k_face_MikuApE.tga", TextureName::LFace1);
	TextureMan::addTexture("./SIOL/hair_MikuAp3.tga", TextureName::LHair0);
	TextureMan::addTexture("./SIOL/hair_MikuAp3.tga", TextureName::LHair1);
	TextureMan::addTexture("./SIOL/k_hika.tga", TextureName::LLightAcc0);
	TextureMan::addPNGTexture("./SIOL/k_hika0.rvni", TextureName::LLightAcc1);
	TextureMan::addPNGTexture("./SIOL/k_hada.rvni", TextureName::LOther);
	TextureMan::addTexture("./SIOL/k_hada.tga", TextureName::LSkin);
	TextureMan::addTexture("./SIOL/k_huku1.tga", TextureName::LArms0);
	TextureMan::addTexture("./SIOL/k_huku1.tga", TextureName::LArms1);

	//	pSIOL = new CompositeAnimController("./SIOL/SIOLv3.skl", "./SIOL/SIOLv3.pose");

	//	pSIOL->addClipFromFile("./SIOL/SIOL.ani");

	pSIOL = new CompositeAnimController2("./SIOL/SIOLv7.skl", "./SIOL/SIOL.pose",0);

	//pSIOL->addClipFromFile("./SIOL/SIOL.ani");
	pSIOL->addClipFromFile("./SIOL/AS0.ani");

	pSIOL->addMesh(LOther, pSkinLightShader, TextureName::LOther, false, true);

	pSIOL->addMesh(LEyeHi1, pSkinShader, TextureName::LEyeHi1, false);
	pSIOL->addMesh(LEyeHi0, pSkinShader, TextureName::LEyeHi1, false);

	pSIOL->addMesh(LBlouse, pSkinLightShader, TextureName::LBlouse, false, true);
	pSIOL->addMesh(LBody, pSkinLightShader, TextureName::LSkin, false, true);

	pSIOL->addMeshWithShapes(LFace0, pSkinShaderShapeDefoLit, TextureName::LFace0, "./SIOL/face0.shape", true);
	pSIOL->addMeshWithShapes(LFace1, pSkinShaderShapeDefoLit, TextureName::LFace1, "./SIOL/face1.shape", true);
	pSIOL->addMesh(LHair0, pSkinLightShader, TextureName::LHair0, false, true);
	pSIOL->addMesh(LHair1, pSkinLightShader, TextureName::LHair1, false, true);
	pSIOL->addMesh(LAcc0, pSkinLightShader, TextureName::LLightAcc1, false, true);
	pSIOL->addMesh(LAcc1, pSkinLightShader, TextureName::LLightAcc1, true, true);

	pSIOL->addMesh(LSkin, pSkinLightShader, TextureName::LBody, false, true);
	pSIOL->addMesh(LArms0, pSkinLightShader, TextureName::LArms0, true, true);
	pSIOL->addMesh(LArms1, pSkinLightShader, TextureName::LArms1, true, true);


	//SIOR
	//skipping Glasses
	Model* RBlouse =     ModelMan::addModelSkinned("./SIOR/Blousef.smdl", ModelName::RBlouse);
	Model* REyeHi0 =     ModelMan::addModelSkinned("./SIOR/eyeHi0f.smdl", ModelName::REyeHi0);
	Model* REyeHi1 =     ModelMan::addModelSkinned("./SIOR/eyeHi1f.smdl", ModelName::REyeHi1);
	Model* RFace0 =       ModelMan::addModelSkinned("./SIOR/face0f.smdl", ModelName::RFace0);
	Model* RFace1 =       ModelMan::addModelSkinned("./SIOR/face1f.smdl", ModelName::RFace1);
	Model* RHair0 =       ModelMan::addModelSkinned("./SIOR/hair0f.smdl", ModelName::RHair0);
	Model* RHair1 =       ModelMan::addModelSkinned("./SIOR/hair1f.smdl", ModelName::RHair1);
	Model* RAcc0 =    ModelMan::addModelSkinned("./SIOR/LightAcc0f.smdl", ModelName::RLightAcc0);
	Model* RAcc1 =    ModelMan::addModelSkinned("./SIOR/LightAcc1f.smdl", ModelName::RLightAcc1);
	Model* ROther =       ModelMan::addModelSkinned("./SIOR/Otherf.smdl", ModelName::ROther);
	Model* RSkin =         ModelMan::addModelSkinned("./SIOR/Skinf.smdl", ModelName::RSkin);
	Model* RArms0 =    ModelMan::addModelSkinned("./SIOR/SkirtArmf.smdl", ModelName::RArms0);
	Model* RArms1 = ModelMan::addModelSkinned("./SIOR/SkirtArmAccf.smdl", ModelName::RArms1);
	Model* RLegs =         ModelMan::addModelSkinned("./SIOR/Legsf.smdl", ModelName::RLegs);

	TextureMan::addPNGTexture("./SIOR/k_huku2.rvni", TextureName::RBlouse);
	TextureMan::addPNGTexture("./SIOR/k_face_MikuAp2.rvni", TextureName::REyeHi0);
	TextureMan::addPNGTexture("./SIOR/k_face_MikuAp2.rvni", TextureName::REyeHi1);
	TextureMan::addTexture("./SIOR/k_face_MikuAp.tga", TextureName::RFace0);
	TextureMan::addTexture("./SIOR/k_face_MikuAp.tga", TextureName::RFace1);
	TextureMan::addPNGTexture("./SIOR/k_hika.rvni", TextureName::RLightAcc0);
	TextureMan::addPNGTexture("./SIOR/k_huku1.rvni", TextureName::RLightAcc1);
	TextureMan::addPNGTexture("./SIOR/k_huku1.rvni", TextureName::RArms0);
	TextureMan::addPNGTexture("./SIOR/k_huku1.rvni", TextureName::RArms1);
	TextureMan::addPNGTexture("./SIOR/ApMiku_hair.rvni", TextureName::RHair0);
	TextureMan::addPNGTexture("./SIOR/ApMiku_hair.rvni", TextureName::RHair1);
	TextureMan::addPNGTexture("./SIOR/k_Hada.rvni", TextureName::ROther);
	TextureMan::addPNGTexture("./SIOR/k_Hada.rvni", TextureName::RSkin);
	TextureMan::addPNGTexture("./SIOR/k_asi2.rvni", TextureName::RLegs);

	pSIOR = new CompositeAnimController2("./SIOR/SIOR.skl", "./SIOR/SIOR.pose",1);
	//
	////pSIOL->addClipFromFile("./SIOL/SIOL.ani");
	pSIOR->addClipFromFile("./SIOR/SIOR.ani");

	//pSIOL->addMesh(LOther, pSkinLightShader, TextureName::LOther, false, true);
	//
	//pSIOL->addMesh(LEyeHi1, pSkinShader, TextureName::LEyeHi1, false);
	//pSIOL->addMesh(LEyeHi0, pSkinShader, TextureName::LEyeHi1, false);

	pSIOR->addMesh(ROther, pSkinLightShader2, TextureName::ROther, false, true);
	pSIOR->addMesh(REyeHi0, pSkinLightShader2, TextureName::REyeHi0, false);
	pSIOR->addMesh(REyeHi1, pSkinLightShader2, TextureName::REyeHi1, false);
	pSIOR->addMesh(RAcc0, pSkinLightShader2, TextureName::RLightAcc0, false, false);


	pSIOR->addMesh(RBlouse, pSkinLightShader2, TextureName::RBlouse, false, true);

	pSIOR->addMesh(RAcc1, pSkinLightShader2,   TextureName::RLightAcc0, true, true);
	pSIOR->addMesh(RArms0, pSkinLightShader2,  TextureName::RArms0, true, true);
	pSIOR->addMesh(RArms1, pSkinLightShader2,  TextureName::RArms1, false, true);
	pSIOR->addMesh(RHair1, pSkinLightShader2, TextureName::RHair1, false, true);
	pSIOR->addMesh(RHair0, pSkinLightShader2,  TextureName::RHair0, false, true);
	pSIOR->addMeshWithShapes(RFace0, pSkinShadershapeDefoLit2, TextureName::RFace0, "./SIOR/face0.shape", true);
	pSIOR->addMeshWithShapes(RFace1, pSkinShadershapeDefoLit2, TextureName::RFace1, "./SIOR/face1.shape", true);


	pSIOR->addMesh(RSkin, pSkinLightShader2,   TextureName::RSkin, false, true);
	pSIOR->addMesh(RLegs, pSkinLightShader2, TextureName::RLegs, false, true);

	//Stage Setup
	GraphicsObject_TextureLight *pGraphics_TextureLight;
	GameObject* pGameObj;

	Model* pFloor = ModelMan::addModel("./SIOStage/floor.mdl", ModelName::Floor);
	Model* pRoof = ModelMan::addModel("./SIOStage/Ceiling.mdl", ModelName::Roof);
	//Model* pClockB = ModelMan::addModel("./SIOStage/ClockBack.mdl", ModelName::ClockBack);
	Model* pClockB0 = ModelMan::addModel("./SIOStage/ClockBack0.mdl", ModelName::ClockBack0);
	Model* pClockB2 = ModelMan::addModel("./SIOStage/ClockBack2.mdl", ModelName::ClockBack2);
//	Model* pGlass = ModelMan::addModel("./SIOStage/Glass.mdl", ModelName::Glass);
	Model* pVaseBase = ModelMan::addModel("./SIOStage/VaseBase.mdl", ModelName::VaseBase);
	Model* pVase = ModelMan::addModel("./SIOStage/Vase.mdl", ModelName::Vase);

	Model* pClockBW = ModelMan::addModel("./SIOStage/ClockBackWhite.mdl", ModelName::ClockBW);
	Model* pClockFrame = ModelMan::addModel("./SIOStage/ClockFrame.mdl", ModelName::ClockFrame);
	Model* pClockMark = ModelMan::addModel("./SIOStage/ClockMark.mdl", ModelName::ClockMark);

	//need to add a bunch more here for the set details

	//TODO:: add glass
	//Model* glass ...

	Model* pG0 = ModelMan::addModel("./SIOStage/Glow0.mdl", ModelName::Glow0);
	Model* pG2 = ModelMan::addModel("./SIOStage/Glow2.mdl", ModelName::Glow2);
	Model* pLF = ModelMan::addModel("./SIOStage/LampFrame.mdl", ModelName::LampFrame);
	Model* pLG = ModelMan::addModel("./SIOStage/LampG.mdl", ModelName::LampG);
	Model* pLS = ModelMan::addModel("./SIOStage/LampStand.mdl", ModelName::LampStand);
	Model* pLFix = ModelMan::addModel("./SIOStage/LFix.mdl", ModelName::LFix);
	Model* pLFG = ModelMan::addModel("./SIOStage/LFixG.mdl", ModelName::LFixG);
	Model* pLF2G = ModelMan::addModel("./SIOStage/LightFix2Glow.mdl", ModelName::LFix2Glow);
	Model* pMA = ModelMan::addModel("./SIOStage/MetalArt.mdl", ModelName::MetalArt);
	Model* pP0 = ModelMan::addModel("./SIOStage/Painting0.mdl", ModelName::P0);
	Model* pP1 = ModelMan::addModel("./SIOStage/Painting1.mdl", ModelName::P1);
	Model* pP2 = ModelMan::addModel("./SIOStage/Painting2.mdl", ModelName::P2);
	Model* pP3 = ModelMan::addModel("./SIOStage/Painting3.mdl", ModelName::P3);
	Model* pP4 = ModelMan::addModel("./SIOStage/Painting4.mdl", ModelName::P4);
	Model* pPlAcc = ModelMan::addModel("./SIOStage/PlasterAcc.mdl", ModelName::PlasterAcc);
	Model* pSconce = ModelMan::addModel("./SIOStage/Sconces.mdl", ModelName::Sconces);
	Model* pShelf = ModelMan::addModel("./SIOStage/Shelf.mdl", ModelName::Shelf);
	Model* pWall0 = ModelMan::addModel("./SIOStage/Wall0.mdl", ModelName::Wall0);
	Model* pWall1 = ModelMan::addModel("./SIOStage/Wall1.mdl", ModelName::Wall1);
	Model* pWall2 = ModelMan::addModel("./SIOStage/Wall2.mdl", ModelName::Wall2);
	Model* pWall3 = ModelMan::addModel("./SIOStage/Wall3.mdl", ModelName::Wall3);
	Model* pWallSconceG = ModelMan::addModel("./SIOStage/WallSconceG.mdl", ModelName::WallSconceG);
	Model* pWinFrame = ModelMan::addModel("./SIOStage/WindowFrame.mdl", ModelName::WindowFrame);

	TextureMan::addPNGTexture("./SIOStage/_ClockBack0b.rvni", TextureName::ClockB0);
	TextureMan::addPNGTexture("./SIOStage/_ClockBack2b.rvni", TextureName::ClockB2);
	TextureMan::addPNGTexture("./SIOStage/_ClockBackWhiteb.rvni", TextureName::ClockBW);
	TextureMan::addPNGTexture("./SIOStage/_Floorb.rvni", TextureName::Floor);
	TextureMan::addPNGTexture("./SIOStage/_Painting0b.rvni", TextureName::P0);
	TextureMan::addPNGTexture("./SIOStage/_Painting1b.rvni", TextureName::P1);
	TextureMan::addPNGTexture("./SIOStage/_Painting2b.rvni", TextureName::P2);
	TextureMan::addPNGTexture("./SIOStage/_Painting3b.rvni", TextureName::P3);
	TextureMan::addPNGTexture("./SIOStage/_Painting4b.rvni", TextureName::P4);
	TextureMan::addPNGTexture("./SIOStage/_Roofb.rvni", TextureName::Roof);
	TextureMan::addPNGTexture("./SIOStage/_Wall0b.rvni", TextureName::Wall0);
	TextureMan::addPNGTexture("./SIOStage/_Wall1b.rvni", TextureName::Wall1);
	TextureMan::addPNGTexture("./SIOStage/_Wall2b.rvni", TextureName::Wall2);
	TextureMan::addPNGTexture("./SIOStage/_Wall3b.rvni", TextureName::Wall3);
	TextureMan::addPNGTexture("./SIOStage/_WallAcc.rvni", TextureName::WallAcc);
	TextureMan::addPNGTexture("./SIOStage/_Shelfb.rvni", TextureName::Black);
	TextureMan::addPNGTexture("./SIOStage/_Vaseb.rvni", TextureName::Vase);

	Vect color(1.50f, 1.50f, 1.50f, 1.5f);
	Vect pos(1.0f, 100.0f, 1.0f);

	Vect Scale(1.3f, 1.3f, 1.3f);

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pG0, pShaderObject_textureLight, TextureName::Gray, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pG2, pShaderObject_textureLight, TextureName::Gray, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLF, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLG, pShaderObject_textureLight, TextureName::Gray, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLS, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLFix, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLFG, pShaderObject_textureLight, TextureName::Gray, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pLF2G, pShaderObject_textureLight, TextureName::Gray, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pMA, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pP0, pShaderObject_textureLight, TextureName::P0, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pP1, pShaderObject_textureLight, TextureName::P1, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pP2, pShaderObject_textureLight, TextureName::P2, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pP3, pShaderObject_textureLight, TextureName::P3, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pP4, pShaderObject_textureLight, TextureName::P4, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pPlAcc, pShaderObject_textureLight, TextureName::WallAcc, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pSconce, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pShelf, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pFloor, pShaderObject_textureLight, TextureName::Floor, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pRoof, pShaderObject_textureLight, TextureName::Roof, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWall0, pShaderObject_textureLight, TextureName::Wall0, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWall1, pShaderObject_textureLight, TextureName::Wall1, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWall2, pShaderObject_textureLight, TextureName::Wall2, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWall3, pShaderObject_textureLight, TextureName::Wall3, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWallSconceG, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pWinFrame, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pClockB0, pShaderObject_textureLight, TextureName::ClockB0, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pClockB2, pShaderObject_textureLight, TextureName::ClockB2, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pVase, pShaderObject_textureLight, TextureName::Vase, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pVaseBase, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pClockBW, pShaderObject_textureLight, TextureName::ClockBW, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pClockFrame, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	pGraphics_TextureLight = new GraphicsObject_TextureLight(pClockMark, pShaderObject_textureLight, TextureName::Black, color, pos);
	pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	pGameObj->SetScale(Scale);
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new GraphicsObject_TextureLight(pGlass, pShaderObject_textureLight, TextureName::Black, color, pos);
	//pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	//pGameObj->SetScale(Scale);
	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//Skydome Goes Last
	Model* pSkydome = ModelMan::addModel("./SIOStage/Skydome.mdl", ModelName::Sky);
	TextureMan::addPNGTexture("./SIOStage/Skydome.rvni", TextureName::Sky);

	GraphicsObject_FlatTexture* pGraphics_FT = new GraphicsObject_FlatTexture(pSkydome, pShaderObject_texture, TextureName::Sky);
	pGameObj = new GameObjectRigid(pGraphics_FT);
	pGameObj->SetScale(Vect(.8f, .8f,.8f));
	GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

#else

	//TODO:: FRIEND here
	Model* pAntenna = ModelMan::addModelSkinned("./FRIEND/antenna.smdl", ModelName::FAntenna);
	Model* pHairBlue = ModelMan::addModelSkinned("./FRIEND/backhair_blue.smdl", ModelName::FHairBlue);
	Model* pHairPink = ModelMan::addModelSkinned("./FRIEND/backhair_pink.smdl", ModelName::FHairPink);
	Model* pBangs = ModelMan::addModelSkinned("./FRIEND/bangs.smdl", ModelName::FBangs);
	Model* pbHair = ModelMan::addModelSkinned("./FRIEND/bhair.smdl", ModelName::FBHair);
	Model* pBClothes = ModelMan::addModelSkinned("./FRIEND/blackwhiteClothes.smdl", ModelName::FBWClothes);
	Model* pBlueClothes = ModelMan::addModelSkinned("./FRIEND/BlueClothes.smdl", ModelName::FBlueClothes);
	Model* pFBody = ModelMan::addModelSkinned("./FRIEND/body.smdl", ModelName::FBody);

	Model* pBrows = ModelMan::addModelSkinned("./FRIEND/brows.smdl", ModelName::FBrows);
	Model* pBWAcc = ModelMan::addModelSkinned("./FRIEND/BWAccesories.smdl", ModelName::FBWAcc);
	Model* pCheeks = ModelMan::addModelSkinned("./FRIEND/cheek.smdl", ModelName::FCheeks);

	Model* pEye = ModelMan::addModelSkinned("./FRIEND/eye.smdl", ModelName::FEye);
	Model* pEyeHi = ModelMan::addModelSkinned("./FRIEND/eye_hi0.smdl", ModelName::FEyeHi);
	Model* pFace0 = ModelMan::addModelSkinned("./FRIEND/face0.smdl", ModelName::FFace0);
	Model* pFace1 = ModelMan::addModelSkinned("./FRIEND/face1.smdl", ModelName::FFace1);

	Model* pFaceExpr = ModelMan::addModelSkinned("./FRIEND/facialExpr.smdl", ModelName::FFaceExpr);
	Model* pLashes = ModelMan::addModelSkinned("./FRIEND/lashes.smdl", ModelName::FLashes);
	//skipping  hairshadow , glasses and lens

	Model* pLooseStrands = ModelMan::addModelSkinned("./FRIEND/loosestrands.smdl", ModelName::FStrands);
	Model* pPinkAccents = ModelMan::addModelSkinned("./FRIEND/pinkAccents.smdl", ModelName::FPAcc);

	////Skipping red1 red2 tears2, transparentfaceEx
	Model* pUnderSkirt = ModelMan::addModelSkinned("./FRIEND/underskirt.smdl", ModelName::FUSkirt);
	Model* pUndies = ModelMan::addModelSkinned("./FRIEND/Undies.smdl", ModelName::FUndies);

	//
	TextureMan::addTexture("./FRIEND/mihair.tga", TextureName::FHairBlue);
	TextureMan::addTexture("./FRIEND/hair.tga", TextureName::FHairPink);
	TextureMan::addTexture("./FRIEND/DejaVu.tga", TextureName::FClothColor);
	TextureMan::addTexture("./FRIEND/body2.tga", TextureName::FBody);
	TextureMan::addTexture("./FRIEND/face02.tga", TextureName::FFace);

	TextureMan::addPNGTexture("./FRIEND/face02.rvni", TextureName::FFaceTransparent);

	TextureMan::addTexture("./FRIEND/eye2.tga", TextureName::FEye);
	TextureMan::addPNGTexture("./FRIEND/facials.rvni", TextureName::FFacialExpr);
	TextureMan::addTexture("./FRIEND/undies.tga", TextureName::FUndies);

	//NOTE!!!!!!!:: I think I can push the accumulated target to the gpu to debug, so take the bais and then blend all the channels onto that, then send it to the gpu. So I can put off doing it on the gpu till I know it works.
	pFRIEND = new CompositeAnimController2("./FRIEND/FRIEND.skl", "./FRIEND/FRIEND.pose",0);

	pFRIEND->addClipFromFile("./FRIEND/FRIEND.ani");

	//pFRIEND->addMesh(pAntenna, pSkinShader, TextureName::FHairBlue, false);
	//pFRIEND->addMesh(pHairBlue, pSkinShader, TextureName::FHairBlue, false);
	//pFRIEND->addMesh(pHairPink, pSkinShader, TextureName::FHairPink, false);
	//pFRIEND->addMesh(pBangs,    pSkinShader, TextureName::FHairBlue, false);
	//pFRIEND->addMesh(pbHair,    pSkinShader, TextureName::FHairBlue, false);
	//pFRIEND->addMesh(pBClothes, pSkinShader, TextureName::FClothColor, false);
	//pFRIEND->addMesh(pBlueClothes, pSkinShader, TextureName::FClothColor, false);
	//pFRIEND->addMesh(pFBody, pSkinShader, TextureName::FBody, false);

	pFRIEND->addMesh(pAntenna, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pHairBlue, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pHairPink, pSkinLightShader, TextureName::FHairPink, false, true);
	pFRIEND->addMesh(pBangs, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pbHair, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pBClothes, pSkinLightShader, TextureName::FClothColor, true, true);
	pFRIEND->addMesh(pBlueClothes, pSkinLightShader, TextureName::FClothColor, false, true);
	pFRIEND->addMesh(pFBody, pSkinLightShader, TextureName::FBody, false, true);

	pFRIEND->addMeshWithShapes(pCheeks, pSkinShaderShapeDefoLit, TextureName::FFaceTransparent, "./FRIEND/cheek.shape", true);
	pFRIEND->addMeshWithShapes(pFaceExpr, pSkinShaderShapeDefoLit, TextureName::FFacialExpr, "./FRIEND/faceExpr.shape");
	pFRIEND->addMeshWithShapes(pFace1, pSkinShaderShapeDefo, TextureName::FFaceTransparent, "./FRIEND/face1.shape", false);

	pFRIEND->addMeshWithShapes(pBrows, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/brows.shape");
	pFRIEND->addMesh(pBWAcc, pSkinLightShader, TextureName::FClothColor, false, true);
	//pFRIEND->addMesh(pBWAcc, pSkinShader, TextureName::FClothColor, false);

	pFRIEND->addMeshWithShapes(pEye, pSkinShaderShapeDefoLit, TextureName::FEye, "./FRIEND/eye.shape");
	pFRIEND->addMeshWithShapes(pEyeHi, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/eye_hi0.shape"); //disable lighting on this

	pFRIEND->addMeshWithShapes(pFace0, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/face0.shape");

	pFRIEND->addMeshWithShapes(pLashes, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/lashes.shape");

	//pFRIEND->addMesh(pLooseStrands, pSkinShader, TextureName::FHairBlue, false);
	//pFRIEND->addMesh(pPinkAccents, pSkinShader, TextureName::FClothColor, false);
	//pFRIEND->addMesh(pUnderSkirt, pSkinShader, TextureName::FClothColor, false);
	//pFRIEND->addMesh(pUndies, pSkinShader, TextureName::FUndies, false);

	pFRIEND->addMesh(pLooseStrands, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pPinkAccents, pSkinLightShader, TextureName::FClothColor, false, true);
	pFRIEND->addMesh(pUnderSkirt, pSkinLightShader, TextureName::FClothColor, true, true);
	pFRIEND->addMesh(pUndies, pSkinLightShader, TextureName::FUndies, false, true);

	////	pSIOL = new CompositeAnimController("./SIOL/SIOLv3.skl", "./SIOL/SIOLv3.pose");
	//
	////	pSIOL->addClipFromFile("./SIOL/SIOL.ani");
	//
	//pSIOL = new CompositeAnimController("./SIOL/SIOLv5.skl", "./SIOL/SIOLv5.pose");
	//
	//pSIOL->addClipFromFile("./SIOL/SIOL.ani");
	//
	//pSIOL->addMesh(LBlouse, pSkinShader, TextureName::LBlouse, false);
	//pSIOL->addMesh(LBody,   pSkinShader, TextureName::LBody, false);
	//pSIOL->addMesh(LEyeHi0, pSkinShader, TextureName::LEyeHi0, false);
	//pSIOL->addMesh(LEyeHi1, pSkinShaderWBT, TextureName::LEyeHi1, false);
	//pSIOL->addMesh(LFace0,  pSkinShader, TextureName::LFace0, false);
	//pSIOL->addMesh(LFace1, pSkinShader, TextureName::LFace1, false);
	//pSIOL->addMesh(LHair0, pSkinShader, TextureName::LHair0, false);
	//pSIOL->addMesh(LHair1, pSkinShader, TextureName::LHair1, false);
	//pSIOL->addMesh(LAcc0, pSkinShader, TextureName::LLightAcc0, true);
	//pSIOL->addMesh(LAcc1, pSkinShaderWBT, TextureName::LLightAcc1, false);
	//pSIOL->addMesh(LOther, pSkinShaderWBT, TextureName::LOther, false);
	//pSIOL->addMesh(LSkin, pSkinShader, TextureName::LSkin, false);
	//pSIOL->addMesh(LArms0, pSkinShader, TextureName::LArms0, false);
	//pSIOL->addMesh(LArms1, pSkinShader, TextureName::LArms0, true);

#endif

	//pSIOL->addClipFromFile

	//NOTE::197 bones for D1 more for D2 so I gotta make sure to upp the bone count in the shader

	//Dancer setup

	//pHIGAD1 = new(ScratchSpace::GetHeap(), Mem::Align::Byte_16, __FILE__, __LINE__) AnimController();
	//pHIGAD2 = new AnimController();
	//pHIGAD3 = new AnimController();
	//pHIGAD4 = new AnimController();

	//pHIGAD1->setSkellyFromFile("Dancer.skl");
	//pHIGAD2->setSkellyFromFile("Dancer.skl");
	//pHIGAD3->setSkellyFromFile("Dancer.skl");
	//pHIGAD4->setSkellyFromFile("Dancer.skl");

	//pHIGAD1->addClipFromFile("HIGAD1.ani");
	//pHIGAD2->addClipFromFile("HIGAD2.ani");
	//pHIGAD3->addClipFromFile("HIGAD3.ani");
	//pHIGAD4->addClipFromFile("HIGAD4.ani");

	//pHIGAD1->Skeleton.pHeriarchyRoot->SetScale(Vect(10.0f, 10.0f, 10.f));
	//pHIGAD2->Skeleton.pHeriarchyRoot->SetScale(Vect(10.0f, 10.0f, 10.f));
	//pHIGAD3->Skeleton.pHeriarchyRoot->SetScale(Vect(10.0f, 10.0f, 10.f));
	//pHIGAD4->Skeleton.pHeriarchyRoot->SetScale(Vect(10.0f, 10.0f, 10.f));

	//////Stage setup
	//GameObjectRigid *pGameObj;
	//GraphicsObject* pGraphics_TextureLight;

	//Vect color(1.50f, 1.50f, 1.50f, 1.5f);
	//Vect pos(1.0f, 100.0f, 1.0f);

	////Model* StageRound = ModelMan::addModel("001_Round.mdl", ModelName::Round);
	////Model* StageLine = ModelMan::addModel("002_Line.mdl", ModelName::Line);
	////Model* StageFloor = ModelMan::addModel("003_Floor.mdl", ModelName::Floor);

	//pGraphics_TextureLight = new GraphicsObject_TextureLight(StageRound, pShaderObject_textureLight, TextureName::Gray, color, pos);
	//pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new GraphicsObject_TextureLight(StageLine, pShaderObject_textureLight, TextureName::Gray, color, pos);
	//pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new GraphicsObject_TextureLight(StageFloor, pShaderObject_textureLight, TextureName::Gray, color, pos);
	//pGameObj = new GameObjectRigid(pGraphics_TextureLight);
	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////Treadmill1
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(-140.0f, -40.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//Matrix RotX = Matrix(ROT_X, MATH_PI2);

	//*pGameObj->pQRot = Quat(RotX);

	//GameObjectMan::Add(pGameObj,GameObjectMan::GetRoot());
	////Treadmill2
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(105.0f, -40.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);

	//*pGameObj->pQRot = Quat(RotX);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////Treadmill3
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(-65.0f, 40.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);
	//Matrix RotZ = Matrix(ROT_Z, MATH_PI);

	//*pGameObj->pQRot = Quat(RotX * RotZ);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());
	////Treadmill4
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(185.0f, 40.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);
	//RotZ = Matrix(ROT_Z, MATH_PI);

	//*pGameObj->pQRot = Quat(RotX * RotZ);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());
	////TreadMill5
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(-140.0f, 120.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);
	//

	//*pGameObj->pQRot = Quat(RotX);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////Treadmill6
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(105.0f, 120.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);

	//*pGameObj->pQRot = Quat(RotX);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////Treadmill7
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(-65.0f, 200.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);
	//RotZ = Matrix(ROT_Z, MATH_PI);
	//*pGameObj->pQRot = Quat(RotX * RotZ);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////Treadmill8
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(185.0f, 200.0f, 0.0f));
	//pGameObj->SetScale(Vect(100.00f, 100.00f, 100.00f));

	//RotX = Matrix(ROT_X, MATH_PI2);
	//RotZ = Matrix(ROT_Z, MATH_PI);
	//*pGameObj->pQRot = Quat(RotX * RotZ);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	////MEIKO
	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pMeiko_Mesh, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObjectRigid(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(185.0f, 200.0f, 30.0f));
	//pGameObj->SetScale(Vect(10.00f, 10.00f, 10.00f));

	////RotX = Matrix(ROT_X, MATH_PI2);
	////RotZ = Matrix(ROT_Z, MATH_PI);
	////*pGameObj->pQRot = Quat(RotX * RotZ);

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pTreadMill, pShaderObject_textureLight, TextureName::Gray, color, pos, StageHeapPtr);
	//pGameObj = new(StageHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, StageHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(1.00f, 1.00f, 1.00f));

	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pHIGAD1->Skeleton.pHeriarchyRoot->SetPos(Vect(0.0f, 0.0f, 0.0f, 0.0f));

	//pTeddyController2 =
	//	new(reservedHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__)
	//	AnimController();

	//pTeddyController2->setSkellyFromFile("teddy.skl");

	//pTeddyController2->addClipFromFile("walk0.ani");

	//pTeddyController2->Skeleton.pHeriarchyRoot->SetPos(Vect(0.0f, 0.0f, 0.0f));

	//pTeddyController3 =
	//	new(ScratchSpace::GetTexHeap(), Mem::Align::Byte_16, __FILE__, __LINE__)
	//	AnimController();

	//pTeddyController3->setSkellyFromFile("teddy.skl");

	//pTeddyController3->addClipFromFile("walk0.ani");

	//pTeddyController3->Skeleton.pHeriarchyRoot->SetPos(Vect(0.0f, -100.0f, 0.0f));

	/*pTeddyController4 =
		new(reservedHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__)
		AnimController();

	pTeddyController4->setSkellyFromFile("teddy.skl");

	pTeddyController4->addClipFromFile("walk0.ani");

	pTeddyController4->Skeleton.pHeriarchyRoot->SetPos(Vect(0.0f, -0.0f, 100.0f));*/

	// SET ANIMATION DATA
	//SetAnimationData(reservedHeapPtr);

	//SetAnimationPose(reservedHeapPtr, pFirstBone);

	//pGameObj->SetPos(Vect(3.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(0.1f, 0.1f, 0.1f));
	//pGameObj->deltaRot = 0.0005f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_Wireframe = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_Wireframe(bSphere, pShaderObject_color, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_Wireframe, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, -20.0f));

	//Sphere s;

	//RitterSphere(s, Astroboy->modelData , Astroboy->numVerts);

	//pGameObj->SetScale(Vect(s.rad*2 , s.rad*2 , s.rad*2));
	////pGameObj->SetPos(s.cntr);
	////pGameObj->SetScale(Vect(0.1f, 0.1f, 0.1f));
	//pGameObj->deltaRot = 0.05f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_FlatTexture = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_FlatTexture(pNoIdeaMod, pShaderObject_texture, TextureName::FRIG, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_FlatTexture, goHeapPtr);

	//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(Astroboy, pShaderObject_textureLight, TextureName::Astroboy, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, -20.0f));
	////pGameObj->SetScale(Vect(0.1f, 0.1f, 0.1f));
	//pGameObj->deltaRot = 0.05f;
	//GameObjectMan::Add(pGameObj, GameObjectMan::GetRoot());

	//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(Geth, pShaderObject_textureLight, TextureName::GethT, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(2.0f, 2.0f, 2.0f));
	//pGameObj->deltaRot = 0.0f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(Frig, pShaderObject_textureLight, TextureName::FRIG, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(3.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(0.1f, 0.1f, 0.1f));
	//pGameObj->deltaRot = 0.0005f;
	//GameObjectMan::Add(pGameObj);
	//
	//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pBuggy, pShaderObject_textureLight, TextureName::BuggyDiff, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, -5.0f, 0.0f));
	//pGameObj->SetScale(Vect(0.01f, 0.01f, 0.01f));
	//pGameObj->deltaRot = 0.0f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_TextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_TextureLight(pBuggy, pShaderObject_textureLight, TextureName::BuggySpec, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_TextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 0.0f, 0.0f));
	//pGameObj->SetScale(Vect(0.01f, 0.01f, 0.01f));
	//pGameObj->deltaRot = 0.0f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_CycleTextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_CycleTextureLight(pBuggy, pShaderObject_textureLight, TextureName::BuggyNrm, TextureName::BuggyDiff, TextureName::BuggySpec, TextureName::BuggyNrm, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_CycleTextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(0.0f, 5.0f, 0.0f));
	//pGameObj->SetScale(Vect(0.01f, 0.01f, 0.01f));
	//pGameObj->deltaRot = 0.0f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_CycleTextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_CycleTextureLight(Bear, pShaderObject_textureLight, TextureName::Warbear, TextureName::PolarBear, TextureName::PolarBear, TextureName::Warbear, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_CycleTextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(15.0f, 1.0f, 7.0f));
	//pGameObj->SetScale(Vect(0.075f, 0.075f, 0.075f));
	//pGameObj->deltaRot = 0.062f;
	//GameObjectMan::Add(pGameObj);

	//pGraphics_CycleTextureLight = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GraphicsObject_CycleTextureLight(Bear, pShaderObject_textureLight, TextureName::Warbear, TextureName::PolarBear, TextureName::Warbear, TextureName::PolarBear, color, pos, matHeapPtr);
	//pGameObj = new(goHeapPtr, Mem::Align::Byte_16, __FILE__, __LINE__) GameObject(pGraphics_CycleTextureLight, goHeapPtr);
	//pGameObj->SetPos(Vect(15.0f, 1.0f, -7.0f));
	//pGameObj->SetScale(Vect(0.025f, 0.025f, 0.025f));
	//pGameObj->deltaRot = 0.042f;
	//GameObjectMan::Add(pGameObj);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	//---------------------------------------------------------------------------------------------------------
	// Launch a Thread
	//---------------------------------------------------------------------------------------------------------

	// Create the snd system
	//SndManager::Create();
	//LoadManager::Create();
	//loadAudio();
	//// Name the main thread....
	//SetMainThreadName("GAME");

	//std::thread  audioThread(AudioMain, SndManager::GetG2ABuff(), SndManager::GetA2GBuff());
	//SetThreadName(audioThread, "AUDIO");
	//audioThread.detach();

	//std::thread  loadThread(LoadMain, LoadManager::GetG2LBuff(), LoadManager::GetL2GBuff());
	//SetThreadName(loadThread, "LOAD");
	//loadThread.detach();

}

//TODO:: add float * conversion operator to the math lib

//void drawBackground()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glDisable(GL_DEPTH_TEST);
//
//	// Background Image
//	glBindTexture(GL_TEXTURE_2D, 3);
//
//	glMatrixMode();
//	glLoadIdentity();
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(0, 1, 1, 0, -1, 1);
//
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glBegin(GL_QUADS);
//	glTexCoord2f(0, 0);
//	glVertex2f(0, 0);
//
//	glTexCoord2f(1, 0);
//	glVertex2f(1, 0);
//
//	glTexCoord2f(1, 1);
//	glVertex2f(1, 1);
//
//	glTexCoord2f(0, 1);
//	glVertex2f(0, 1);
//	glEnd();
//
//
//}

void Game::Draw()
{
	//todo: reflection, MotionBlur and DOF here
	//drawBackground();

	GameObjectMan::Draw();
}

void ProcessKeyboardInput();

void Game::Update()
{
	Time elapsedTime = this->intervalTimer.Toc();
	this->intervalTimer.Tic();
	int timeInSeconds_ms = Time::Quotient(elapsedTime, Time(Time::ONE_MILLISECOND));
	Trace::out("Frame ms:%d \n", timeInSeconds_ms);

	//Needs to be updated once a frame, Does it though? is there a way to detect changes to save on math.
	//this->intervalTimer.Toc();
	pCamTrack->CProcessAnimation(elapsedTime);
	//pCamTrack->CProcessAnimation(tDelta);

	Camera* pCam = CameraManager::GetActiveCamera();
	pCam->updateCamera();

	ProcessKeyboardInput();

	GameObjectMan::Update(this->globalTimer.Toc());

	//static int i = 0;

	//	Trace::out("%d: %f\n", i++, currentTime);

	

	
	static Time tCurrent(Time::ZERO);

	//pHIGAD1->CProcessAnimation(tDelta);
	//pHIGAD2->CProcessAnimation(tDelta);
	//pHIGAD3->CProcessAnimation(tDelta);
	//pHIGAD4->CProcessAnimation(tDelta);


#ifdef HUMAN

	pTeddyController2->CProcessAnimation(tDelta);

#elif SIO

	pSIOL->CProcessAnimation(tDelta);
	pSIOR->CProcessAnimation(tDelta);

#else

	pFRIEND->CProcessAnimation(elapsedTime);

#endif

	//ProcessAnimation(tCurrent);

	//pTeddyController2->CProcessAnimation(tDelta);
	//TODO:: make an animation manager to process all the animations or wrap this into the update method of a game object or something dynamic and clever

	//pTeddyController->SetAnimationPose(tDelta);

	//pTeddyController->Skeleton.pHeriarchyRoot->SetPos(Vect(000.0f, 0.0f, 0.0f));

	//pTeddyController2->CProcessAnimation(tDelta);

	//pTeddyController2->SetAnimationPose(tDelta);

	//pTeddyController2->Skeleton.pHeriarchyRoot->SetScale(Vect(.5f, .5f, .5f));
	//pTeddyController2->Skeleton.pHeriarchyRoot->SetPos(Vect(600.0f, -1288.917847f, -526.212402f));
	//pTeddyController2->Skeleton.pHeriarchyRoot->SetScale(Vect(.8f, .8f, .8f));

	//pTeddyController3->CProcessAnimation(tCurrent);
	//pTeddyController3->SetAnimationPose(tCurrent);

	//pTeddyController4->CProcessAnimation(tCurrent);
	//pTeddyController4->SetAnimationPose(tCurrent);

	//pTeddyController2->Skeleton.pHeriarchyRoot->SetPos(Vect(-10.f, 0.0f, 0.0f));

	//pTeddyController2->CProcessAnimation(tCurrent);
	//pTeddyController2->SetAnimationPose(tCurrent);

	//SetAnimationPose(pFirstBone, tCurrent);

	// update the time
	//Time deltaTime = 0.5f * Time(Time::NTSC_30_FRAME);
	//Time maxTime;

	//tCurrent += deltaTime;

	//FindMaxTime(maxTime);

	//static const Time tZero(Time::ZERO);

	//// protection for time values for looping
	//if (tCurrent < tZero)
	//{
	//	tCurrent = maxTime;
	//}
	//else if (tCurrent > maxTime)
	//{
	//	tCurrent = tZero;
	//}
	//else
	//{
	//	// do nothing
	//}
}

void Game::UnloadContent()
{

	//TODO:: make sure I can call all of theese when the managers are empty... just in case
	GameObjectMan::DeleteTree();

	ModelMan::deleteModels();

	ShaderManager::deleteShaders();

	TextureMan::deleteTextures();

	CameraManager::deleteCameras();

	InputMan::deleteInputs();


#ifdef HUMAN

	delete pTeddyController2;
#elif SIO

	delete pSIOL;

#else

	delete pFRIEND;

#endif

	//delete pSIOL;
	//delete pTeddyController;

	//delete pHIGAD1;
	//delete pHIGAD2;
	//delete pHIGAD3;
	//delete pHIGAD4;


	CameraManager::deleteManager();
	ModelMan::deleteManager();
	TextureMan::deleteManager();
	ShaderManager::deleteManager();

	//TODO:: move the following into a memfunction

	//TODO: find some way to check for leaks on the GPU
	//I think I can do it by setting up a heap for this and using proxy objects in the memory system to represent allocations and deletions for the gpu
	//I could just anytime I send something to the gpu I can create like a 4 byte object to represent it in the mem system and put it in a GPU object heap
	//It wouldn't be ideal but I could do paired allocations and deltions to ensure leak free GPU run as well
	//I don't really wanna build it into the engine because I just want to use it to check my understanding of how memory is being managed on the GPU.
	//NOTE: I already discovered that it's got a handle system built into the gpu for at least the liked objects so I can call delete on the compiled shader code immedetly after linking and it wont be destroyed until the program deleted

	//leaks are cleaned on Mem::Destroy() but it shouldn't leak ever because that means something's not working as intended and destructors wont be called so could cause leaks if dynamic storage in an object isn't also managed
	//if objects in Mem have objects that are outside of mem;


}

void Game::ClearBufferFunc()
{
	Engine::ClearBufferFunc();
}

//TODO:: optimize and switch from projection to a camera class
void Game::onResize(int w, int h)
{

	Engine::onResize(w, h);
	Camera* pCam = CameraManager::GetActiveCamera();
	assert(pCam);
	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(50.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 0.10f, 1000.0f);
	pCam->updateCamera();

};

//HELPER FUNCTIONS TO MOVE ELSEWHERE
//TODO:: move theese into fitting homes.
void ProcessKeyboardInput()
{

	//first check if I'm switching camera
	Keyboard*pKey = InputMan::GetKeyboard();

	//if (pKey->GetKeyState(KeyEnum::KEY_1) == true)
	//{
	//	CameraManager::setActive(Camera::CameraName::DEFAULT);
	//}
	/*if (pKey->GetKeyState(KeyEnum::KEY_2) == true)
	{
		CameraManager::setActive(Camera::CameraName::View1);
	}
	if (pKey->GetKeyState(KeyEnum::KEY_3) == true)
	{
		CameraManager::setActive(Camera::CameraName::View2);
	}
	if (pKey->GetKeyState(KeyEnum::KEY_4) == true)
	{
		CameraManager::setActive(Camera::CameraName::View3);
	}*/

	if (pKey->GetKeyState(KeyEnum::KEY_COMMA))
	{
		tDelta -= 0.005f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_PERIOD))
	{
		tDelta += 0.005f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_SPACE))
	{
		tDelta = 0.0f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_SLASH))
	{
		tDelta = 0.5f;
	}

	bool RBracketKeyCurr = pKey->GetKeyState(KeyEnum::KEY_RIGHT_BRACKET);
	if (RBracketKeyCurr == true && pKey->RBKeyPrev == false)
	{
		//pTeddyController->nextAnim();
		//pTeddyController2->nextAnim();
	}
	pKey->RBKeyPrev = RBracketKeyCurr;

	bool LBracketKeyCurr = pKey->GetKeyState(KeyEnum::KEY_LEFT_BRACKET);
	if (LBracketKeyCurr == true && pKey->LBKeyPrev == false)
	{
		//pTeddyController->prevAnim();
		//pTeddyController2->prevAnim();
	}
	pKey->LBKeyPrev = LBracketKeyCurr;

	bool OKeyCurr = pKey->GetKeyState(KeyEnum::KEY_B);
	if (OKeyCurr == true && pKey->OKeyPrev == false)
	{
		GameObjectMan::ToggleDebug();
	}
	pKey->OKeyPrev = OKeyCurr;

	//grab the active camera
	Camera* pCam = CameraManager::GetActiveCamera();

	//TODO:: can probably store it because I don't think I need to do this every frame

	Vect vUp;
	Vect vRight;
	Vect vDir;
	Vect vLookAt;
	Vect vPos;

	pCam->getUp(vUp);
	pCam->getRight(vRight);
	pCam->getDir(vDir);
	pCam->getLookAt(vLookAt);
	pCam->getPos(vPos);

	//Trace::out("camera: pos:%f %f %f  at:%f %f %f \n", vPos[x], vPos[y], vPos[z], vLookAt[x], vLookAt[y], vLookAt[z]);

	if (pKey->GetKeyState(KeyEnum::KEY_W) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 -= (10.0f*0.04f*vDir);
		vLookAt2 -= (10.0f*0.04f*vDir);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}

	if (pKey->GetKeyState(KeyEnum::KEY_S) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 += (0.04f*vDir);
		vLookAt2 += (0.04f*vDir);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}

	if (pKey->GetKeyState(KeyEnum::KEY_D) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 += (10.0f*0.04f*vRight);
		vLookAt2 += (10.0f*0.04f*vRight);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}

	if (pKey->GetKeyState(KeyEnum::KEY_A) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 -= (0.04f*vRight);
		vLookAt2 -= (0.04f*vRight);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}
	if (pKey->GetKeyState(KeyEnum::KEY_ARROW_UP) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 += (0.04f*vUp);
		vLookAt2 += (0.04f*vUp);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}

	if (pKey->GetKeyState(KeyEnum::KEY_LEFT_CONTROL) == true)
	{

		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;

		vPos2 -= (0.04f*vUp);
		vLookAt2 -= (0.04f*vUp);

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);

	}

	if (pKey->GetKeyState(KeyEnum::KEY_Q) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;
		Matrix rotY(RotType::ROT_Y, 0.02f);

		vLookAt2 = (vLookAt2 - vDir) * rotY;;
		//vLookAt2 = rotY * vLookAt;
		//vLookAt2 = (vLookAt2-vPos2) * rotY ;

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}
	if (pKey->GetKeyState(KeyEnum::KEY_E) == true)
	{
		Vect vPos2 = vPos;
		Vect vLookAt2 = vLookAt;
		Matrix rotY(RotType::ROT_Y, -0.02f);

		vLookAt2 = (vLookAt2 - vDir) * rotY;

		pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	}

	//if (pKey->GetKeyState(KeyEnum::KEY_Z) == true)
	//{
	//	Vect vPos2 = vPos;
	//	Vect vLookAt2 = vLookAt;
	//	Matrix rotX(RotType::ROT_X, 0.02f);

	//	vLookAt2 = (vLookAt2 - vPos2) * rotX;

	//	pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	//}
	//if (pKey->GetKeyState(KeyEnum::KEY_X) == true)
	//{
	//	Vect vPos2 = vPos;
	//	Vect vLookAt2 = vLookAt;
	//	Matrix rotX(RotType::ROT_X, -0.02f);

	//	vLookAt2 = (vLookAt2 - vPos2) * rotX;

	//	pCam->setOrientAndPosition(vUp, vLookAt2, vPos2);
	//}

}
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

#include "RvnTime.h"
#include "RvnTimer.h"
#include "PyramidModel.h"
#include "Anim.h"

#include "AnimController.h"
#include "CamAnimController.h"
#include "ScratchSpace.h"

//TODO:: move into model manager
#include "SkinnedModelFromFile.h"
#include "../AudioEngine/SndManager.h"
#include "../AudioEngine/LoadManager.h"
#include "ThreadHelper.h"
//#include "AudioInclude.h"//Needs Lots of work

//#define new(inSize, heapPtr, alignment) (inSize, heapPtr, alignment, __FILE__ , __LINE__)

//TODO:: get rid of all globals
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
//TODO:: eliminate externs and globals
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

float TimeScale = 1.0f;
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
	//todo add debug texture
	//this->pProjMatrix = new(matHeapPtr, Mem::Align::Byte_16, __FILE__ , __LINE__) Matrix(IDENTITY);
	//this->onResize(info.windowWidth, info.windowHeight);
}

//TODO:: DEMO:: setup 4 cameras, one that showcases a different section then move 
void Game::LoadContent()
{
	//swap everything into managers and push the storage to my memory management system passing in __FILE__ and __LINE__ as arguments to the manager create call

	Camera* pCam = new Camera();

	pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
	pCam->setPerspective(35.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 1.00f, 10000.0f);

	pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, 0.0f), Vect(0.0f, -50.0f, 15.0f));
	pCam->updateCamera();

	pCamTrack = new CamAnimController("./FRIEND/camera.rcam");

	//TODO:: setup a camera handle system
	CameraManager::addCamera(Camera::CameraName::DEFAULT, pCam, true);

	ShaderObject *pSkinLightShader = ShaderManager::addShader(ShaderObject::ShaderName::TextureSkinLight, "skinLightShader");
	ShaderObject *pSkinShaderShapeDefo = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlatSkinShapeDefo, "skinShapeDefo");
	ShaderObject *pSkinShaderShapeDefoLit = ShaderManager::addShader(ShaderObject::ShaderName::ShapeDefoLight, "shapeDefoLight");

	TextureMan::addTexture("Duckweed.tga", TextureName::DUCKWEED);
	TextureMan::addTexture("GrayTex.tga", TextureName::Gray);

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

	TextureMan::addTexture("./FRIEND/mihair.tga", TextureName::FHairBlue);
	TextureMan::addTexture("./FRIEND/hair.tga", TextureName::FHairPink);
	TextureMan::addTexture("./FRIEND/DejaVu.tga", TextureName::FClothColor);
	TextureMan::addTexture("./FRIEND/body2.tga", TextureName::FBody);
	TextureMan::addTexture("./FRIEND/face02.tga", TextureName::FFace);

	TextureMan::addPNGTexture("./FRIEND/face02.rvni", TextureName::FFaceTransparent);

	TextureMan::addTexture("./FRIEND/eye2.tga", TextureName::FEye);
	TextureMan::addPNGTexture("./FRIEND/facials.rvni", TextureName::FFacialExpr);
	TextureMan::addTexture("./FRIEND/undies.tga", TextureName::FUndies);

	//NOTE!!!!!!!:: I think I can push the accumulated target to the gpu to debug, so take the bias and then blend all the channels onto that, then send it to the gpu. So I can put off doing it on the gpu till I know it works.
	pFRIEND = new CompositeAnimController2("./FRIEND/FRIEND.skl", "./FRIEND/FRIEND.pose",0);

	pFRIEND->addClipFromFile("./FRIEND/FRIEND.ani");

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

	pFRIEND->addMeshWithShapes(pEye, pSkinShaderShapeDefoLit, TextureName::FEye, "./FRIEND/eye.shape");
	pFRIEND->addMeshWithShapes(pEyeHi, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/eye_hi0.shape"); //disable lighting on this
	pFRIEND->addMeshWithShapes(pFace0, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/face0.shape");
	pFRIEND->addMeshWithShapes(pLashes, pSkinShaderShapeDefoLit, TextureName::FFace, "./FRIEND/lashes.shape");

	pFRIEND->addMesh(pLooseStrands, pSkinLightShader, TextureName::FHairBlue, false, true);
	pFRIEND->addMesh(pPinkAccents, pSkinLightShader, TextureName::FClothColor, false, true);
	pFRIEND->addMesh(pUnderSkirt, pSkinLightShader, TextureName::FClothColor, true, true);
	pFRIEND->addMesh(pUndies, pSkinLightShader, TextureName::FUndies, false, true);
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

	Camera* pCam = CameraManager::GetActiveCamera();
	pCam->updateCamera();

	ProcessKeyboardInput();

	GameObjectMan::Update(this->globalTimer.Toc());

	pFRIEND->CProcessAnimation(elapsedTime);
}

void Game::UnloadContent()
{

	//TODO:: make sure I can call all of these when the managers are empty... just in case
	GameObjectMan::DeleteTree();

	ModelMan::deleteModels();

	ShaderManager::deleteShaders();

	TextureMan::deleteTextures();

	CameraManager::deleteCameras();

	InputMan::deleteInputs();

	delete pFRIEND;

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
		TimeScale -= 0.005f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_PERIOD))
	{
		TimeScale += 0.005f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_SPACE))
	{
		TimeScale = 0.0f;
	}
	if (pKey->GetKeyState(KeyEnum::KEY_SLASH))
	{
		TimeScale = 1.0f;
	}

	bool BKeyCurr = pKey->GetKeyState(KeyEnum::KEY_B);
	if (BKeyCurr == true && pKey->OKeyPrev == false)
	{
		GameObjectMan::ToggleDebug();
	}
	pKey->OKeyPrev = BKeyCurr;

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

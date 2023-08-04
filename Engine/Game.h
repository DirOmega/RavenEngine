#pragma once

#include "MathEngine.h"

#include "Engine.h"
#include "GameObject.h"
#include "Time.h"
#include "Timer.h"


class Game : public Engine
{
public:

	Game(const char* windowName, const int Width, const int Height);
	Game(const Game&) = delete;
	Game & operator=(const Game &) = delete;
	~Game() = default;

	virtual void Initialize();
	virtual void LoadContent();
	virtual void Update();
	virtual void Draw();
	virtual void UnloadContent();

	virtual void ClearBufferFunc();

	void onResize(int w, int h);

	Timer intervalTimer;
	Timer globalTimer;

	//TODO:: eww refactor
	//Matrix*         pProjMatrix;

private:

	Game();

	//GLuint          program;
	//GLuint          vao;
	//GLuint          buffer;
	//GLuint			uv_location;
	//GLuint			texture;
	//GLuint			texture_location;
	//GLint           mv_location;
	//GLint           proj_location;

	//Heap*			reservedHeapPtr; //this is for engine use. (default textures and models for bounding objects etc.)
	//float           aspect;
	//Heap*			matHeapPtr; //matrix heap
	//Heap*			texHeapPtr; //texture heap
	//Heap*			goHeapPtr; //game object heap
	//Heap*			camHeapPtr;
	//Heap*			modelHeapPtr;
	
	//GameObject *	pGameObj;

};

//DECLARE_MAIN(singlepoint_app)
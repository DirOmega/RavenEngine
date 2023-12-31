#include "Engine.h"
#include <string.h>
#include <assert.h>
#include "Trace.h"
#include "Model.h"
#include "ModelMan.h"
#include "ShaderObject.h"
#include "ShaderManager.h"
#include "../AudioEngine/SndManager.h"
#include "../AudioEngine/LoadManager.h"
#include "ThreadHelper.h"

//TODO:: refactor this out
Engine * Engine::app = 0;

//------------------------------------------------------------------
// Engine::Run()
//		This is the internal game loop that the engine runs on.
//------------------------------------------------------------------
void Engine::Run()
{
	Timer bootTime;
	Time result;

	bootTime.Tic();
	this->privPreInitalize();

	Initialize();

	setVsync(true);

	this->privPreLoadContent();

	LoadContent();
	result = bootTime.Toc();

	int timeInMS = Time::Quotient(result, Time(Time::ONE_MILLISECOND));
	Trace::out("Load Time is %i ms\n", timeInMS);
	

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && glfwWindowShouldClose(window) != GL_TRUE)
	{
		//update game state
		//probably do the time up here then?
		Update();

		//render
		ClearBufferFunc();

		Draw();

		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	UnloadContent();

	glfwDestroyWindow(window);

	glfwTerminate();

}

Engine::Engine() {}

Engine::Engine(const char* windowName, const int Width, const int Height)
{

	strcpy(info.title, windowName);
	info.windowWidth = Width;
	info.windowHeight = Height;

	info.majorVersion = 4;
	info.minorVersion = 3;

	info.samples = 8;
	info.flags.all = 0;
	info.flags.cursor = 1;

#ifdef _DEBUG
	info.flags.debug = 1;
#endif
}

Engine::~Engine()
{
}

void Engine::privPreInitalize()
{
	app = this;
	
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

    //---------------------------------------------------------------------------------------------------------
	// Launch Threads
	//---------------------------------------------------------------------------------------------------------

	// Create the snd system

	//loadAudio();
	//// Name the main thread....
	SetMainThreadName("GAME");

}

void Engine::privStartAudioEngine()
{
	//SndManager::Create();
	//LoadManager::Create();
	/*std::thread  audioThread(AudioMain, SndManager::GetG2ABuff(), SndManager::GetA2GBuff());
	SetThreadName(audioThread, "AUDIO");
	audioThread.detach();

	std::thread  loadThread(LoadMain, LoadManager::GetG2LBuff(), LoadManager::GetL2GBuff());
	SetThreadName(loadThread, "LOAD");
	loadThread.detach();*/
}


Engine::CODE Engine::privGetWindow()
{
	CODE retVal = CODE::FailedToGetWindow;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
	if (info.flags.debug)
#endif /* _DEBUG */
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
	if (info.flags.robust)
	{
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);

	window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	//window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, glfwGetPrimaryMonitor(), 0);
	if (window)
	{
		retVal = CODE::Succes;
	}

	///glfwSwapInterval(1); //this is Vsync
	
	return retVal;
}

void Engine::privPreLoadContent()
{
	if (privGetWindow() == CODE::Succes)
	{
		glfwMakeContextCurrent(window);

		glfwSetWindowSizeCallback(window, glfw_onResize);
		glfwSetKeyCallback(window, glfw_onKey);
		glfwSetMouseButtonCallback(window, glfw_onMouseButton);
		glfwSetCursorPosCallback(window, glfw_onMouseMove);
		glfwSetScrollCallback(window, glfw_onMouseWheel);

		if (!info.flags.cursor)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}

		gl3wInit();

#ifdef _DEBUG
		fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

		if (info.flags.debug)
		{
			if (gl3wIsSupported(4, 3))
			{
				glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			}
		}
	}
	else
	{
		//setup logging to explain failure
		assert(false);
	}

	int n = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
	Trace::out("Max Vertex Attribs = %i\n", n);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//BSphere's need to be moved to Engine's preLoadContent I want engine to do some stuff before it calls into games load content
	Model* bSphere = ModelMan::addModel("BSphere.mdl", ModelName::BSphere);
	UNUSED_VAR(bSphere);

	// Create/Load Shaders and program
	//Move basic texture and any debug shaders/textures to engine privPreLoadContent
	ShaderObject* pShaderObject_texture = ShaderManager::addShader(ShaderObject::ShaderName::TextureFlat, "textureRender");
	assert(pShaderObject_texture);

	ShaderObject* pShaderObject_textureLight = ShaderManager::addShader(ShaderObject::ShaderName::TexturePointLight, "texturePointLightDiff");
	assert(pShaderObject_textureLight);
}

void Engine::Initialize()
{
}

void Engine::Draw()
{
}

void Engine::LoadContent()
{
}

void Engine::UnloadContent()
{
}

void Engine::ClearBufferFunc()
{
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
	const GLfloat one = 1.0f;

	glViewport(0, 0, info.windowWidth, info.windowHeight);
	glClearBufferfv(GL_COLOR, 0, green);
	glClearBufferfv(GL_DEPTH, 0, &one);
}

void APIENTRY Engine::debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam)
{
	reinterpret_cast<Engine *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
}
#ifndef ENGINE_H
#define ENGINE_H

#include "GLibSetup.h"
#include "KeyEnum.h"
#include "Time.h"
#include "Timer.h"

//TODO:: asserts EVERYWHERE
//TODO:: reorder and organize for clarity and ease of use
//TODO:: switch out all #pragma once to #ifndef style header guards for compiler compatability
//TODO:: big 4 on all classes and also move constructor and move assignment on all classes.
//TODO:: mark all end chain classes as final including matrix and vector from the math library 
//TODO:: compiled graphic data .cgd is not taken use that for saving graphics objects 
//TODO:: create a refrence counting system or something to allow model data to be fliyweighted like textures but have objects able to be deleted between levels without deleting the models in use by other instances

#define UNUSED_VAR(x) (x)

class Engine
{
public:

	enum class CODE
	{
		Succes,
		Fail,
		FailedToGetWindow
	};
	//consider pushing title out of the app info because it doesn't appear to be needed often though width and height of the window does come up more often.
	//optimize futher by perhaps creating two layers of app info one for frequently used items and a second for less frequently used items and then still in the second object
	//puth title out into a pointer because it seems to be mostly useless.
	struct APPINFO
	{
		char title[128];
		int windowWidth;
		int windowHeight;
		int majorVersion;
		int minorVersion;
		int samples;
		union //this union is size of 1 int each flag is a bit field.
		{
			struct
			{
				unsigned int    fullscreen : 1;
				unsigned int    vsync : 1;
				unsigned int    cursor : 1;
				unsigned int    stereo : 1;
				unsigned int    debug : 1;
				unsigned int    robust : 1;
			};
			unsigned int        all;
		} flags;
	};

private:

	Engine(); //priv default constructor so end user can't call the wrong one.


	void privPreInitalize();
	void privPreLoadContent();

	Engine::CODE privGetWindow();

	static void APIENTRY debug_callback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		GLvoid* userParam);

public:



	Engine(const char* windowName, const int Width, const int Height);
	virtual ~Engine();

	virtual void Run();

	virtual void Initialize();
	virtual void LoadContent() = 0;
	virtual void Draw();
	virtual void Update() = 0;
	virtual void UnloadContent() = 0;

	//TODO:: move to .cpp
	virtual int GetKey(int key)
	{
		return glfwGetKey(window, key);
	}

	virtual int GetMouseButton(int button)
	{
		return glfwGetMouseButton(window, button);
	}

	virtual void GetCursorPos(float &xPos, float &yPos)
	{
		double xPosDbl;
		double yPosDbl;

		glfwGetCursorPos(window, &xPosDbl, &yPosDbl);
		
		xPos = (float)xPosDbl;
		yPos = (float)yPosDbl;

	}

	//optional override for differing clear buffer
	virtual void ClearBufferFunc();

	//TODO:: low priority move theese to the .cpp
	void setWindowTitle(const char * title)
	{
		glfwSetWindowTitle(window, title);
	}

	virtual void onResize(int w, int h)
	{
		info.windowWidth = w;
		info.windowHeight = h;
	}

	virtual void onKey(int key, int action)
	{
		UNUSED_VAR(key);
		UNUSED_VAR(action);
	}

	virtual void onMouseButton(int button, int action)
	{
		UNUSED_VAR(button);
		UNUSED_VAR(action);
	}

	virtual void onMouseMove(int x, int y)
	{
		UNUSED_VAR(x);
		UNUSED_VAR(y);
	}

	virtual void onMouseWheel(int pos)
	{
		UNUSED_VAR(pos);
	}

	virtual void onDebugMessage(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message)
	{
		UNUSED_VAR(source);
		UNUSED_VAR(type);
		UNUSED_VAR(id);
		UNUSED_VAR(severity);
		UNUSED_VAR(length);

#ifdef _WIN32
		OutputDebugStringA(message);
		OutputDebugStringA("\n");
#endif /* _WIN32 */
	}

	void getMousePosition(int& x, int& y)
	{
		double dx, dy;
		glfwGetCursorPos(window, &dx, &dy);

		x = static_cast<int>(floor(dx));
		y = static_cast<int>(floor(dy));
	}

public:

protected:

	APPINFO     info;
	static      Engine * app;
	GLFWwindow* window;

	static void glfw_onResize(GLFWwindow* window, int w, int h)
	{
		UNUSED_VAR(window);
		app->onResize(w, h);
	}

	static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		UNUSED_VAR(window);
		UNUSED_VAR(scancode);
		UNUSED_VAR(mods);
		app->onKey(key, action);
	}

	static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		UNUSED_VAR(window);
		UNUSED_VAR(mods);
		app->onMouseButton(button, action);
	}

	static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
	{
		UNUSED_VAR(window);
		app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
	}

	static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		UNUSED_VAR(window);
		UNUSED_VAR(xoffset);
		app->onMouseWheel(static_cast<int>(yoffset));
	}

	void setVsync(bool enable)
	{
		info.flags.vsync = enable ? 1u : 0u;
		glfwSwapInterval((int)info.flags.vsync);
	}

};

#endif
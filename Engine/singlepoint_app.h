#pragma once

#include "MathEngine.h"
#include "MemEngine.h"
#include "Engine.h"

// Remove this to draw only a single cube!
// #define MANY_CUBES
class singlepoint_app : public Engine//public sb7::application //TODO:: inherit from engine instead of sb7 so I can start edging that fucking thing out.
{
public:

	singlepoint_app();

	void init();
	virtual void startup();
	virtual void render(double currentTime);
	virtual void shutdown();
	void onResize(int w, int h);

private:

	GLuint          program;
	GLuint          vao;
	GLuint          buffer;
	GLint           mv_location;
	GLint           proj_location;

	float           aspect;
	Heap*			 heapPtr;
	Matrix*          proj_matrix;

};

//DECLARE_MAIN(singlepoint_app)
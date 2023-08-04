#include "DebugCube.h"

const char* DebugCube::fn = "DebugCube.mdl";

Vert_xyzuvn DebugCube::vertList[] =
{

	//8 verts

	//front verts
	//{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f, 0.6f,  0.6f,  0.6f} // D I O
	//{-0.25f, 0.25f, 0.25f, 0.25f, 0.75f,-0.6f,  0.6f, 0.6f } // A H N
	//{0.25f, -0.25f, 0.25f, 0.50f, 0.50f,  } // C J T
	//{-.25f, -.25f, 0.25f, 0.25f, 0.5f} //B G Q

	//rear verts
	//{.25f, .25f, -.25f, 0.0f, 0.0f} //P L U
	//{-.25f, .25f, -.25f, 0.0f,0.0f} //M E X
	//{.25f , -.25f, -.25f, 0.0f,0.0f} // V K S
	//{-.25f, -.25f, -.25f, 0.0f,0.0f} //F R W

	//Front face
	//Triangle 0 = A D C
	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f, -0.6f,  0.6f, 0.6f },
	{  0.25f, 0.25f, 0.25f, 0.50f, 0.75f,  0.6f,  0.6f, 0.6f },
	{  0.25f,-0.25f, 0.25f, 0.50f, 0.50f,  0.6f, -0.6f, 0.6f },

		//Triangle 1 = A C B				
	{ -0.25f,  0.25f, 0.25f, 0.25f, 0.75f ,-0.6f,  0.6f, 0.6f },
	{  0.25f, -0.25f, 0.25f, 0.50f, 0.50f , 0.6f, -0.6f, 0.6f },
	{ -0.25f, -0.25f, 0.25f, 0.25f, 0.5f  ,-0.6f, -0.6f, 0.6f },

		//Right face						
		//Triangle 2 = I L K				
	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,0.6f,  0.6f, 0.6f },
	{ .25f, .25f, -.25f, .75f, 0.75f     ,0.6f,  0.6f,  -0.6f },
	{ .25f , -.25f, -.25f, .75f,0.50f    ,0.6f, -0.6f, -0.6f },

		//Triangle 3 = I K J			
	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,0.6f, 0.6f, 0.6f },
	{ 0.25f , -0.25f, -0.25f, 0.75f,0.50f,0.6f, -0.6f, -0.6f },
	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,0.6f, -0.6f, 0.6f },

		//Left face							
		//Triangle 4 = E H G					
	{ -.25f, .25f, -.25f, 0.0f,0.75f     ,-0.6f, 0.6f, -0.6f },
	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f ,-0.6f, 0.6f, 0.6f },
	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, 0.6f },

		//Triangle 5 = E G F					 
	{ -.25f, .25f, -.25f, 0.0f,0.75f      ,-0.6f, 0.6f, -0.6f },
	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, 0.6f },
	{ -.25f, -.25f, -.25f, 0.0f,0.50f    ,-0.6f, -0.6f, -0.6f },

		//Top Face						
		//Triangle 6 =  M P O					 
	{ -.25f, .25f, -.25f, 0.25f,1.0f     ,-0.6f, 0.6f, -0.6f },
	{ .25f, .25f, -.25f, 0.50f, 1.0f     ,0.6f, 0.6f, -0.6f },
	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,0.6f, 0.6f, 0.6f },

		//Triangle 7 = M O N					
	{ -.25f, .25f, -.25f, 0.25f,1.0f     ,-0.6f, 0.6f, -0.6f },
	{ 0.25f, 0.25f, 0.25f, 0.50f, 0.75f  ,0.6f, 0.6f, 0.6f },
	{ -0.25f, 0.25f, 0.25f, 0.25f, 0.75f ,-0.6f, 0.6f, 0.6f },

		//Bottom Face 						 
		//Triangle 8 = Q T S					
	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, 0.6f },
	{ 0.25f, -0.25f, 0.25f, 0.50f, 0.50f ,0.6f, -0.6f, 0.6f },
	{ .25f , -.25f, -.25f, 0.50f,0.25f   ,0.6f, -0.6f, -0.6f },

		//Triangle 9 = Q S R					
	{ -.25f, -.25f, 0.25f, 0.25f, 0.5f   ,-0.6f, -0.6f, 0.6f },
	{ .25f , -.25f, -.25f, 0.50f,0.25f   ,0.6f, -0.6f, -0.6f },
	{ -.25f, -.25f, -.25f, 0.25f,0.25f   ,-0.6f, -0.6f, -0.6f },

		//Back Face							
		//Triangle 10 = U X W					
	{ .25f, .25f, -.25f, 0.75f, 0.75f    ,0.6f, 0.6f, -0.6f },
	{ -.25f, .25f, -.25f, 1.0f,0.75f     ,-0.6f, 0.6f, -0.6f },
	{ -.25f, -.25f, -.25f, 1.0f,0.50f    ,-0.6f, -0.6f, -0.6f },

		//Triangle 11 = U W V					
	{ .25f, .25f, -.25f, 0.75f, 0.75f     ,0.6f, 0.6f, -0.6f },
	{ -.25f, -.25f, -.25f, 1.0f,0.50f    ,-0.6f, -0.6f, -0.6f },
	{ .25f , -.25f, -.25f, 0.75f,0.50f   ,0.6f, -0.6f, -0.6f }

};

Tri_index DebugCube::triList[] =
{
	{ 0,1,2 },
	{ 3,4,5 },
	{ 6,7,8 },
	{ 9,10,11 },
	{ 12,13,14 },
	{ 15,16,17 },
	{ 18,19,20 },
	{ 21,22,23 },
	{ 24,25,26 },
	{ 27,28,29 },
	{ 30,31,32 },
	{ 33,34,35 }
};

unsigned int DebugCube::getVertCount()
{
	return sizeof(vertList)/sizeof(Vert_xyzuvn);
}

unsigned int DebugCube::getTriArraySize()
{
	return sizeof(triList);
}

unsigned int DebugCube::getVertArraySize()
{
	return sizeof(vertList);
}

unsigned int DebugCube::getTriCount()
{
	return sizeof(triList)/sizeof(Tri_index);
}

const char* DebugCube::getFileName()
{
	return fn;
}
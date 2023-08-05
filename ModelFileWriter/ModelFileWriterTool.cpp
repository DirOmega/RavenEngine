#include "FileSystem\include\File.h"
#include "ModelStructs.h"
#include "Models.h"

/*
File Format needs work
First unsigned int (4 bytes) is number of verts data xyzuvn to read
Second unsigned int (4 bytes) is the number of tri indexes to read
*/

void main()
{

	File::Handle fh;
	//Basic Cube
	if (File::Error::SUCCESS == File::Open(fh, BasicCube::getFileName(), File::Mode::READ_WRITE))
	{

		Vert_xyzuvn *cubeData = BasicCube::getVertList();
		Tri_index   *triList = BasicCube::getTriIndexList();

		unsigned int t[2];
		t[0] = BasicCube::getVertCount();
		t[1] = BasicCube::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, BasicCube::getVertArraySize());
		File::Write(fh, triList, BasicCube::getTriArraySize());

		File::Close(fh);
	}

	//Debug Cube
	if (File::Error::SUCCESS == File::Open(fh, DebugCube::getFileName(), File::Mode::READ_WRITE))
	{
		
		Vert_xyzuvn *cubeData = DebugCube::getVertList();
		Tri_index   *triList  = DebugCube::getTriIndexList();

		unsigned int t[2];
		t[0] = DebugCube::getVertCount();
		t[1] = DebugCube::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, DebugCube::getVertArraySize());
		File::Write(fh, triList,  DebugCube::getTriArraySize());

		File::Close(fh);
	}

	//Pryamid
	if (File::Error::SUCCESS == File::Open(fh, Pryamid::getFileName(), File::Mode::READ_WRITE))
	{

		Vert_xyzuvn *cubeData = Pryamid::getVertList();
		Tri_index   *triList = Pryamid::getTriIndexList();

		unsigned int t[2];
		t[0] = Pryamid::getVertCount();
		t[1] = Pryamid::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, Pryamid::getVertArraySize());
		File::Write(fh, triList, Pryamid::getTriArraySize());

		File::Close(fh);
	}

	if (File::Error::SUCCESS == File::Open(fh, Diamond::getFileName(), File::Mode::READ_WRITE))
	{
		Vert_xyzuvn *cubeData = Diamond::getVertList();
		Tri_index   *triList = Diamond::getTriIndexList();

		unsigned int t[2];
		t[0] = Diamond::getVertCount();
		t[1] = Diamond::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, Diamond::getVertArraySize());
		File::Write(fh, triList, Diamond::getTriArraySize());

		File::Close(fh);
	}

	if(File::Error::SUCCESS == File::Open(fh, NoIdea::getFileName(), File::Mode::READ_WRITE))
	{
		Vert_xyzuvn *cubeData = NoIdea::getVertList();
		Tri_index   *triList = NoIdea::getTriIndexList();

		unsigned int t[2];
		t[0] = NoIdea::getVertCount();
		t[1] = NoIdea::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, NoIdea::getVertArraySize());
		File::Write(fh, triList, NoIdea::getTriArraySize());

		File::Close(fh);

	}

	if (File::Error::SUCCESS == File::Open(fh, BSphere::getFileName(), File::Mode::READ_WRITE))
	{

		Vert_xyzuvn *cubeData = BSphere::getVertList();
		Tri_index   *triList  = BSphere::getTriIndexList();

		unsigned int t[2];
		t[0] = BSphere::getVertCount();
		t[1] = BSphere::getTriCount();

		File::Write(fh, t, sizeof(t));
		File::Write(fh, cubeData, BSphere::getVertArraySize());
		File::Write(fh, triList, BSphere::getTriArraySize());

		File::Close(fh);




	}

}
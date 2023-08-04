#include "Texture.h"
#include "TextureMan.h"
#include "baseManager.h"
#include "TGALoader.h"
#include <assert.h>

#include "eat.h"

#include "ScratchSpace.h"


//TODO:: remove not great png loading, move over into different file format
#define UNUSED_VAR(x) (x)

#pragma pack(1)
typedef struct
{
	GLbyte	identsize;						// Size of ID field that follows header (0)
	GLbyte	colorMapType;					// 0 = None, 1 = paletted
	GLbyte	imageType;						// 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;			// bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte	bits;							// bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;						// image descriptor

} TGAHEADER;
#pragma pack(8)

TextureMan * TextureMan::pInstance = 0;

TextureMan::TextureMan()
	: baseManager(3)
{
	generateReserves(5);
	//this->active = 0;
};
TextureMan::TextureMan(int numStart, int deltaAdd)
	: baseManager(deltaAdd)
{
	generateReserves(numStart);
}

TextureMan::~TextureMan()
{
	deleteTextures();
	//DLInks are deleted via the base class
	//and textures are removed off the gpu by the tex link destructor
	//and textures are removed off the gpu by the texLink dClean() when they are recycled
};

TextureMan* TextureMan::privGetInstance()
{
	if (pInstance == nullptr)
	{
		TextureMan::Create();
	}
	return pInstance;
};


void TextureMan::Create(int numStart, int deltaAdd)
{
	assert(numStart > 0); //makes sure that it always starts with at least one
	assert(deltaAdd > 0); // makes sure that it's usable because if this is 0 or negative the manager would not create additonal resources when it needs to.
	assert(pInstance == nullptr);

	if (pInstance == nullptr)
	{
		pInstance = new TextureMan(numStart, deltaAdd);
		//TextureMan::addTexture("HotPink.tga", TextureName::NOT_INITIALIZED); //adds the hot pink texture with the default name so if a object can't find it's texture it will show as pink.
	}
}

//TODO:: this is bad I need to recycle theese to reserve 
void TextureMan::deleteTextures()
{
	TextureMan *pTextMan = TextureMan::privGetInstance();

	TexLink* pNode = (TexLink*)pTextMan->getActiveHead();
	while (pNode != 0)
	{
		//this recycles it to the resrve list when it's removed from active it's cleaned so the texture is freed from the gpu
		pTextMan->Remove((DLink&)*pNode);
		pNode = (TexLink*)pTextMan->getActiveHead();
	}
};

void TextureMan::deleteManager()
{
	delete TextureMan::privGetInstance();
}

DLink* TextureMan::dCreateNode()
{
	DLink* newNode = new TexLink();
	assert(newNode != nullptr);
	return newNode;
};

void TextureMan::dClearNode(DLink* pLink)
{
	pLink->dClean();
};

bool TextureMan::dCompareNodes(DLink& pLinkA, DLink& pLinkB)
{
	UNUSED_VAR(pLinkA);
	UNUSED_VAR(pLinkB);
	assert(false);
	/*if ((TexLink)pLinkA == (TexLink)pLinkB)
	{
		return true;
	}*/
	return false;
};

void TextureMan::addTexture(const char * const _packageName, const char * const _assetName, const TextureName _name)
{
	GLuint               textureID;
	GLuint *pTextureID = &textureID;

	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();
	assert(pTextMan != nullptr);

	// Load the texture and get the textureID
	pTextMan->privLoadTexture(_packageName, _assetName, pTextureID);

	// get a node from the textureman
	TexLink *pNode = (TexLink*)pTextMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

void TextureMan::addTexture(const char * const _assetName, const TextureName _name)
{
	GLuint               textureID;
	GLuint *pTextureID = &textureID;

	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();
	assert(pTextMan != nullptr);

	// Load the texture and get the textureID
	pTextMan->privLoadTexture(_assetName, pTextureID);

	// get a node from the textureman
	TexLink *pNode = (TexLink*) pTextMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_REPEAT);
	
};

//TODO:: use basefind and implement the dCompare but I would need to add a refrence version of the objects
//TODO:: adapt the implementation from the camera manager
GLuint TextureMan::find(const TextureName _name)
{
	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();

	//TexLink *pNode = (TexLink *)pTextMan->active;
	TexLink *pNode = (TexLink *)pTextMan->getActiveHead();
	while (pNode != 0)
	{
		if (pNode->name == _name)
		{
			// found it
			break;
		}

		pNode = (TexLink *)pNode->pNext;
	}
	assert(pNode);
	return pNode->textureID;
};

void TextureMan::privLoadTexture(const char * const _assetName, GLuint *&textureID)
{
	glGenTextures(1, textureID);

	// Bind it.
	glBindTexture(GL_TEXTURE_2D, *textureID);

	// Load the texture
	this->privLoadTGATexture(_assetName, GL_LINEAR, GL_LINEAR, GL_REPEAT);
};

void TextureMan::privLoadTexture(const char * const _packageName, const char * const _assetName, GLuint*& textureID)
{
	glGenTextures(1, textureID);

	// Bind it.
	glBindTexture(GL_TEXTURE_2D, *textureID);

	// Loat the texture
	this->privLoadTGATexture(_packageName,_assetName, GL_LINEAR, GL_LINEAR, GL_REPEAT);

}

////TODO:: include the data for the texture format IN the package or build in conversion to TGA into the converter.
//bool TextureMan::privLoadPNGTexture(const char*_PackageName, const char *_assetName, GLint minFilter, GLint magFilter, GLint wrapMode)
//{
//
//	TGATex* pBits = TGALoader::LoadTGA(_PackageName, _assetName);
//	if (pBits == nullptr)
//		return false;
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapMode);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapMode);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);
//
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	glTexImage2D(GL_TEXTURE_2D, 0, pBits->hdr.getComponents(), pBits->hdr.width, pBits->hdr.height, 0, pBits->hdr.getFormat(), GL_UNSIGNED_BYTE, pBits->data);
//	//glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
//
//	//free(pBits);
//	delete pBits;
//
//	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
//		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
//		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
//		minFilter == GL_NEAREST_MIPMAP_NEAREST)
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//	return true;
//
//
//}

bool TextureMan::privLoadTGATexture(const char *_PackageName, const char * _assetName, GLint minFilter, GLint magFilter, GLint wrapMode)
{

	TGATex* pBits = TGALoader::LoadTGA(_PackageName, _assetName);
	if (pBits == nullptr)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, pBits->hdr.getComponents(), pBits->hdr.width, pBits->hdr.height, 0, pBits->hdr.getFormat(), GL_UNSIGNED_BYTE, pBits->data);
	//glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	//free(pBits);
	delete pBits;

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;

}

bool TextureMan::privLoadTGATexture(const char * szFileName, GLint minFilter, GLint magFilter, GLint wrapMode)
{
	/*GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
*/
	// Read the texture bits
	//pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	TGATex* pBits = TGALoader::LoadTGA(szFileName);
	if (pBits == nullptr)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, pBits->hdr.getComponents(), pBits->hdr.width, pBits->hdr.height, 0, pBits->hdr.getFormat(), GL_UNSIGNED_BYTE, pBits->data);

	//glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	//free(pBits);
	delete pBits;

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;
};


bool TextureMan::privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode)
{

	TGATex* pBits = TGALoader::LoadTGA(szFileName);
	if (pBits == nullptr)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)horizWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)vertWrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, pBits->hdr.getComponents(), pBits->hdr.width, pBits->hdr.height, 0, pBits->hdr.getFormat(), GL_UNSIGNED_BYTE, pBits->data);
	//glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	//free(pBits);
	delete pBits;

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;

}

void TextureMan::addPNGTexture(const char * const _assetName, const TextureName _name)
{
	GLuint               textureID;
	GLuint *pTextureID = &textureID;

	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();
	assert(pTextMan != nullptr);

	// Load the texture and get the textureID
	pTextMan->privLoadPngTexture(_assetName, pTextureID);

	// get a node from the textureman
	TexLink *pNode = (TexLink*)pTextMan->baseAdd();
	assert(pNode != nullptr);

	// initialize it
	pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_REPEAT);
}

void TextureMan::privLoadPngTexture(const char * const _assetName, GLuint *&textureID)
{

	//*textureID = SOIL_load_OGL_texture(_assetName, 0, 0, SOIL_FLAG_TEXTURE_REPEATS);

	glGenTextures(1, textureID);
	
	// Bind it.
	glBindTexture(GL_TEXTURE_2D, *textureID);
	
	// Load the texture
	this->privLoadPNGTexture(_assetName, GL_LINEAR, GL_LINEAR, GL_REPEAT);

};

//bool TextureMan::privLoadPNGTexture(const char *_PackageName, const char * _assetName, GLint minFilter, GLint magFilter, GLint wrapMode)
//{
//
//
//}

bool TextureMan::privLoadPNGTexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode)
{

	UNUSED_VAR(szFileName);
	UNUSED_VAR(minFilter);
	UNUSED_VAR(wrapMode);
	//uncompress the png's in the converter

	//unsigned char* buffer = 0;
	//unsigned long imageSize;
	//std::vector<unsigned char> image;
	//picoPNG::loadFile(buffer, imageSize, szFileName);
	//unsigned long w;
	//unsigned long h;
	//picoPNG::decodePNG(image, w, h, buffer, imageSize);
	//
	//pngInvert(image, w, h);

	File::Handle fh;
	File::Open(fh, szFileName, File::Mode::READ);

	unsigned int w;
	unsigned int h;

	File::Read(fh, &w, sizeof(int));
	File::Read(fh, &h, sizeof(int));

	auto intsize = sizeof(int);
	auto arraysize = w * h * 4;

	int* image = new int[arraysize];

	File::Read(fh, image, arraysize);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	delete[] image;
	File::Close(fh);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)g, 0, GL_BGRA, GL_UNSIGNED_BYTE, &image[0]);
	//glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	//free(pBits);
	//delete pBits;
	//delete buffer;
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}


//bool TextureMan::privLoadPNGTexture(const char *szFileName, GLint minfilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode)
//{
//
//}

//void TextureMan::privAddToFront(TexLink *node, TexLink *&head)
//{
//	UNUSED_VAR(node);
//	UNUSED_VAR(head);
//};

//GLbyte * TextureMan::gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeght, GLint *iComponents, GLenum * eFormat)
//{
//	UNUSED_VAR(szFileName);
//	UNUSED_VAR(iWidth);
//	UNUSED_VAR(iHeght);
//	UNUSED_VAR(iComponents);
//	UNUSED_VAR(eFormat);
//};

#pragma once

#include "GLibSetup.h"
#include "baseManager.h"
#include "TexLink.h"


// Singleton
class TextureMan : public baseManager
{
public:

	//TODO:: add return codes for all void methods
	static void addTexture(const char * const _assetName, const TextureName _name);
	static void addPNGTexture(const char * const _assetName, const TextureName _name);

	static void addTexture(const char * const _packageName, const char * const _assetName, const TextureName _name);
	static void deleteTextures();
	static void deleteManager();
	static GLuint find(const TextureName _name);	

	static void Create(int numStart = 5, int deltaAdd = 3);

private:  // methods

	TextureMan();
	TextureMan(int numStart, int deltaAdd);
	virtual ~TextureMan();
	TextureMan(const TextureMan &) = delete;
	TextureMan(const TextureMan &&) = delete;
	TextureMan operator = (const  TextureMan & ) = delete;
	TextureMan operator = (const TextureMan && ) = delete;

	static TextureMan *privGetInstance();
	
	DLink* dCreateNode() override;
	bool dCompareNodes(DLink& pLinkA, DLink& pLinkB) override;
	void dClearNode(DLink* pLink) override;

	// helper private methods
	void privLoadTexture(const char * const _assetName, GLuint *&textureID);
	void privLoadTexture(const char * const _packageName, const char * const _assetName, GLuint *&textureID);
	bool privLoadTGATexture(const char *_PackageName, const char * _assetName, GLint minFilter, GLint magFilter, GLint wrapMode);
	bool privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode);
	bool privLoadTGATexture(const char *szFileName, GLint minfilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
	

	void privLoadPngTexture(const char * const _assetName, GLuint *&textureID);
//	bool privLoadPNGTexture(const char *_PackageName, const char * _assetName, GLint minFilter, GLint magFilter, GLint wrapMode);
	bool privLoadPNGTexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint wrapMode);
//	bool privLoadPNGTexture(const char *szFileName, GLint minfilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);

	//void privAddToFront(TexLink *node, TexLink *&head);
	//GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

private:  // add

	static TextureMan* pInstance;
	//static Texture poRefTex; "for searching"
	//TexLink *active;

};
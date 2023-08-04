//#pragma once
//
//#include "TexLink.h"
//#include "TGALoader.h"
//
//class Texture : public TexLink
//{
//public:
//
//	TGAHeader hdr;
//	unsigned int* data;
//
//	Texture()
//	{
//
//	}
//
//	virtual ~Texture()
//	{
//		if (data != nullptr)
//		{
//			delete data;
//		}
//	}
//	Texture(const Texture&) = delete;
//	Texture(const Texture&&) = delete;
//	Texture operator=(const Texture&) = delete;
//	Texture operator=(const Texture&&) = delete;
//
//};
//
//
//class TGATex : public Texture
//{
//
//public:
//
//	//todo: optimize the structure layout for size, it's got padding that it doesn't need but that's the way it's defined 
//
//	//TODO: set this up for debuging and make it part of the actual TGA data so i don't have to double deref.
//	//char* imageName
//	//TGAHeader hdr;
//
//	TGAHeader hdr;
//	unsigned int* data;
//
//	TGATex()
//	{
//
//	}
//
//	~TGATex()
//	{
//		if (data != nullptr)
//		{
//			delete data;
//		}
//	}
//
//	TGATex(TGAHeader& _hdr, unsigned int* _dataptr) : hdr(_hdr), data(_dataptr)
//	{
//	}
//
//};
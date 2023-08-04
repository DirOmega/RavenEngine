#pragma once

#include "File.h"
#include "Trace.h"
#include "ChunkHeader.h"
#include "eat.h"

#include "ScratchSpace.h"
//TODO:: set up texture factory and manager 

//
//class Texture
//{
//
//
//};
//
////setup multiple bit detph support

//TODO: reorder to eliminate padding
//TODO: push the get functions into TGATex
struct TGAHeader
{
	unsigned char identsize;
	unsigned char colourmaptype;
	unsigned char imagetype;
	unsigned char pad0;

	short int colourmapstart;
	short int colourmaplength;
	unsigned char colourmapbits;
	unsigned char pad1;

	short int xstart;
	short int ystart;
	short int width;
	short int height;
	unsigned char bits;
	unsigned char descriptor;

	GLint getComponents()
	{
		GLint retval = 0;
		short sDepth = bits / 8;
		if (sDepth == 3)
		{
			retval = GL_SRGB;
		}
		else if (sDepth == 2)
		{
			retval = GL_RGBA;
		}
		else
		{
			assert(false);
		}

		return retval;
	}

	GLuint getFormat()
	{
		GLuint retval = 0;
		short sDepth = bits / 8;

		if (sDepth == 3)
		{
			retval = GL_BGR;
		}
		else if (sDepth == 2)
		{
			retval = GL_BGRA;
		}
		else
		{
			assert(false);
		}

		return retval;
	}

};

//TODO:: check above vs below I wanna make a much neater header copy operation

//#pragma pack(1)
//typedef struct
//{
//	GLubyte	identsize;						// Size of ID field that follows header (0)
//	GLubyte	colorMapType;					// 0 = None, 1 = paletted
//	GLubyte	imageType;						// 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
//	unsigned short	colorMapStart;          // First colour map entry
//	unsigned short	colorMapLength;         // Number of colors
//	unsigned char 	colorMapBits;			// bits per palette entry
//	unsigned short	xstart;                 // image x origin
//	unsigned short	ystart;                 // image y origin
//	unsigned short	width;                  // width in pixels
//	unsigned short	height;                 // height in pixels
//	GLubyte	bits;							// bits per pixel (8 16, 24, 32)
//	GLubyte	descriptor;						// image descriptor
//} TGAHEADER;
//#pragma pack(8)

class TGATex// : Texture
{

public:

	//todo: optimize the structure layout for size, it's got padding that it doesn't need but that's the way it's defined 

	//TODO: set this up for debuging and make it part of the actual TGA data so i don't have to double deref.
	//char* imageName
	//TGAHeader hdr;

	TGAHeader hdr;
	unsigned int* data;

	TGATex() : data(0)
	{

	}

	~TGATex()
	{
		if (data != nullptr)
		{
			delete[] data;
		}
	}

	TGATex(TGAHeader& _hdr, unsigned int* _dataptr) : hdr(_hdr) , data(_dataptr)
	{
	}

};

class TGALoader
{
public:

	static TGATex* LoadTGA(const char * package, const char * texName)
	{
		//if it fails I need to pull the default texture

		unsigned char * buffer;
		unsigned int bufferSize;
		
		eat(package, ChunkType::TEXTURE_TYPE, texName, buffer, bufferSize);

		unsigned char * cursor = buffer;
		unsigned char hdrbuffer[18];

		TGATex* retTex = nullptr;
		
		//read for header
		TGAHeader hdr;

		memcpy(hdrbuffer, cursor, 18);
		cursor += 18;

		//copy the header over
				unsigned char* cptr = hdrbuffer;
				hdr.identsize = *cptr++;
				hdr.colourmaptype = *cptr++;
				hdr.imagetype = *cptr++;
				short * sptr = (short*)cptr;
				hdr.colourmapstart = *sptr++;
				hdr.colourmaplength = *sptr++;
				cptr = (unsigned char*)sptr;
				hdr.colourmapbits = *cptr++;
				sptr = (short*)cptr;
				hdr.xstart = *sptr++;
				hdr.ystart = *sptr++;
				hdr.width = *sptr++;
				hdr.height = *sptr++;
				cptr = (unsigned char*)sptr;
				hdr.bits = *cptr++;
				hdr.descriptor = *cptr;

				//use header to find length of file its height * width * 3 because it's 3x ints per pixel 
				unsigned int size = hdr.width * hdr.height * (hdr.bits / 8u);
				unsigned int * dataPtr = new unsigned int[size]; //3 for the 24 bit depth means 3 ints per pixel
				
				memcpy(dataPtr, cursor, size);
																									 //get buffer for image data
																									 //TODO:: make sure the cursor on the file
				
				//create texture with all the data
				retTex = new TGATex(hdr, dataPtr);
		
		return retTex;

	}

	//TODO: incorporate with my mem system. add return codes so user doesn't have to check for null
	//TODO: incoproate additonal support for the rest of the TGA features like colour mapping
	static TGATex* LoadTGA(const char* imagePath)
	{
		//if it fails I need to pull the default texture

		File::Handle fh;
		TGATex* retTex = nullptr;
		if (File::Open(fh, imagePath, File::Mode::READ) == File::Error::SUCCESS)
		{
			
			//read for header
			unsigned char buffer[18];
			TGAHeader hdr;
			if (File::Read(fh, buffer, 18) == File::Error::SUCCESS)
			{
				//save header variables to header
				//the compiler is aligning to boundries adding 2 bytes of padding which means this doesn't work....sigh
				//hdr = (TGAHeader&) buffer;

				unsigned char* cptr = buffer;
				hdr.identsize = *cptr++;
				hdr.colourmaptype = *cptr++;
				hdr.imagetype = *cptr++;
				short * sptr = (short*)cptr;
				hdr.colourmapstart = *sptr++;
				hdr.colourmaplength = *sptr++;
				cptr = (unsigned char*)sptr;
				hdr.colourmapbits = *cptr++;
				sptr = (short*)cptr;
				hdr.xstart = *sptr++;
				hdr.ystart = *sptr++;
				hdr.width = *sptr++;
				hdr.height = *sptr++;
				cptr = (unsigned char*)sptr;
				hdr.bits = *cptr++;
				hdr.descriptor = *cptr;


				//unsigned char* cptr = buffer;
				//hdr.identsize = *cptr;
				//cptr++;
				//hdr.colourmaptype = *cptr;
				//cptr++;
				//hdr.imagetype = *cptr;
				//cptr++;
				//short * sptr = (short*)cptr;
				//hdr.colourmapstart = *sptr;
				//sptr++;
				//hdr.colourmaplength = *sptr;
				//sptr++;
				//cptr = (unsigned char*)sptr;
				//hdr.colourmapbits = *cptr;
				//cptr++;
				//sptr = (short*)cptr;
				//hdr.xstart = *sptr;
				//sptr++;
				//hdr.ystart = *sptr;
				//sptr++;
				//hdr.width = *sptr;
				//sptr++;
				//hdr.height = *sptr;
				//sptr++;
				//cptr = (unsigned char*)sptr;
				////Trace::out("image bits per pixel (8,16,24,32) = %i\n", *cptr);
				//hdr.bits = *cptr;
				//cptr++;
				//hdr.descriptor = *cptr;


				////Trace::out("Begin Header Data printf sizeof header = %i\n ", sizeof(TGAHeader));
				////Trace::out("hdr identsize usually 0 = %i\n", *cptr); 
				//hdr.identsize = *cptr;
				//cptr++;
				////Trace::out("Type of colour map 0=none, 1=has pallete = %i\n", *cptr); 
				//hdr.colourmaptype = *cptr;
				//cptr++;
				////Trace::out("image type 0=none, 1= indexed, 2=rgb, 3=grey, +8=rle packed = %i\n", *cptr); 
				//hdr.imagetype = *cptr;
				//cptr++;
				//short * sptr = (short*) cptr;
				////Trace::out("colourmapstart = %i\n", *sptr); 
				//hdr.colourmapstart = *sptr;
				//sptr++;
				////Trace::out("colourmaplength number of coloursin palette = %i\n", *sptr); 
				//hdr.colourmaplength = *sptr;
				//sptr++;
				//cptr = (unsigned char*) sptr;
				////Trace::out("colourmapbits = %i\n", *cptr);
				//hdr.colourmapbits = *cptr;
				//cptr++;
				//sptr = (short*) cptr;
				////Trace::out("xstart = %i\n", *sptr);
				//hdr.xstart = *sptr;
				//sptr++;
				////Trace::out("ystart = %i\n", *sptr);
				//hdr.ystart = *sptr;
				//sptr++;
				////Trace::out("width in pix = %i\n", *sptr); 
				//hdr.width = *sptr;
				//sptr++;
				////Trace::out("height in pix = %i\n", *sptr); 
				//hdr.height = *sptr;
				//sptr++;
				//cptr = (unsigned char*)sptr;
				////Trace::out("image bits per pixel (8,16,24,32) = %i\n", *cptr);
				//hdr.bits = *cptr;
				//cptr++;
				////Trace::out("descriptor (vh flip bits) = %i\n", *cptr);
				//hdr.descriptor = *cptr;
				////Trace::out("End of Header Data\n");
				
				//add read for color map data and/or skip for it

				//use header to find length of file its height * width * 3 because it's 3x ints per pixel ]
				unsigned int size = hdr.width * hdr.height * (hdr.bits / 8u);
				unsigned int * dataPtr = new unsigned int[size]; //3 for the 24 bit depth means 3 ints per pixel

				//get buffer for image data
				//TODO:: make sure the cursor on the file
				if (File::Read(fh, dataPtr, size) == File::Error::SUCCESS)//read the rest of the texture
				{
					//create texture with all the data
					retTex = new TGATex(hdr,dataPtr);
				}
			}

			//clean up file 
			File::Close(fh);

		}

		return retTex;
	}

};

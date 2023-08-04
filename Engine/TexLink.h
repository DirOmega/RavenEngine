#pragma once

#include "DLink.h"
#include "GLibSetup.h"

enum class TextureName
{

	FRIG,
	BuggyDiff,
	BuggyNrm,
	BuggySpec,
	Warbear,
	PolarBear,
	GethT,
	Astroboy,
	STONES,
	RED_BRICK,
	DUCKWEED,
	ROCKS,
	DEFAULT,
	CUBETEST,
	Gray,
	Black,

	Floor,
	ClockB0,
	ClockB2,
	ClockBW,

	Vase,
	VaseBase,
	
	P0,
	P1,
	P2,
	P3,
	P4,
	Roof,
	Wall0,
	Wall1,
	Wall2,
	Wall3,
	WallAcc,
	Sky,
	



	LBlouse,
	LBody,
	LEyeHi0,
	LEyeHi1,
	LFace0,
	LFace1,
	LHair0,
	LHair1,
	LLightAcc0,
	LLightAcc1,
	LOther,
	LSkin,
	LArms0,
	LArms1,

	RBlouse,
	RLegs,
	REyeHi0,
	REyeHi1,
	RFace0,
	RFace1,
	RHair0,
	RHair1,
	RLightAcc0,
	RLightAcc1,
	ROther,
	RSkin,
	RArms0,
	RArms1,


	FHairBlue,
	FHairPink,
	FClothColor,
	FBody,
	FFace,
	FEye,
	FFacialExpr,
	FUndies,
	FFaceTransparent,

	NOT_INITIALIZED

	

};

#define TEXTURE_ASSET_NAME_SIZE 64

class TexLink : public DLink
{

public:

	TexLink();
	virtual ~TexLink();
	TexLink(const TexLink&) = delete;
	TexLink(const TexLink&&) = delete;
	TexLink operator = (const TexLink&) = delete;
	TexLink operator = (const TexLink&&) = delete;

	void dClean() override;

	
	void set(const char * const _assetName,
		TextureName _name,
		GLuint _TextureID,
		GLenum minFilter,
		GLenum magFilter,
		GLenum wrapMode);


private:
	char assetName[TEXTURE_ASSET_NAME_SIZE];

public:

	TextureName name;
	GLuint textureID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapMode;


};
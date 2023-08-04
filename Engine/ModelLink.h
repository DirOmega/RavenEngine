#pragma once

#include "DLink.h"
#include "Model.h"

enum class ModelName
{
	NOT_INITIALIZED,
	Hack,
	NullMod,
	BSphere,
	Cube,
	DebugCube,
	Pryamid,
	Diamond,
	NoIdea,
	Astroboy,
	Buggy,
	Frig,
	WarBear,
	Geth,
	Chopper,
	Treadmill,

	Round,
	Line,
	Floor,
	Roof,
	ClockBack0,
	ClockBack2,
	ClockBW,
	ClockFrame,
	ClockMark,
	Glass,
	Glow0,
	Glow2,
	LampFrame,
	LampG,
	LampStand,
	LFix,
	LFixG,
	LFix2Glow,
	MetalArt,
	P0,P1,P2,P3,P4,
	PlasterAcc,
	Sconces,
	Shelf,
	Vase,
	Wall0,
	Wall1,
	Wall2,
	Wall3,
	WallSconceG,
	WindowFrame,
	VaseBase,
	Sky,


	//SIOL hair shadow is not included cuz i don't have SSAO yet
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

	//SIOR
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

	//FRIEND
	FAntenna,
	FHairBlue,
	FHairPink,
	FBangs,
	FBHair,
	FBWClothes,
	FBlueClothes,
	FBody,
	FBrows,
	FBWAcc,
	FCheeks,
	FEye,
	FEyeHi,
	FFace0,
	FFace1,
	FFaceExpr,
	FLashes,
	FStrands,
	FPAcc,
	FUSkirt,
	FUndies

};

class ModelLink : public DLink
{

public:

	ModelLink();
	virtual ~ModelLink();
	ModelLink(const ModelLink&) = delete;
	ModelLink(const ModelLink&&) = delete;
	ModelLink operator = (const ModelLink&) = delete;
	ModelLink operator = (const ModelLink&&) = delete;

	void dClean() override;
	void set(ModelName _name, Model* mdl);
	
	void setName(ModelName _name);
	Model* getModel();
	
	
	ModelName name;

	private:
		Model* mdl;

};
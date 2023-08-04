#pragma once

struct CamVals : Align16
{
	float distance;
	float px;
	float py;
	float pz;
	float rx;
	float ry;
	float rz;
	char interpData[24];
	float fov;
};
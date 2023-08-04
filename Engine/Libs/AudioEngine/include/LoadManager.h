//#pragma once
//#include "CircularData.h"
//
//class LoadManager
//{
//public:
//	// Defaults
//	LoadManager(const LoadManager &) = delete;
//	const LoadManager &operator = (const LoadManager &) = delete;
//	~LoadManager() = delete;
//
//	// Public methods:
//	static void Create();
//	static int Update();
//	static void SendData(int val);
//
//	static CircularData *GetG2LBuff();
//	static CircularData *GetL2GBuff();
//
//private:
//	static LoadManager *privGetInstance();
//	static void privCreate();
//
//	LoadManager();
//
//	CircularData *pG2LBuff;  // Game to Audio
//	CircularData *pL2GBuff; // Audio to Game
//
//	static LoadManager *pSndMan;
//};
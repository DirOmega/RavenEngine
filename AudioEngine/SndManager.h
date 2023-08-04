#ifndef SND_MANAGER_H
#define SND_MANAGER_H

#include "CircularData.h"

class SndManager
{
public:
	// Defaults
	SndManager(const SndManager &) = delete;
	const SndManager &operator = (const SndManager &) = delete;
	~SndManager() = delete;

	// Public methods:
	static void Create();
	static void Update();
	static void SendData(int val);

	static CircularData *GetG2ABuff();
	static CircularData *GetA2GBuff();

private:
	static SndManager *privGetInstance();
	static void privCreate();

	SndManager();

	CircularData *pG2ABuff;  // Game to Audio
	CircularData *pA2GBuff; // Audio to Game

	static SndManager *pSndMan;
};
#endif

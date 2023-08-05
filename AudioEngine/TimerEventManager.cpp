#include "TimerEventManager.h"

unsigned int TimerManager::currTime = 0;
TimerManager* TimerManager::pInstance;
TimeEvent* TimerManager::poRefTE;
Command* TimerManager::pNullCMD;

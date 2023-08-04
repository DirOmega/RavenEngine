#ifndef SND_H
#define SND_H

#include "HandleMan.h"
#include "SndCallRegistry.h"

class Snd
{
public:
	// defaults
	Snd() = delete;
	Snd(const Snd &);
	const Snd &operator=(const Snd &);
	~Snd();

	// public
	Snd(SndId snd);

	HandleStatus Play();
	HandleStatus Load();
	//	HandleStatus Stop();

private:
	SndId	sndID;
	Handle  handle;
};

#endif

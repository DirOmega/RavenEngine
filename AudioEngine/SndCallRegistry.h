#ifndef SND_CALL_REGISTRY_H
#define SND_CALL_REGISTRY_H

enum class SndId
{

	//DUMMY_ID = 0x9990000,
	Undef,

	//demo1
	D1Start,
	Fiddle = 101,
	Bassoon = 102,
	Calliope = 103,
	SongA = 104,
	SongB = 105,

	//demo2
	D2Start = 200,
	Coma = 201,

	//demo3
	D3Start = 300,
	seinfeld = 301,

	//demo4
	D4Start = 400,
	Dial = 401,
	MoonPatrol = 402,
	Sequence = 403,
	Donkey = 404,

	//demo5
	D5Start = 500,
	Electro = 501,
	Alarm = 502,
	Beethoven = 503,

	STRINGS_ID,
	LRMONO_ID,
	FIDDLE_ID,
	Heli,
	HeliDemo,
	Applause,
	SeinfieldDemo,
	SLoopA,
	SLoopB,
	SLoopC,
	STranAtoB,
	STranBtoC,
	STranCtoA,
	SIntro,
	SEnd
};

#endif

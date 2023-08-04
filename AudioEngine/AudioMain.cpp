//#include <thread>
#include <assert.h>

#include "Trace.h"
#include "File.h"

#include "SndCallRegistry.h"

#include "XAudio2Wrapper.h"
#include "XAudio2Chunk.h"

#include "SourceVoice.h"
#include "VoiceCallback.h"
#include "AudioMain.h"

#include "AudioBufferManager.h"
#include "TimerEventManager.h"
#include "SourceVoiceTopManager.h"
#include "PriorityManager.h"
#include "Actor.h"

#include "GameCallback.h"

#include "RVNTime.h"
#include "RVNTimer.h"


//I want to push all this and hte mastering voice out of this file.
IXAudio2				*pXAudio2 = 0;
IXAudio2MasteringVoice	*pMasterVoice = 0;

CircularData* pOut;

// Horrible hack - should be part of the buffer/voice
//WAVEFORMATEX wfx = { 0 };
//XAUDIO2_BUFFER    buffer = { 0 };

//void InitializeXAudio2()
//{
//	// -----------------------------------------------------------------
//	// Audio Engine Setup
//	// -----------------------------------------------------------------
//
//	HRESULT	hr;
//
//	// Needed for COM initialization
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
//
//	// -------------------------------------------------------
//	// Create an instance, use its default processor
//	// -------------------------------------------------------
//	hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
//	assert(hr == S_OK);
//	assert(pXAudio2);
//
//	// -------------------------------------------------------
//	// Creating a master voice, with default settings:
//	//
//	//      InputChannels = XAUDIO2_DEFAULT_CHANNELS,
//	//      InputSampleRate = XAUDIO2_DEFAULT_SAMPLERATE,
//	//      Flags = 0,
//	//      szDeviceId = NULL,
//	//      *pEffectChain = NULL,
//	//      StreamCategory = AudioCategory_GameEffects
//	// -------------------------------------------------------
//	hr = pXAudio2->CreateMasteringVoice(&pMasterVoice);
//	assert(hr == S_OK);
//}

void LoadBuffer(const char *pFileName, WAVEFORMATEX &wfx, XAUDIO2_BUFFER &buffer)
{
	// -----------------------------------------------------------------
	// Audio: Load Files
	//------------------------------------------------------------------

	HANDLE FileHandle;
	File::Error ferror;

	// Open file

	// "Bass.wav";
	// "LRMonoPhase4.wav";
	const char *srcFile = pFileName;

	ferror = File::Open(FileHandle, srcFile, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	// Set file to beginning
	ferror = File::Seek(FileHandle, File::Location::BEGIN, 0);
	assert(ferror == File::Error::SUCCESS);

	// -------------------------------------------------------
	// Audio Format - wfx
	// Audio Buffer - how to play it back
	// -------------------------------------------------------
	//WAVEFORMATEX wfx = { 0 };
	//XAUDIO2_BUFFER buffer = { 0 };

	wfx = { 0 };
	buffer = { 0 };


	// -------------------------------------------------------
	// Find and load specific Chunks
	// -------------------------------------------------------

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	DWORD filetype;
	ChunkError cerror;

	// Scan to the RIFF and load file type
	cerror = FindChunk2(FileHandle, fourccRIFF, dwChunkSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);
	cerror = ReadChunkData2(FileHandle, &filetype, sizeof(DWORD), dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	// Make sure its not in the compressed format, WAVE format is uncompressed
	if (filetype != fourccWAVE)
	{
		assert(false);
	}

	// Read the FMT: format
	cerror = FindChunk2(FileHandle, fourccFMT, dwChunkSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);
	cerror = ReadChunkData2(FileHandle, &wfx, dwChunkSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	// Scan to the DATA chunk, read the size, allocate buffer of that size
	cerror = FindChunk2(FileHandle, fourccDATA, dwChunkSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	assert(pDataBuffer != 0);

	// Fill the data...
	cerror = ReadChunkData2(FileHandle, pDataBuffer, dwChunkSize, dwChunkPosition);
	assert(cerror == ChunkError::CHUNK_SUCCESS);

	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	buffer.LoopCount = 0;				  // looping...

	ferror = File::Close(FileHandle);
	assert(ferror == File::Error::SUCCESS);
}


//void CallSound(CircularData *pOut, WAVEFORMATEX &wfx, XAUDIO2_BUFFER &buffer)
//{
//	// -------------------------------------------------------
//	// Create a source voice
//	//		Format in wfx
//	// -------------------------------------------------------
//	HRESULT	hr;
//
//	IXAudio2SourceVoice* pSourceVoice;
//	VoiceCallback *pCallback = new VoiceCallback();
//
//	hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx, 0, XAUDIO2_MAX_FREQ_RATIO, pCallback);
//	assert(hr == S_OK);
//
//	// -------------------------------------------------------
//	// Create a move the voice onto the voice queue
//	//		Only files on the queue can be played
//	//		There is no deep copy, we still hold the buffer while its active
//	// -------------------------------------------------------
//
//	hr = pSourceVoice->SubmitSourceBuffer(&buffer);
//	assert(hr == S_OK);
//
//	// -------------------------------------------------------
//	// Start the source voice
//	// -------------------------------------------------------
//
//	hr = pSourceVoice->Start(0);
//	assert(hr == S_OK);
//
//
//}
//Integration from MS1 below
//
//void updateLoop()
//{
//	Timer timer;
//	Time elapsedTime;
//	TimerManager* tm = TimerManager::getInstance();
//
//	int timeInMs = 0;
//	volatile int c = 0;
//	while (tm->FlagQuit != true)
//	{
//		// Mark our begin time.
//		timer.tic();
//
//		//this just lets me update the audio as fast as possibble which I'm not sure is great.
//		c = 0;
//
//		for (int i = 0; i<500000; i++)
//		{
//			c += 1;
//		}
//
//		elapsedTime = timer.toc();
//
//		//tm->Update(timeInMs+1);
//		tm->Update(timeInMs);
//		//Time elapsedTime = timer.toc();
//		elapsedTime = timer.toc();
//		timeInMs = Time::quotient(elapsedTime, Time(TIME_ONE_MILLISECOND));
//		//	tm->Update(timeInMs);
//	}
//	//this just allows multiple demo's to use the same update loop code. in sucession
//	tm->resetQuitFlag();
//}

//
//void loadAudio()
//{
//
//	AudioBufferManager::Add();
//
//
//	AudioBufferManager::Add("heli.wav", SndId::Heli);
//	AudioBufferManager::Add("applause.wav", SndId::Applause);
//	AudioBufferManager::Add(".\\Seinples\\Loops\\A.wav", SndId::SLoopA);
//	AudioBufferManager::Add(".\\Seinples\\Loops\\B.wav", SndId::SLoopB);
//	AudioBufferManager::Add(".\\Seinples\\Loops\\C.wav", SndId::SLoopC);
//
//	AudioBufferManager::Add(".\\Seinples\\Transitions\\AtoB.wav", SndId::STranAtoB);
//	AudioBufferManager::Add(".\\Seinples\\Transitions\\BtoC.wav", SndId::STranBtoC);
//	AudioBufferManager::Add(".\\Seinples\\Transitions\\CtoA.wav", SndId::STranCtoA);
//	AudioBufferManager::Add(".\\Seinples\\Transitions\\Intro.wav",SndId::SIntro);
//	AudioBufferManager::Add(".\\Seinples\\Transitions\\End.wav",  SndId::SEnd);
//
//}

Script* makeSineLoopScript()
{
	Script* myScript = new Script();

	myScript->addLine(*AudioBufferManager::Find(SndId::SLoopA), "Sienfeld Loop A");

	myScript->addLine(*AudioBufferManager::Find(SndId::SLoopC), "Sienfeld Loob C");
	//	myScript->addLine(*DuffMan->Find(BufferID::AtoB));
	myScript->addLine(*AudioBufferManager::Find(SndId::SLoopB), "Sienfeld Loop B");
	//	myScript->addLine(*DuffMan->Find(BufferID::BtoC));
	myScript->addLine(*AudioBufferManager::Find(SndId::SLoopA), "Sienfeld Loop A");

	return myScript;
}

Script* makeSineTransitionScript()
{

	Script* myScript = new Script();


	myScript->addLine(*AudioBufferManager::Find(SndId::SEnd), "Sienfeld End");

	myScript->addLine(*AudioBufferManager::Find(SndId::STranCtoA), "Sienfeld Transition C to A");

	myScript->addLine(*AudioBufferManager::Find(SndId::STranBtoC), "Sienfeld Transition B to C");

	myScript->addLine(*AudioBufferManager::Find(SndId::STranAtoB), "Sienfeld Transition A to B");

	myScript->addLine(*AudioBufferManager::Find(SndId::SIntro), "Sienfeld Intro");

	return myScript;
}

void Demo1A()
{
	AudioDataBuffer* Fiddle;
	AudioDataBuffer* Bassoon;

	SourceVoice* SV1;
	SourceVoice* SV2;
	SourceVoice* SV3;

	Fiddle = AudioBufferManager::Find(SndId::Fiddle);
	Bassoon = AudioBufferManager::Find(SndId::Bassoon);

	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV1);
	SV1->setpOut(pOut);
	SV1->SubmitAudio(*Fiddle);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV2);
	SV2->setpOut(pOut);
	SV2->SubmitAudio(*Fiddle);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV3);
	SV3->setpOut(pOut);
	SV3->SubmitAudio(*Bassoon);

	TriggerVoiceCMDwPan* cmd;
	TriggerVoiceCMD* TVCMD = new TriggerVoiceCMD(SV1);
	TimerManager::Add(TimeEventID::PlayAndKeep, *TVCMD, 0);
	cmd = new TriggerVoiceCMDwPan(SV2, -1.0f);
	TimerManager::Add(TimeEventID::PlayAndPan, *cmd, 3000);
	cmd = new TriggerVoiceCMDwPan(SV3, 1.0f);
	TimerManager::Add(TimeEventID::PlayAndPan, *cmd, 6000);


	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(SV1);
	SV1->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(SV2);
	SV2->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(SV3);
	SV3->setStreamEndCMD(*cleanup);


}

void Demo1B()
{

	AudioDataBuffer* Bassoon;
	SourceVoice* SV1;
	SourceVoice* SV2;
	Bassoon = AudioBufferManager::Find(SndId::Bassoon);

	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV1);
	SV1->setpOut(pOut);
	SV1->SubmitAudio(*Bassoon);

	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV2);
	SV2->setpOut(pOut);
	SV2->SubmitAudio(*Bassoon);

	PlayPanCMD* cmd;

	cmd = new PlayPanCMD(SV1, -1.0f, 1.0f, 2000);
	TimerManager::Add(TimeEventID::PlayAndPan, *cmd, 10000);
	cmd = new PlayPanCMD(SV2, 1.0f, -1.0f, 2000);
	TimerManager::Add(TimeEventID::PlayAndPan, *cmd, 15000);

	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(SV1);
	SV1->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(SV2);
	SV2->setStreamEndCMD(*cleanup);

}

void Demo1C()
{
	AudioDataBuffer* buff;
	buff = AudioBufferManager::Find(SndId::Calliope);

	SourceVoice* SV1;
	SourceVoice* SV2;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV1);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SV2);
	SV1->setpOut(pOut);
	SV1->SubmitAudio(*buff);
	SV2->setpOut(pOut);
	SV2->SubmitAudio(*buff);

	PlayVolSlideCMD* cmd;

	cmd = new PlayVolSlideCMD(SV1, 0.0f, 1.0f, 1000);

	TimerManager::Add(TimeEventID::PlayAndVol, *cmd, 20000);

	cmd = new PlayVolSlideCMD(SV2, 1.0f, 0.0f, 1000);

	TimerManager::Add(TimeEventID::PlayAndVol, *cmd, 25000);

	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(SV1);
	SV1->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(SV2);
	SV2->setStreamEndCMD(*cleanup);

}

void Demo1D()
{
	AudioDataBuffer* buff104;
	AudioDataBuffer* buff105;
	buff104 = AudioBufferManager::Find(SndId::SongA);
	buff105 = AudioBufferManager::Find(SndId::SongB);

	SourceVoice* sv1;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, sv1);
	sv1->setpOut(pOut);
	sv1->SubmitAudio(*buff104);
	SourceVoice* sv2;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, sv2);
	sv2->setpOut(pOut);
	sv2->SubmitAudio(*buff104);

	TriggerVoiceCMDwPan* cmd = new TriggerVoiceCMDwPan(sv1, -1.0f);

	TimerManager::Add(TimeEventID::PlayAndKeep, *cmd, 30000);

	cmd = new TriggerVoiceCMDwPan(sv2, 1.0f);

	TimerManager::Add(TimeEventID::PlayAndKeep, *cmd, 30000);

	getPlayTimeCMD* ptCMD = new getPlayTimeCMD(sv1);

	TimerManager::Add(TimeEventID::PrintPlayTime, *ptCMD, 35000);

	ptCMD = new getPlayTimeCMD(sv1);

	TimerManager::Add(TimeEventID::PrintPlayTime, *ptCMD, 38000);

	StopVoiceAndReleaseCMD* stpCMD = new StopVoiceAndReleaseCMD(sv1);

	TimerManager::Add(TimeEventID::StopVoice, *stpCMD, 60000);

	stpCMD = new StopVoiceAndReleaseCMD(sv2);

	TimerManager::Add(TimeEventID::StopVoice, *stpCMD, 70000);

	ptCMD = new getPlayTimeCMD(sv2);

	TimerManager::Add(TimeEventID::PrintPlayTime, *ptCMD, 72000);

	//ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(sv1);
	//sv1->setStreamEndCMD(*cleanup);
	//cleanup = new ReleaseVoiceCMD(sv2);
	//sv2->setStreamEndCMD(*cleanup);

}

void Demo1E()
{
	AudioDataBuffer* buff102;
	buff102 = AudioBufferManager::Find(SndId::Bassoon);

	SourceVoice* A;
	SourceVoice* B;
	SourceVoice* C;
	SourceVoice* D;

	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, A);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, B);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, C);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, D);

	A->setpOut(pOut);
	A->SubmitAudio(*buff102);
	B->setpOut(pOut);
	B->SubmitAudio(*buff102);
	C->setpOut(pOut);
	C->SubmitAudio(*buff102);
	D->setpOut(pOut);
	D->SubmitAudio(*buff102);

	TriggerVoiceCMD* cmd = new TriggerVoiceCMD(A);

	TimerManager::Add(TimeEventID::TriggerVoice, *cmd, 80000);
	cmd = new TriggerVoiceCMD(B);
	TimerManager::Add(TimeEventID::TriggerVoice, *cmd, 80500);
	cmd = new TriggerVoiceCMD(C);
	TimerManager::Add(TimeEventID::TriggerVoice, *cmd, 81000);
	cmd = new TriggerVoiceCMD(D);
	TimerManager::Add(TimeEventID::TriggerVoice, *cmd, 81500);

	StopVoiceAndReleaseCMD* stpcmd = new StopVoiceAndReleaseCMD(A);
	TimerManager::Add(TimeEventID::StopVoice, *stpcmd, 81500);
	stpcmd = new StopVoiceAndReleaseCMD(B);
	TimerManager::Add(TimeEventID::StopVoice, *stpcmd, 81500);
	stpcmd = new StopVoiceAndReleaseCMD(C);
	TimerManager::Add(TimeEventID::StopVoice, *stpcmd, 81500);


	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(D);
	D->setStreamEndCMD(*cleanup);
}

void Demo1()
{
	Demo1A();
	Demo1B();
	Demo1C();
	Demo1D();
	Demo1E();
}

void Demo2()
{
	AudioDataBuffer* buff201;
	buff201 = AudioBufferManager::Find(SndId::Coma);

	SourceVoice* A;
	SourceVoice* B;
	SourceVoice* C;
	SourceVoice* D;
	SourceVoice* E;
	SourceVoice* F;
	SourceVoice* G;
	SourceVoice* H;
	SourceVoice* I;
	SourceVoice* J;
	SourceVoice* K;

	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, A);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, B);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, C);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, D);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, E);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, F);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, G);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, H);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, I);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, J);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, K);

	A->setpOut(pOut);
	B->setpOut(pOut);
	C->setpOut(pOut);
	D->setpOut(pOut);
	E->setpOut(pOut);
	F->setpOut(pOut);
	G->setpOut(pOut);
	H->setpOut(pOut);
	I->setpOut(pOut);
	J->setpOut(pOut);
	K->setpOut(pOut);

	A->SubmitAudio(*buff201);
	B->SubmitAudio(*buff201);
	C->SubmitAudio(*buff201);
	D->SubmitAudio(*buff201);
	E->SubmitAudio(*buff201);
	F->SubmitAudio(*buff201);
	G->SubmitAudio(*buff201);
	H->SubmitAudio(*buff201);
	I->SubmitAudio(*buff201);
	J->SubmitAudio(*buff201);
	K->SubmitAudio(*buff201);

	A->setVolume(0.1f);
	B->setVolume(0.1f);
	C->setVolume(0.1f);
	D->setVolume(0.1f);
	E->setVolume(0.1f);
	F->setVolume(0.1f);
	G->setVolume(0.1f);
	H->setVolume(0.1f);
	I->setVolume(0.1f);
	J->setVolume(0.1f);
	K->setVolume(0.1f);

	A->setPriority(10);
	B->setPriority(50);
	C->setPriority(150);
	D->setPriority(50);
	E->setPriority(75);
	F->setPriority(100);
	G->setPriority(150);
	H->setPriority(75);
	I->setPriority(75);
	J->setPriority(75);
	K->setPriority(150);


	//I know this is horribble but it makes life so much easier.
	TriggerVoiceCMDpt* acmd = new TriggerVoiceCMDpt(A);
	TriggerVoiceCMDpt* bcmd = new TriggerVoiceCMDpt(B);
	TriggerVoiceCMDpt* ccmd = new TriggerVoiceCMDpt(C);
	TriggerVoiceCMDpt* dcmd = new TriggerVoiceCMDpt(D);
	TriggerVoiceCMDpt* ecmd = new TriggerVoiceCMDpt(E);
	TriggerVoiceCMDpt* fcmd = new TriggerVoiceCMDpt(F);
	TriggerVoiceCMDpt* gcmd = new TriggerVoiceCMDpt(G);
	TriggerVoiceCMDpt* hcmd = new TriggerVoiceCMDpt(H);
	TriggerVoiceCMDpt* icmd = new TriggerVoiceCMDpt(I);
	TriggerVoiceCMDpt* jcmd = new TriggerVoiceCMDpt(J);
	TriggerVoiceCMDpt* kcmd = new TriggerVoiceCMDpt(K);

	TimerManager::Add(TimeEventID::PlayAndKeep, *acmd, 0);
	TimerManager::Add(TimeEventID::PlayAndKeep, *bcmd, 0);
	TimerManager::Add(TimeEventID::PlayAndKeep, *ccmd, 0);
	TimerManager::Add(TimeEventID::PlayAndKeep, *dcmd, 1000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *ecmd, 2000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *fcmd, 3000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *gcmd, 4000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *hcmd, 5000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *icmd, 6000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *jcmd, 7000);
	TimerManager::Add(TimeEventID::PlayAndKeep, *kcmd, 8000);



	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(A);
	A->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(B);
	B->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(C);
	C->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(D);
	D->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(E);
	E->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(F);
	F->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(G);
	G->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(H);
	H->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(I);
	I->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(J);
	J->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(K);
	K->setStreamEndCMD(*cleanup);
}


void Demo3()
{
	//load scripts
	//Script* narratorLines = makeNarrationScript();
	Script* SineLoopScript = makeSineLoopScript();
	Script* SineTransScript = makeSineTransitionScript();

	Actor* SineLoop = new Actor();
	Actor* SineTransition = new Actor();

	SineLoop->setPartner(*SineTransition);
	SineTransition->setPartner(*SineLoop);

	SineLoop->giveScript(SineLoopScript);
	SineTransition->giveScript(SineTransScript);

	SineLoop->fromTheTop();
	SineTransition->fromTheTop();

	LineEndCMD* SLLE = new LineEndCMD(SineLoop);
	LineEndCMD* STLE = new LineEndCMD(SineTransition);

	SineLoop->giveVoice(VoiceTypeID::mono32b48000, SLLE, pOut);
	SineTransition->giveVoice(VoiceTypeID::mono32b48000, STLE, pOut);
	SineLoop->setVolume(.8f);
	SineTransition->setVolume(.8f);

	//Command* exit = new CMDQuit();
	Command* triggerVoice = new TriggerNextActorCmd(SineTransition);

	TimerManager::Add(TimeEventID::PlayAndKeep, *triggerVoice, 0);
	//TimerManager::Add(TimeEventID::Quit, *exit, 25000);

	//updateLoop();

	Command* deleteActor = new CleanupActorCMD(SineLoop);
	TimerManager::Add(TimeEventID::undef, *deleteActor, 66000);
	deleteActor = new CleanupActorCMD(SineTransition);
	TimerManager::Add(TimeEventID::undef, *deleteActor, 66000);

}

void Demo4()
{
	AudioDataBuffer* Dial;
	AudioDataBuffer* MoonPatrol;
	AudioDataBuffer* Sequence;
	AudioDataBuffer* Donkey;

	Dial = AudioBufferManager::Find(SndId::Dial);
	MoonPatrol = AudioBufferManager::Find(SndId::MoonPatrol);
	Sequence = AudioBufferManager::Find(SndId::Sequence);
	Donkey = AudioBufferManager::Find(SndId::Donkey);

	SourceVoice* DialVoice;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, DialVoice);
	DialVoice->setpOut(pOut);

	SourceVoice* MoonVoice;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, MoonVoice);
	MoonVoice->setpOut(pOut);

	SourceVoice* SequenceVoice;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, SequenceVoice);
	SequenceVoice->setpOut(pOut);

	SourceVoice* DonkeyVoice;
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, DonkeyVoice);
	DonkeyVoice->setpOut(pOut);

	demo4CMD* cmd = new demo4CMD(DialVoice, "Dial_mono.wav");
	DialVoice->setStreamEndCMD(*cmd);
	Dial->setStreamEnd();
	DialVoice->SubmitAudio(*Dial);

	cmd = new demo4CMD(MoonVoice, "MoonPatrol_mono.wav");
	MoonVoice->setStreamEndCMD(*cmd);
	MoonPatrol->setStreamEnd();
	MoonVoice->SubmitAudio(*MoonPatrol);

	cmd = new demo4CMD(SequenceVoice, "Sequence_mono.wav");
	SequenceVoice->setStreamEndCMD(*cmd);
	Sequence->setStreamEnd();
	SequenceVoice->SubmitAudio(*Sequence);

	cmd = new demo4CMD(DonkeyVoice, "Donkey_mono.wav");
	DonkeyVoice->setStreamEndCMD(*cmd);
	Donkey->setStreamEnd();
	DonkeyVoice->SubmitAudio(*Donkey);

	TriggerVoiceCMDwPan* tcmd;
	tcmd = new TriggerVoiceCMDwPan(DialVoice, -1.0f);
	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 0);

	tcmd = new TriggerVoiceCMDwPan(MoonVoice, 1.0f);
	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 0);

	tcmd = new TriggerVoiceCMDwPan(SequenceVoice, -1.0f);
	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 0);

	tcmd = new TriggerVoiceCMDwPan(DonkeyVoice, -1.0f);
	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 3500);

	//Demo4CMD returns the source voice
}

void Demo5()
{
	AudioDataBuffer* Alarm;
	AudioDataBuffer* Electro;

	SourceVoice* EVoice;
	SourceVoice* AVoice;
	SourceVoice* BVoice;

	SourceVoiceManager::getSourceVoice(VoiceTypeID::stereo32b48000, BVoice);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, EVoice);
	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono32b48000, AVoice);

	Electro = AudioBufferManager::Find(SndId::Electro);
	Alarm = AudioBufferManager::Find(SndId::Alarm);
	//no beethoven

	EVoice->setpOut(pOut);
	EVoice->SubmitAudio(*Electro);

	AVoice->setpOut(pOut);
	AVoice->SubmitAudio(*Alarm);

	TriggerVoiceCMDwPan* tcmd = new TriggerVoiceCMDwPan(EVoice, -1.0f);

	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 0);

	tcmd = new TriggerVoiceCMDwPan(AVoice, -1.0f);

	TimerManager::Add(TimeEventID::PlayAndKeep, *tcmd, 0);

	demo5CMD* dcmd = new demo5CMD(BVoice, SndId::Beethoven);

	TimerManager::Add(TimeEventID::User, *dcmd, 43000);

	StopVoiceAndReleaseCMD* scmd = new StopVoiceAndReleaseCMD(BVoice);

	TimerManager::Add(TimeEventID::StopVoice, *scmd, 55000);


	ReleaseVoiceCMD* cleanup = new ReleaseVoiceCMD(EVoice);
	EVoice->setStreamEndCMD(*cleanup);
	cleanup = new ReleaseVoiceCMD(AVoice);
	AVoice->setStreamEndCMD(*cleanup);

	//added cleanup to the stop voice cmd type used here

}

//void panningHeliDemo()
//{
//
//	AudioDataBuffer* heli1;
//	AudioDataBuffer* heli2;
//	AudioDataBuffer* applause;
//
//	SourceVoice* StereoVoice;
//	SourceVoice* SV1;
//	SourceVoice* SV2;
//	SourceVoice* SV3;
//	SourceVoice* SV4;
//	SourceVoice* SV5;
//
//	//panning heli voices
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono16b44100, SV1);
//	SV1->setpOut(pOut);
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono16b44100, SV2);
//	SV2->setpOut(pOut);
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono16b44100, SV3);
//	SV3->setpOut(pOut);
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono16b44100, SV4);
//	SV4->setpOut(pOut);
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::mono16b44100, SV5);
//	SV5->setpOut(pOut);
//	SourceVoiceManager::getSourceVoice(VoiceTypeID::stereo16b44100, StereoVoice);
//	StereoVoice->setpOut(pOut);
//
//	//get audio data for the demo
//	heli1 = AudioBufferManager::Find(SndId::Heli);
//	heli2 = AudioBufferManager::Find(SndId::Heli);
//	applause = AudioBufferManager::Find(SndId::Applause);
//
//	//I don't know if this is the behavior I want but since heli1 and heli2 both point
//	//to the same underlying buffer the follow command applies to all using the buffer
//	//but if I have a streaming audio buffer the flag would be important to know when
//	//the stream ends.
//	//I might want to make all resident buffers have stream end enabled by default.
//	//still working on figuring out a good interface for everything.
//	heli1->setStreamEnd();
//	applause->setStreamEnd();
//
//	//souce voice settings
//	SV1->SubmitAudio(*heli1);
//	SV1->pan2Ch(-1.0f);
//	SV2->SubmitAudio(*heli2);
//	SV2->pan2Ch(-.5f);
//	SV3->SubmitAudio(*heli1);
//	SV3->pan2Ch(0.0f);
//	SV4->SubmitAudio(*heli1);
//	SV4->pan2Ch(.5f);
//	SV5->SubmitAudio(*heli1);
//	SV5->pan2Ch(1.0f);
//	StereoVoice->SubmitAudio(*applause);
//
//	TriggerVoiceCMD* SV1Trigger = new TriggerVoiceCMD(SV1);
//	TriggerVoiceCMD* SV2cbTrigger = new TriggerVoiceCMD(SV2);
//	TriggerVoiceCMD* SV3cbTrigger = new TriggerVoiceCMD(SV3);
//	TriggerVoiceCMD* SV4cbTrigger = new TriggerVoiceCMD(SV4);
//	TriggerVoiceCMD* SV5cbTrigger = new TriggerVoiceCMD(SV5);
//
//	//StopVoiceCMD* ApplStopTrigger = new StopVoiceCMD(StereoVoice);
//	TriggerVoiceCMD* ApplStartTrigger = new TriggerVoiceCMD(StereoVoice);
//
//	//change for narration to trigger after first line finishes.
//	//will autoStart when the update loop starts and not before.
//	TimerManager::Add(TimeEventID::TriggerVoice, *SV1Trigger, 10000u);
//	//	tm->Add(TimeEventID::StopVoice, *ApplStopTrigger, 12000);
//	//tm->Add(TimeEventID::TriggerVoice, *ApplStartTrigger, 10000);
//
//	//thesse commands are not fired by the timer but by the callbacks, I have it setup so
//	//you can load and swap commands into the callback context of a source voice
//	//so you can reuse a voice and change it's commands even though the callback context
//	//in engine cant be changed.
//	SV1->setStreamEndCMD(*SV2cbTrigger);
//	SV2->setStreamEndCMD(*SV3cbTrigger);
//	SV3->setStreamEndCMD(*SV4cbTrigger);
//	SV4->setStreamEndCMD(*SV5cbTrigger);
//	SV5->setStreamEndCMD(*ApplStartTrigger);
//
//	Command* exit = new CMDQuit();
//	StereoVoice->setStreamEndCMD(*exit);
//
//	//updateLoop();
//	//The problem with using the system like so to stitch voices is the voices are not properly ended
//	//unless I switch the command to add a pointer for a voice to stop, on stream end doesn't stop a voice
//	//so I made actors that wrap source voices and sets of buffers.
//}



//Integration from MS1 above
void AudioMain(CircularData *pIn, CircularData *_pOut)
{
	pIn;
	pOut = _pOut;

	/*WAVEFORMATEX wfx0;
	XAUDIO2_BUFFER    buffer0;
	WAVEFORMATEX wfx1;
	XAUDIO2_BUFFER    buffer1;
	WAVEFORMATEX wfx2;
	XAUDIO2_BUFFER    buffer2;*/

	//InitializeXAudio2();
	pXAudio2 = AudioEngine::getInstance()->pXAudio2;

	/*LoadBuffer("LRMonoPhase4.Wav", wfx0, buffer0);
	LoadBuffer("Strings.Wav", wfx1, buffer1);
	LoadBuffer("Fiddle.Wav", wfx2, buffer2);*/

	//loadAudio();
	//panningHeliDemo();

	Timer timer;
	Time elapsedTime;
	TimerManager* tm = TimerManager::getInstance();

	int timeInMs = 0;
	//	volatile int c = 0;

	//while (1)
	while (tm->FlagQuit != true)
	{
		timer.tic();
		int val;

		if (pIn->PopFront(val) == true)
		{
			Trace::out("A:%x \n", val);

			if (val == (int)SndId::D1Start)
			{
				Demo1();
			}
			if (val == (int)SndId::D2Start)
			{
				Demo2();
			}

			if (val == (int)SndId::D3Start)
			{
				Demo3();
			}

			if (val == (int)SndId::D4Start)
			{
				Demo4();
			}

			if (val == (int)SndId::D5Start)
			{
				Demo5();
				//Demo1();
			}

			val = 0;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		tm->Update(timeInMs);
		elapsedTime = timer.toc();
		timeInMs = Time::quotient(elapsedTime, Time(TIME_ONE_MILLISECOND));

	}

}

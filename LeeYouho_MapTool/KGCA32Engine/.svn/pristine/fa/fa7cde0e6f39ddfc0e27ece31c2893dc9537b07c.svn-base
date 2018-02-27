#include "BSound.h"

bool BSound::Init()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	//m_pSystem->createSound("../../data/TitleMusic.mp3",FMOD_DEFAULT, 0, &m_pSound[TitleBG]);
	//m_pSystem->createSound("../../data/MenuSound.ogg", FMOD_DEFAULT, 0, &m_pSound[MenuUp]);
	//m_pSystem->createSound("../../data/MenuSelectSound.mp3", FMOD_DEFAULT, 0, &m_pSound[MenuSelect]);
	//m_pSystem->createSound("../../data/NoteMusic_umikaze.mp3", FMOD_DEFAULT, 0, &m_pSound[GameBG]);
	//m_pSystem->createSound("../../data/CloseMusic.mp3", FMOD_DEFAULT, 0, &m_pSound[CloseBG]);
	//m_pSystem->createSound("../../data/ResultMusic.mp3", FMOD_DEFAULT, 0, &m_pSound[ResultBG]);
	//m_pSound[TitleBG]->setMode(FMOD_LOOP_NORMAL);
	//m_pSound[ResultBG]->setMode(FMOD_LOOP_NORMAL);
	//m_pSound[GameBG]->setMode(FMOD_LOOP_OFF);
	return true;
}
bool BSound::Render()
{
	return true;
}
bool BSound::Frame()
{
	return true;
}
bool BSound::Release()
{
	//m_pSound[TitleBG]->release();
	//m_pSound[GameBG]->release();
	//m_pSystem->close();
	//m_pSystem->release();
	return true;
}

bool BSound::playeffect(int SoundNum)
{
	bool playing = false;
	if (m_pChannel[SoundNum]->isPlaying(&playing)==false)
	{
		m_pChannel[SoundNum]->stop();
	}
	m_pSystem->playSound(m_pSound[SoundNum], 0, false, &m_pChannel[SoundNum]);
	return true;
}

bool BSound::play(int MusicNum)
{
	bool playing = false;
	if (m_pChannel[MusicNum]->isPlaying(&playing) != false)
	{
		m_pSystem->playSound(m_pSound[MusicNum], 0, false, &m_pChannel[MusicNum]);
	}
	return true;
}

bool BSound::stop(int MusicNum)
{
	m_pChannel[MusicNum]->stop();
	return true;
}

bool BSound::stop()
{
	bool paused;
	//m_pChannel[GameBG]->getPaused(&paused);
	//m_pChannel[GameBG]->setPaused(!paused);
	return true;
}

BSound::BSound()
{
}


BSound::~BSound()
{
}

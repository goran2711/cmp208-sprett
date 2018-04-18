#include "Audio.h"
#include <audio/audio_manager.h>

gef::AudioManager*	Audio::m_audioManager	= nullptr;
Int32				Audio::m_samples[NUM_SAMPLES];
bool				Audio::m_musicLoaded = true;

void Audio::init(gef::Platform& platform, const std::string& music)
{
	if (m_audioManager)
		return;

	m_audioManager = gef::AudioManager::Create();

	if (!m_audioManager)
		return;

	m_samples[JINGLE]			= m_audioManager->LoadSample("splash_jingle.wav", platform);
	m_samples[BUTTON_SELECT]	= m_audioManager->LoadSample("button_select.wav", platform);
	m_samples[BUTTON_CLICK]		= m_audioManager->LoadSample("button_click.wav", platform);
	m_samples[BOUNCE]			= m_audioManager->LoadSample("jump.wav", platform);
	m_samples[JETPACK]			= m_audioManager->LoadSample("rocket.wav", platform);
	m_samples[BEDSPRING]		= m_audioManager->LoadSample("spring.wav", platform);
	m_samples[GAME_OVER]		= m_audioManager->LoadSample("game_over.wav", platform);

	// LoadMusic returns code 0 (false) if successful
	m_musicLoaded = !(m_audioManager->LoadMusic((music == "" ? "music.wav" : music.c_str()), platform));
}

void Audio::cleanUp()
{
	if (!m_audioManager)
		return;

	m_audioManager->UnloadAllSamples();
}

void Audio::playSample(Sample sample)
{
	if (m_audioManager && sample >= 0 && sample < NUM_SAMPLES)
		m_audioManager->PlaySample(m_samples[sample]);
}

void Audio::startMusic()
{
	if (m_audioManager && m_musicLoaded)
		m_audioManager->PlayMusic();
}

void Audio::stopMusic()
{
	if (m_audioManager && m_musicLoaded)
		m_audioManager->StopMusic();
}

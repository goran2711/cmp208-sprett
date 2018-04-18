#pragma once
#include <gef.h>
#include <string>

namespace gef
{
	class Platform;
	class AudioManager;
}

class Audio
{
public:
	Audio() = delete;

	enum Sample
	{
		JINGLE,
		BUTTON_SELECT,
		BUTTON_CLICK,
		BOUNCE,
		JETPACK,
		BEDSPRING,
		GAME_OVER,
		NUM_SAMPLES
	};

	static void init(gef::Platform& platform, const std::string& music = "");
	static void cleanUp();

	static void playSample(Sample sample);

	static void startMusic();
	static void stopMusic();

private:
	static gef::AudioManager* m_audioManager;

	static Int32 m_samples[NUM_SAMPLES];
	static bool m_musicLoaded;
};


#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>
#include <map>

#include <OpenALSound.h>
#include <JsonPlaythroughParser.h>

class WAG_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;
	static Texture * cheryl;
	static JsonPlaythroughParser * playthrough;

	static std::string speaker;
	static bool skipEnabled;
	static int musicVolume;
	static int sfxVolume;

	static void init();

	// returns musicVolume, adjusted for use with setGain
	static float getMusicVolume();
	// returns musicVolume, adjusted for use with setGain
	static float getSfxVolume();
};
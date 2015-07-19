#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>
#include <map>

#include <OpenALSound.h>
#include <scenario/Scenario.h>

class WAG_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;
	static Scenario * playthrough;

	static int WAG_ResourceManager::eatCountMayor;
	static int WAG_ResourceManager::noEatCountMayor;
	static int WAG_ResourceManager::eatCountChipmunk;
	static int WAG_ResourceManager::noEatCountChipmunk;
	static int WAG_ResourceManager::eatCountMarten;
	static int WAG_ResourceManager::noEatCountMarten;
	static int WAG_ResourceManager::eatCountSquirrel;
	static int WAG_ResourceManager::noEatCountSquirrel;
	static int WAG_ResourceManager::eatCountDormouse;
	static int WAG_ResourceManager::noEatCountDormouse;

	static std::string speaker;
	static bool skipEnabled;
	static bool censored;
	static int musicVolume;
	static int sfxVolume;
	
	static int eatCount;
	static int noEatCount;

	static void init();

	// returns musicVolume, adjusted for use with setGain
	static float getMusicVolume();
	// returns musicVolume, adjusted for use with setGain
	static float getSfxVolume();

	// returns playthrough->getTexture(_id)->texture
	// if censored, checks for "_id.censored" first
	static Texture * getTexture(std::string _id);
};
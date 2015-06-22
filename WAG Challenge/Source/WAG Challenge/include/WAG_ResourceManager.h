#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>

#include <OpenALSound.h>
#include <JsonPlaythroughParser.h>

class WAG_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;
	static Texture * cheryl;
	static Texture * frameWithAnimals;
	static OpenAL_Sound * stream;
	static JsonPlaythroughParser * playthrough;

	static void init();
};
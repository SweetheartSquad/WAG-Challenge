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
	static Texture * burrow;
	static Texture * river;
	static Texture * foxPortrait;
	static Texture * rabbitPortrait;
	static OpenAL_Sound * stream;
	static JsonPlaythroughParser * playthrough;
	static Texture * scratchings;

	static void init();
};
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
	static OpenAL_Sound * blip;
	static JsonPlaythroughParser * playthrough;
	static Texture * scratchings;

	static char dialogueChar;

	static void init();
};
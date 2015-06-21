#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>

#include <OpenALSound.h>

class PD_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;
	static Texture * cheryl;
	static OpenAL_Sound * stream;

	static void init();
};
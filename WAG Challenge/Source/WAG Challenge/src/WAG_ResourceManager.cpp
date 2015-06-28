#pragma once

#include <WAG_ResourceManager.h>

#include <NumberUtils.h>
#include <Easing.h>

#include <math.h>

Texture * WAG_ResourceManager::cursor = new Texture("../assets/engine basics/cursor.png", 32, 32, true, false);
Texture * WAG_ResourceManager::cheryl = new Texture("../assets/engine basics/img_cheryl.jpg", 256, 256, true, false);
JsonPlaythroughParser * WAG_ResourceManager::playthrough = nullptr;


std::string WAG_ResourceManager::speaker = "blip";

void WAG_ResourceManager::init(){
	playthrough = new JsonPlaythroughParser("../assets/json_structure.json");
	resources.push_back(cursor);
	resources.push_back(cheryl);
}
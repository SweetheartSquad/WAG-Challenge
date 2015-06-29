#pragma once

#include <WAG_Game.h>
#include <WAG_SceneMain.h>

WAG_Game::WAG_Game() :
	Game(true, std::pair<std::string, Scene *>("test", new WAG_SceneMain(this)), true)
{
}

WAG_Game::~WAG_Game(){

}
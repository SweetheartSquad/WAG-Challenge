#pragma once

#include <WAG_Game.h>
#include <WAG_SceneMenu.h>

WAG_Game::WAG_Game() :
	Game(true, std::pair<std::string, Scene *>("MENU", new WAG_SceneMenu(this)), true)
{
}

WAG_Game::~WAG_Game(){

}
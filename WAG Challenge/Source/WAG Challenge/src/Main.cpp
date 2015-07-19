#pragma once

#include <Vox.h>
#include <node\Node.h>
#include <typeinfo>
#include <Texture.h>
#include <Game.h>
#include <WAG_ResourceManager.h>

#include <WAG_Game.h>
#include <Log.h>

// memory leak debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(void){
#ifdef _DEBUG
	_CrtMemState s1;
	_CrtMemCheckpoint( &s1 );
#endif

	vox::fullscreen = false;
	vox::initialize("WAG Challenge - post-jam build");
	WAG_ResourceManager::init();
	WAG_ResourceManager::load();
	WAG_Game * game = new WAG_Game();

	while (game->isRunning){
		game->performGameLoop();
	}
	
	delete game;
	game = nullptr;
	WAG_ResourceManager::destruct();
#ifdef _DEBUG
	if(Node::nodes.size() > 0){
		std::stringstream ss;
		ss << "Probable node leaks: " << Node::nodes.size();
		Log::warn(ss.str());

		for(auto n : Node::nodes){
			Log::warn(typeid(*n).name());
		}
	}
#endif

	vox::destruct();
	
#ifdef _DEBUG
	_CrtMemDumpAllObjectsSince(&s1);
#endif
}
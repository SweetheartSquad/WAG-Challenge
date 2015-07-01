#pragma once

#include <WAG_SceneMain.h>
#include <WAG_Game.h>
#include <WAG_ResourceManager.h>

#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentHsv.h>

#include <System.h>
#include <GLFW\glfw3.h>

#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>

#include <OpenALSound.h>
#include <DialogueDisplay.h>


WAG_SceneMain::WAG_SceneMain(Game * _game) :
	WAG_Scene(_game),
	dialogueDisplay(new DialogueDisplay(uiLayer->world, this, font, textShader, 1.f, 1.f))
{
	uiLayer->addChild(dialogueDisplay);

	// start the game proper
	WAG_ResourceManager::playthrough->currentConversation = WAG_ResourceManager::playthrough->conversations["MENU"];
	WAG_ResourceManager::playthrough->currentConversation->reset();
	Step step;
	dialogueDisplay->update(&step);
}

void WAG_SceneMain::update(Step * _step){
	WAG_ResourceManager::playthrough->getAudio("bgm")->sound->update(_step);
	
	// skip button
	if(keyboard->keyDown(GLFW_KEY_SPACE) && WAG_ResourceManager::skipEnabled){
		dialogueDisplay->dialogue->finishTicking();
		dialogueDisplay->autoProgress = true;
		dialogueDisplay->autoProgressTimer->targetSeconds = 0.01f;
	}else{
		dialogueDisplay->autoProgress = false;
	}

	if(keyboard->keyJustUp(GLFW_KEY_E)){	
		std::wcout << L"Calling RequestJSONValueAsync..." << std::endl;
		//RequestJSONValueAsync(label);
	}
	if(keyboard->keyJustUp(GLFW_KEY_ESCAPE)){	
		game->switchScene("MENU", false);
	}

	WAG_Scene::update(_step);
}
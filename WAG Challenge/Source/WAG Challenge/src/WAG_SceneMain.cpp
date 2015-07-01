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
#include <WAG_SceneStats.h>


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
	
	if(dialogueDisplay->options.size() > 0){
		if(WAG_ResourceManager::playthrough->characters.at("WAG_FOX")->variables.at("ATE") != "1"
			&&
			dialogueDisplay->options.back()->getText().substr(3,3) == L"Eat"){
			glm::vec3 p = dialogueDisplay->options.back()->getWorldPos();
			p.x += dialogueDisplay->options.back()->getWidth()*0.5f;
			p.y += dialogueDisplay->options.back()->getHeight()*0.5f;
			glm::vec3 m = mouseIndicator->getWorldPos();
			glm::vec3 d = p - m;
			float dist = glm::length(d);
			d = glm::normalize(d);
			float mult = stof(WAG_ResourceManager::playthrough->characters.at("WAG_FOX")->variables.at("HUNGER"));
			mouse->translate(glm::vec2(d.x, d.y)*1.5f*(std::max(50.f, dist)/50.f)*mult);
		}
	}

	if(dialogueDisplay->dialogue->getText() == L"The end for reals."){
		game->scenes.insert(std::pair<std::string, Scene *>("STATS", new WAG_SceneStats(game)));
		game->switchScene("STATS", true);
	}

	if(keyboard->keyJustUp(GLFW_KEY_ESCAPE)){	
		game->switchScene("MENU", false);
	}

	WAG_Scene::update(_step);
}
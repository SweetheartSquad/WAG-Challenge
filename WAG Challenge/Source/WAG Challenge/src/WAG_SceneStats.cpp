#pragma once

#include <WAG_SceneStats.h>

WAG_SceneStats::WAG_SceneStats(Game * _game) :
	WAG_Scene(_game),
	layout(new VerticalLinearLayout(uiLayer->world, this)),
	fade(2.f, layout)
{
	layout->setRationalWidth(1.f);
	layout->horizontalAlignment = kCENTER;

	WAG_Button * stats = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * mayor = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * dormouse = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * marten = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * chipmunk = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * squirrel = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * back = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	
	stats->mouseEnabled = false;
	mayor->mouseEnabled = false;
	dormouse ->mouseEnabled = false;
	marten->mouseEnabled = false;
	chipmunk->mouseEnabled = false;
	squirrel->mouseEnabled = false;
	
	stats->setText(L"Global Murders");

	{std::wstringstream ss;
	ss << "Mayor - Eaten: " << WAG_ResourceManager::eatCount << ", Spared: " << WAG_ResourceManager::noEatCount;
	mayor->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Dormouse - Eaten: " << WAG_ResourceManager::eatCount << ", Spared: " << WAG_ResourceManager::noEatCount;
	dormouse->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Marten - Eaten: " << WAG_ResourceManager::eatCount << ", Spared: " << WAG_ResourceManager::noEatCount;
	marten->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Chipmunk - Eaten: " << WAG_ResourceManager::eatCount << ", Spared: " << WAG_ResourceManager::noEatCount;
	chipmunk->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Squirrel - Eaten: " << WAG_ResourceManager::eatCount << ", Spared: " << WAG_ResourceManager::noEatCount;
	squirrel->setText(ss.str());}

	back->setText(L"Back");
	back->onClickFunction = [_game](NodeUI * _this){
		_game->switchScene("MENU", true);
	};
	
	layout->addChild(stats);
	stats->setMarginBottom(10);
	layout->addChild(mayor);
	layout->addChild(dormouse);
	layout->addChild(marten);
	layout->addChild(chipmunk);
	layout->addChild(squirrel);
	layout->addChild(back);
	back->setMarginBottom(0.1f);

	uiLayer->addChild(layout);

	fade.start();
}

void WAG_SceneStats::update(Step * _step){
	WAG_Scene::update(_step);
	fade.update(_step);
}
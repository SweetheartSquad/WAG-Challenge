#pragma once

#include <WAG_SceneStats.h>

WAG_SceneStats::WAG_SceneStats(Game * _game) :
	WAG_Scene(_game),
	layout(new VerticalLinearLayout(uiLayer->world, this))
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
	
	stats->setMouseEnabled(false);
	mayor->setMouseEnabled(false);
	dormouse->setMouseEnabled(false);
	marten->setMouseEnabled(false);
	chipmunk->setMouseEnabled(false);
	squirrel->setMouseEnabled(false);
	
	stats->setText(L"Global Player Decisions");

	{std::wstringstream ss;
	ss << "Mayor - Eaten: " << WAG_ResourceManager::eatCountMayor << ", Spared: " << WAG_ResourceManager::noEatCountMayor;
	mayor->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Dormouse - Eaten: " << WAG_ResourceManager::eatCountDormouse << ", Spared: " << WAG_ResourceManager::noEatCountDormouse;
	dormouse->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Marten - Eaten: " << WAG_ResourceManager::eatCountMarten << ", Spared: " << WAG_ResourceManager::noEatCountMarten;
	marten->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Chipmunk - Eaten: " << WAG_ResourceManager::eatCountChipmunk << ", Spared: " << WAG_ResourceManager::noEatCountChipmunk;
	chipmunk->setText(ss.str());}

	{std::wstringstream ss;
	ss << "Squirrel - Eaten: " << WAG_ResourceManager::eatCountSquirrel << ", Spared: " << WAG_ResourceManager::noEatCountSquirrel;
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
}

void WAG_SceneStats::update(Step * _step){
	WAG_Scene::update(_step);
}
#pragma once

#include <WAG_SceneMenu.h>
#include <WAG_SceneMain.h>

WAG_SceneMenu::WAG_SceneMenu(Game * _game) :
	WAG_Scene(_game)
{
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world, this);
	vl->setRationalWidth(0.75f);
	vl->setRationalHeight(0.5f);

	uiLayer->addChild(vl);
	
	WAG_Button * newGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 1.f);
	newGameButt->setText(L"New Game");
	newGameButt->onClickFunction = [_game](NodeUI * _this){
		_game->scenes.insert(std::pair<std::string, Scene *>("MAIN", new WAG_SceneMain(_game)));
		_game->switchScene("MAIN", false);
	};
	WAG_Button * continueGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 1.f);
	continueGameButt->setText(L"Continue Game");
	continueGameButt->mouseEnabled = false;
	continueGameButt->onClickFunction = [_game](NodeUI * _this){
		_game->switchScene("MAIN", false);
	};
	WAG_Button * optionsButt = new WAG_Button(uiLayer->world, this, font, textShader, 1.f);
	optionsButt->setText(L"Options");
	optionsButt->onClickFunction = [_game](NodeUI * _this){

	};
	
	vl->addChild(newGameButt);
	vl->addChild(continueGameButt);
	vl->addChild(optionsButt);
}
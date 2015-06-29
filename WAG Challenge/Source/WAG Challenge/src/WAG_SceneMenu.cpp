#pragma once

#include <WAG_SceneMenu.h>
#include <WAG_SceneMain.h>

WAG_SceneMenu::WAG_SceneMenu(Game * _game) :
	WAG_Scene(_game)
{
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world, this);
	vl->setRationalWidth(1.f);
	vl->setAutoresizeHeight();
	vl->setMarginBottom(10);
	vl->horizontalAlignment = kCENTER;
	
	WAG_Button * newGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.5f);
	WAG_Button * continueGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.5f);
	WAG_Button * optionsButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.5f);
	newGameButt->setText(L"New Game");
	newGameButt->onClickFunction = [this, continueGameButt](NodeUI * _this){
		// enable the continue button
		continueGameButt->mouseEnabled = true;

		// overwrite existing game
		if(game->scenes.count("MAIN") > 0){
			delete game->scenes["MAIN"];
		}
		game->scenes["MAIN"] = new WAG_SceneMain(game);
		game->switchScene("MAIN", false);
	};
	continueGameButt->setText(L"Continue Game");
	continueGameButt->mouseEnabled = false;
	continueGameButt->onClickFunction = [_game](NodeUI * _this){
		_game->switchScene("MAIN", false);
	};
	optionsButt->setText(L"Options");
	optionsButt->onClickFunction = [_game](NodeUI * _this){

	};
	
	vl->addChild(newGameButt);
	vl->addChild(continueGameButt);
	vl->addChild(optionsButt);
	uiLayer->addChild(vl);

	addMouse();
}

void WAG_SceneMenu::update(Step * _step){
	
	if(keyboard->keyJustUp(GLFW_KEY_ESCAPE)){	
		game->exit();
	}

	WAG_Scene::update(_step);
}
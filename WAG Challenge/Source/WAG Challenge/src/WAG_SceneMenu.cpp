#pragma once

#include <WAG_SceneMenu.h>
#include <WAG_SceneMain.h>

WAG_SceneMenu::WAG_SceneMenu(Game * _game) :
	WAG_Scene(_game),
	mainLayout(new VerticalLinearLayout(uiLayer->world, this)),
	optionsLayout(new VerticalLinearLayout(uiLayer->world, this))
{
	// main
	mainLayout->setRationalWidth(1.f);
	mainLayout->setAutoresizeHeight();
	mainLayout->horizontalAlignment = kCENTER;
	
	WAG_Button * newGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
	WAG_Button * continueGameButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
	WAG_Button * optionsButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
	WAG_Button * exitButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
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
	optionsButt->onClickFunction = [this](NodeUI * _this){
		uiLayer->removeChild(mainLayout);
		uiLayer->addChild(optionsLayout);
	};
	exitButt->setText(L"Exit Game");
	exitButt->onClickFunction = [_game](NodeUI * _this){
		_game->exit();
	};
	exitButt->setMarginBottom(10);
	
	mainLayout->addChild(newGameButt);
	mainLayout->addChild(continueGameButt);
	mainLayout->addChild(optionsButt);
	mainLayout->addChild(exitButt);
	uiLayer->addChild(mainLayout);
	
	// options
	optionsLayout->setRationalWidth(1.f);
	optionsLayout->setAutoresizeHeight();
	optionsLayout->horizontalAlignment = kCENTER;
	
	WAG_Button * skipButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
	WAG_Button * backButt = new WAG_Button(uiLayer->world, this, font, textShader, 0.6f);
	WAG_Button * musicVolumeDown = new WAG_Button(uiLayer->world, this, font, textShader, 0.1f);
	WAG_Button * musicVolumeUp = new WAG_Button(uiLayer->world, this, font, textShader, 0.1f);
	WAG_Button * sfxVolumeDown = new WAG_Button(uiLayer->world, this, font, textShader, 0.1f);
	WAG_Button * sfxVolumeUp = new WAG_Button(uiLayer->world, this, font, textShader, 0.1f);
	WAG_Button * sfxVolumeText = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	WAG_Button * musicVolumeText = new WAG_Button(uiLayer->world, this, font, textShader, 0.4f);
	
	HorizontalLinearLayout * musicvolume = new HorizontalLinearLayout(uiLayer->world, this);
	HorizontalLinearLayout * sfxvolume = new HorizontalLinearLayout(uiLayer->world, this);
	
	skipButt->setText(L"Space to Skip - Disabled");
	skipButt->onClickFunction = [skipButt](NodeUI * _this){
		WAG_ResourceManager::skipEnabled = !WAG_ResourceManager::skipEnabled;
		if(WAG_ResourceManager::skipEnabled){
			skipButt->setText(L"Space to Skip - Enabled");
		}else{
			skipButt->setText(L"Space to Skip - Disabled");
		}
	};
	
	musicVolumeText->setText(L"Music Volume: 5");
	musicVolumeDown->setText(L"-");
	musicVolumeUp->setText(L"+");
	sfxVolumeText->setText(L"Sound Volume: 5");
	sfxVolumeDown->setText(L"-");
	sfxVolumeUp->setText(L"+");
	musicVolumeText->mouseEnabled = false;
	sfxVolumeText->mouseEnabled = false;
	
	backButt->setText(L"Back");
	backButt->onClickFunction = [this](NodeUI * _this){
		uiLayer->removeChild(optionsLayout);
		uiLayer->addChild(mainLayout);
	};
	skipButt->setMarginBottom(10);
	
	musicvolume->addChild(musicVolumeDown);
	musicvolume->addChild(musicVolumeText);
	musicvolume->addChild(musicVolumeUp);
	
	sfxvolume->addChild(sfxVolumeDown);
	sfxvolume->addChild(sfxVolumeText);
	sfxvolume->addChild(sfxVolumeUp);
	
	optionsLayout->addChild(backButt);
	optionsLayout->addChild(musicvolume);
	optionsLayout->addChild(sfxvolume);
	optionsLayout->addChild(skipButt);
}

void WAG_SceneMenu::update(Step * _step){
	if(keyboard->keyJustUp(GLFW_KEY_ESCAPE)){	
		game->exit();
	}
	WAG_Scene::update(_step);
}
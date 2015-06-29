#pragma once

#include <WAG_Scene.h>

class WAG_SceneMenu : public WAG_Scene{
public:
	VerticalLinearLayout * mainLayout;
	VerticalLinearLayout * optionsLayout;


	WAG_SceneMenu(Game * _game);

	virtual void update(Step * _step) override;
};
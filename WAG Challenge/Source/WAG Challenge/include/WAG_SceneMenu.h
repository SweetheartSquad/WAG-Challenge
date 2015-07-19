#pragma once

#include <WAG_Scene.h>

class WAG_SceneMenu : public WAG_Scene{
public:
	VerticalLinearLayout * mainLayout;
	VerticalLinearLayout * optionsLayout;
	VerticalLinearLayout * warningLayout;
	NodeUI * bg;

	WAG_SceneMenu(Game * _game);
};
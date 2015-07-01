#pragma once

#include <WAG_Scene.h>
#include <TimeoutEffects.h>

class WAG_SceneStats : public WAG_Scene{
public:
	VerticalLinearLayout * layout;
	Fadein fade;
	WAG_SceneStats(Game * _game);

	virtual void update(Step * _step) override;
};
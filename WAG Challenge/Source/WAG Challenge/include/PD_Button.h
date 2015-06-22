#pragma once

#include <NodeUI.h>
#include <TextArea.h>

class PD_Button : public TextArea{
public:
	PD_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width);
	void update(Step * _step) override;
};
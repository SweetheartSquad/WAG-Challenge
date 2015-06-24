#pragma once

#include <DialogueText.h>

DialogueTextLabel::DialogueTextLabel(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader) :
	TextLabel(_world, _scene, _font, _textShader, 1.f),
	NodeBulletBody(_world)
{

}

DialogueTextLabel::~DialogueTextLabel(){

}

DialogueTextArea::DialogueTextArea(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader) :
	TextArea(_world, _scene, _font, _textShader, 1.f),
	NodeBulletBody(_world)
{

}

DialogueTextArea::~DialogueTextArea(){

}
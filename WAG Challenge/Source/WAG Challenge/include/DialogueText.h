#pragma once

#include <TextArea.h>
#include <TextLabel.h>

class DialogueTextLabel : public TextLabel{
public:
	DialogueTextLabel(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader);
	~DialogueTextLabel();
};

class DialogueTextArea : public TextArea{
public:
	//std::vector<Fadein *>

	DialogueTextArea(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader);
	~DialogueTextArea();
};
#pragma once

#include <Entity.h>
#include <LinearLayout.h>
#include <TextArea.h>
#include <Timeout.h>
#include <DialogueSay.h>
#include <WAG_Button.h>
#include <Easing.h>


class FadeTimeout : public Timeout{
public:
	NodeUI * target;
	FadeTimeout(float _targetSeconds, NodeUI * _target);

	virtual void update(Step * _step) override;
};


class DialogueDisplay : public NodeUI{
public:
	bool autoProgress;

	bool shouldSayNext;
	std::vector<DialogueSay *> stuffToSay;
	unsigned long int currentDialogue;

	DialogueDisplay(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height);
	~DialogueDisplay();
	
	LinearLayout * vlayout;
	LinearLayout * optionslayout;
	NodeUI * portraitPanel;
	NodeUI * portraitPanelOverlay;
	TextArea * speaker;
	TextArea * dialogue;
	Timeout * autoProgressTimer;
	
	bool waitingForInput;
	std::vector<WAG_Button *> options;
	Font * font;
	Shader * textShader;


	virtual void update(Step * _step) override;

	// increments currentDialogue and sets text to stuffToSay.at(currentDialogue)
	// if there is nothing left to say, returns false. Returns true otherwise
	bool sayNext();

	
	FadeTimeout * fadeTimeout;
	void loadPortrait(std::string _portrait);
};
#pragma once

#include <Entity.h>
#include <LinearLayout.h>
#include <TextArea.h>
#include <Timeout.h>
#include <DialogueSay.h>
#include <PD_Button.h>

class DialogueDisplay : public NodeUI{
public:
	bool shouldSayNext;
	std::vector<DialogueSay *> stuffToSay;
	unsigned long int currentDialogue;

	DialogueDisplay(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height);
	~DialogueDisplay();
	
	LinearLayout * hlayout;
	LinearLayout * vlayout;
	LinearLayout * optionslayout;
	NodeUI * portraitPanel;
	TextArea * speaker;
	TextArea * dialogue;
	Timeout * timeout;
	
	bool waitingForInput;
	std::vector<PD_Button *> options;
	Font * font;
	Shader * textShader;


	virtual void update(Step * _step) override;

	// increments currentDialogue and sets text to stuffToSay.at(currentDialogue)
	// if there is nothing left to say, returns false. Returns true otherwise
	bool sayNext();
};
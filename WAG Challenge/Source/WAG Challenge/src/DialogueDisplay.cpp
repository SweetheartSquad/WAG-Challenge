#pragma once

#include <DialogueDisplay.h>
#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentTexture.h>
#include <MeshInterface.h>
#include <Font.h>
#include <PD_ResourceManager.h>
#include <VerticalLinearLayout.h>
#include <HorizontalLinearLayout.h>

DialogueDisplay::DialogueDisplay(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	currentDialogue(0),
	waitingForInput(false),
	font(_font),
	textShader(_textShader),
	NodeUI(_world, _scene),
	NodeBulletBody(_world),
	shouldSayNext(false),
	autoProgress(false)
{
	setWidth(_width);
	setHeight(_height);
	//setPadding(0.1f, 0);
	//setMargin(0.1f, 0);
	
	vlayout = new VerticalLinearLayout(_world, _scene);
	vlayout->setRationalWidth(1.f, this);
	vlayout->setRationalHeight(1.f, this);
	vlayout->verticalAlignment = kTOP;
	vlayout->setPadding(10);

	portraitPanel = new NodeUI(_world, _scene);
	portraitPanel->setHeight(1.f);
	portraitPanel->setWidth(1.f);

	dialogue = new TextArea(_world, _scene, _font, _textShader, -1);
	dialogue->setRationalWidth(0.75f, vlayout);
	dialogue->setRationalHeight(1.f, vlayout);
	dialogue->verticalAlignment = kTOP;
	speaker = new TextArea(_world, _scene, _font, _textShader, -1);
	speaker->setRationalWidth(1.f, vlayout);
	speaker->setMarginTop(0.80f);

	
	PD_Button * progressButton = new PD_Button(world, scene, font, textShader, 1.f);
	progressButton->setText(L"->");
	progressButton->setRationalWidth(0.25f);
	progressButton->setRationalHeight(1.f);
	progressButton->onClickFunction = [this](NodeUI * _this) {
		this->autoProgressTimer->trigger();
	};

	optionslayout = new VerticalLinearLayout(_world, _scene);
	optionslayout->setRationalWidth(1.f);
	optionslayout->setRationalHeight(1.f);
	optionslayout->verticalAlignment = kTOP;
	optionslayout->horizontalAlignment = kCENTER;
	
	HorizontalLinearLayout * hlayout = new HorizontalLinearLayout(_world, _scene);
	hlayout->verticalAlignment = kTOP;
	hlayout->setRationalHeight(1.f);
	hlayout->setRationalWidth(1.f);
	hlayout->addChild(dialogue);
	hlayout->addChild(progressButton);

	addChild(portraitPanel);
	addChild(vlayout);
	vlayout->addChild(speaker);
	vlayout->addChild(hlayout);
	addChild(optionslayout);

	autoProgressTimer = new Timeout(2.f);
	autoProgressTimer->onCompleteFunction = [this](Timeout * _this) {
		this->shouldSayNext = true;
	};

	
}

DialogueDisplay::~DialogueDisplay(){

}

bool DialogueDisplay::sayNext(){
	// check if there's anything left to say at all
	if(currentDialogue >= stuffToSay.size()){
		return false;
	}

	// clear out the options list (they'll still be there if the last thing was an ask)
	/*while(options.size() > 0){
		delete options.back()->parents.at(0);
		options.pop_back();
	}*/

	// set the speaker and portrait
	std::string sp = stuffToSay.at(currentDialogue)->speaker;
	speaker->setText(std::wstring(sp.begin(), sp.end()));
	while(portraitPanel->background->mesh->textures.size() > 0){
		portraitPanel->background->mesh->popTexture2D();
	}
	if(stuffToSay.at(currentDialogue)->portrait == "cheryl"){
		portraitPanel->background->mesh->pushTexture2D(PD_ResourceManager::cheryl);
	}else if(stuffToSay.at(currentDialogue)->portrait == "animals"){
		portraitPanel->background->mesh->pushTexture2D(PD_ResourceManager::frameWithAnimals);
	}

	// move to the next text in the current dialogue object
	++stuffToSay.at(currentDialogue)->currentText;
	// check if there's any text left to say in the current dialogue object
	if(stuffToSay.at(currentDialogue)->currentText < stuffToSay.at(currentDialogue)->text.size()){
		// if there is, read the text in the current dialogue and return
		std::string thingToSay = stuffToSay.at(currentDialogue)->getCurrentText();
		dialogue->setText(std::wstring(thingToSay.begin(), thingToSay.end()));

		DialogueAsk * ask = dynamic_cast<DialogueAsk *>(stuffToSay.at(currentDialogue));
		if(ask != nullptr){
			waitingForInput = true;
			for(unsigned long int i = 0; i < ask->options.size(); ++i){
				//dialogue->appendText(std::wstring(s.begin(), s.end()));
				PD_Button * o = new PD_Button(world, scene, font, textShader, 1.f);
				std::wstringstream ss;
				ss << (i+1) << L". " << std::wstring(ask->options.at(i).begin(), ask->options.at(i).end());
				o->setText(ss.str());
				options.push_back(o);
				optionslayout->addChild(o);
				//o->parents.at(0)->scale(50,50,1);
				
				std::vector<Trigger *> optionResult = ask->optionsResults.at(i);
				o->onClickFunction = [this, optionResult](NodeUI * _this) {
					for(auto t : optionResult){
						t->trigger();
					}
					this->waitingForInput = false;
					this->shouldSayNext = true;
				};
			}
			optionslayout->setAutoresizeHeight();
			optionslayout->layoutChildren();
		}

		if(!waitingForInput){
			autoProgressTimer->restart();
		}

		return true;
	}else{
		// if there isn't move to the next dialogue object and try again
		++currentDialogue;
		return sayNext();
	}
}

void DialogueDisplay::update(Step * _step){
	if(shouldSayNext){
		while(options.size() > 0){
			optionslayout->removeChild(options.back());
			delete options.back();
			options.pop_back();
		}
		sayNext();
		shouldSayNext = false;
	}
	
	if(autoProgress){
		autoProgressTimer->update(_step);
	}
	NodeUI::update(_step);
}
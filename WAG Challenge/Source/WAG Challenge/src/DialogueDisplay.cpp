#pragma once

#include <DialogueDisplay.h>
#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentTexture.h>
#include <MeshInterface.h>
#include <Font.h>
#include <WAG_ResourceManager.h>
#include <VerticalLinearLayout.h>
#include <HorizontalLinearLayout.h>

DialogueDisplay::DialogueDisplay(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	waitingForInput(false),
	font(_font),
	textShader(_textShader),
	NodeUI(_world, _scene),
	NodeBulletBody(_world),
	shouldSayNext(false),
	autoProgress(false),
	stuffToSay(&WAG_ResourceManager::playthrough->currentConversation)
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
	portraitPanelOverlay = new NodeUI(_world, _scene);
	portraitPanelOverlay->setHeight(1.f);
	portraitPanelOverlay->setWidth(1.f);
	portraitPanelOverlay->setBackgroundColour(0,0,0,0);

	dialogue = new TextArea(_world, _scene, _font, _textShader, -1);
	dialogue->setRationalWidth(0.75f, vlayout);
	dialogue->setRationalHeight(1.f, vlayout);
	dialogue->verticalAlignment = kTOP;
	speaker = new TextArea(_world, _scene, _font, _textShader, -1);
	speaker->setRationalWidth(1.f, vlayout);
	speaker->setMarginTop(0.80f);

	
	WAG_Button * progressButton = new WAG_Button(world, scene, font, textShader, 1.f);
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
	addChild(portraitPanelOverlay);
	addChild(vlayout);
	vlayout->addChild(speaker);
	vlayout->addChild(hlayout);
	addChild(optionslayout);

	autoProgressTimer = new Timeout(2.f);
	autoProgressTimer->onCompleteFunction = [this](Timeout * _this) {
		this->shouldSayNext = true;
	};

	fadeTimeout = new FadeTimeout(1.f, portraitPanelOverlay);
	fadeTimeout->onCompleteFunction = [this](Timeout * _this){
		while(portraitPanel->background->mesh->textures.size() > 0){
			portraitPanel->background->mesh->popTexture2D();
		}
		while(portraitPanelOverlay->background->mesh->textures.size() > 0){
			portraitPanel->background->mesh->pushTexture2D(portraitPanelOverlay->background->mesh->popTexture2D());
		}
		portraitPanelOverlay->setBackgroundColour(0,0,0,0);
	};
}

DialogueDisplay::~DialogueDisplay(){
	delete autoProgressTimer;
	delete fadeTimeout;
}

bool DialogueDisplay::sayNext(){
	// move conversation forward
	if(!(*stuffToSay)->sayNextDialogue()){
		// if there's nothing left to say, return false
		return false;
	}

	// set the speaker
	std::string sp = (*stuffToSay)->getCurrentDialogue()->speaker;
	speaker->setText(std::wstring(sp.begin(), sp.end()));
	// set the portrait
	loadPortrait((*stuffToSay)->getCurrentDialogue()->portrait);
	
	// set the text
	std::string thingToSay = (*stuffToSay)->getCurrentDialogue()->getCurrentText();
	dialogue->setText(std::wstring(thingToSay.begin(), thingToSay.end()));

	// check for dialogue options
	DialogueAsk * ask = dynamic_cast<DialogueAsk *>((*stuffToSay)->getCurrentDialogue());
	if(ask != nullptr){
		waitingForInput = true;
		for(unsigned long int i = 0; i < ask->options.size(); ++i){
			//dialogue->appendText(std::wstring(s.begin(), s.end()));
			WAG_Button * o = new WAG_Button(world, scene, font, textShader, 1.f);
			std::wstringstream ss;
			ss << (i+1) << L". " << std::wstring(ask->options.at(i).begin(), ask->options.at(i).end());
			o->setText(ss.str());
			options.push_back(o);
			optionslayout->addChild(o);
			//o->parents.at(0)->scale(50,50,1);
				
			std::vector<Trigger *> optionResult = ask->optionsResults.at(i);
			o->onClickFunction = [this, optionResult](NodeUI * _this) {
				// remove the function pointers on all the other buttons to avoid multiple buttons getting triggered at the same time
				for(auto o : options){
					if(o != _this){
						o->onClickFunction = nullptr;
					}
				}

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

	// if auto-progression is enabled, restart the timer (should also set the duration here based on length of content)
	if(!waitingForInput){
		autoProgressTimer->restart();
	}

	return true;
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
	fadeTimeout->update(_step);
	NodeUI::update(_step);
}

void DialogueDisplay::loadPortrait(std::string _portrait){
	if(!fadeTimeout->complete){
		fadeTimeout->trigger();
	}
	while(portraitPanelOverlay->background->mesh->textures.size() > 0){
		portraitPanelOverlay->background->mesh->popTexture2D();
	}
	if((*stuffToSay)->getCurrentDialogue()->portrait == "cheryl"){
		portraitPanelOverlay->background->mesh->pushTexture2D(WAG_ResourceManager::cheryl);
	}else if((*stuffToSay)->getCurrentDialogue()->portrait == "animals"){
		portraitPanelOverlay->background->mesh->pushTexture2D(WAG_ResourceManager::frameWithAnimals);
	}else{
		portraitPanelOverlay->background->mesh->pushTexture2D(WAG_ResourceManager::frameWithAnimals);
	}

	fadeTimeout->restart();
}

FadeTimeout::FadeTimeout(float _targetSeconds, NodeUI * _target) :
	Timeout(_targetSeconds),
	target(_target)
{
}

void FadeTimeout::update(Step * _step){
	Timeout::update(_step);
	if(!complete){
		target->setBackgroundColour(0,0,0, Easing::easeInQuad(elapsedSeconds, 0, 1, targetSeconds));
	}
}
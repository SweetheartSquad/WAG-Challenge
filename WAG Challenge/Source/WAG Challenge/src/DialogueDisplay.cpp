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
	
	framePanel = new NodeUI(_world, _scene);
	framePanel->setHeight(1.f);
	framePanel->setWidth(1.f);
	framePanel->background->mesh->pushTexture2D(WAG_ResourceManager::playthrough->getTexture("TRANSPARENT")->texture);
	framePanelOverlay = new NodeUI(_world, _scene);
	framePanelOverlay->setHeight(1.f);
	framePanelOverlay->setWidth(1.f);
	framePanelOverlay->setBackgroundColour(0,0,0,0);
	framePanelOverlay->background->mesh->pushTexture2D(WAG_ResourceManager::playthrough->getTexture("TRANSPARENT")->texture);


	portraitPanel = new NodeUI(_world, _scene);
	portraitPanel->boxSizing = kCONTENT_BOX;
	portraitPanel->setHeight(0.09629f);
	portraitPanel->setWidth(0.05417f);
	portraitPanel->setMarginBottom(0.1662f);
	portraitPanel->setMarginLeft(0.5f - 0.05417f/2.f);
	portraitPanel->background->mesh->pushTexture2D(WAG_ResourceManager::playthrough->getTexture("TRANSPARENT")->texture);

	portraitPanelOverlay = new NodeUI(_world, _scene);
	portraitPanelOverlay->boxSizing = kCONTENT_BOX;
	portraitPanelOverlay->setHeight(0.09629f);
	portraitPanelOverlay->setWidth(0.05417f);
	portraitPanelOverlay->setMarginBottom(0.1662f);
	portraitPanelOverlay->setMarginLeft(0.5f - 0.05417f/2.f);
	portraitPanelOverlay->setBackgroundColour(0,0,0,0);
	portraitPanelOverlay->background->mesh->pushTexture2D(WAG_ResourceManager::playthrough->getTexture("TRANSPARENT")->texture);

	dialogue = new DialogueTextArea(_world, _scene, _font, _textShader);
	dialogue->setRationalWidth(0.8f, vlayout);
	dialogue->setRationalHeight(1.f, vlayout);
	dialogue->setMarginLeft(0.1f);
	dialogue->verticalAlignment = kTOP;
	speaker = new TextArea(_world, _scene, _font, _textShader, -1);
	speaker->setRationalWidth(1.f, vlayout);
	speaker->setMarginLeft(0.1f);
	speaker->setMarginTop(0.80f);
	speaker->setVisible(false);

	progressButton = new WAG_Button(world, scene, font, textShader, 1.f);
	progressButton->setText(L"Next");
	progressButton->setRationalWidth(1.f);
	progressButton->setRationalHeight(1.f);
	progressButton->onClickFunction = [this](NodeUI * _this) {
		this->autoProgressTimer->trigger();
	};
	progressButton->setVisible(false);

	optionslayout = new HorizontalLinearLayout(_world, _scene);
	optionslayout->setRationalWidth(1.f);
	optionslayout->setRationalHeight(1.f);
	optionslayout->verticalAlignment = kMIDDLE;
	optionslayout->horizontalAlignment = kCENTER;
	
	hlayout = new HorizontalLinearLayout(_world, _scene);
	hlayout->verticalAlignment = kTOP;
	hlayout->setRationalHeight(1.f);
	hlayout->setRationalWidth(1.f);
	hlayout->addChild(dialogue);
	
	addChild(framePanelOverlay);
	addChild(framePanel);
	addChild(portraitPanelOverlay);
	addChild(portraitPanel);
	addChild(vlayout);
	vlayout->addChild(speaker);
	vlayout->addChild(hlayout);
	addChild(optionslayout);
	addChild(progressButton);

	autoProgressTimer = new Timeout(2.f);
	autoProgressTimer->onCompleteFunction = [this](Timeout * _this) {
		if(dialogue->ticking){
			dialogue->finishTicking();
			autoProgressTimer->restart();
		}else{
			this->shouldSayNext = true;
		}
	};
	autoProgressTimer->start();

	fadeInTimeoutFrame = new Fadein(1.f, framePanel);
	fadeInTimeoutFrame->start();
	fadeOutTimeoutFrame = new Fadeout(1.f, framePanelOverlay);
	fadeOutTimeoutFrame->start();

	
	/*fadeInTimeoutFrame->onCompleteFunction = [this](Timeout * _this){
		while(framePanel->background->mesh->textures.size() > 0){
			framePanel->background->mesh->popTexture2D();
		}
		while(framePanelOverlay->background->mesh->textures.size() > 0){
			framePanel->background->mesh->pushTexture2D(framePanelOverlay->background->mesh->popTexture2D());
		}
		framePanelOverlay->setBackgroundColour(0,0,0,0);
		framePanelOverlay->setVisible(false);
		framePanel->setVisible(true);
	};*/

	fadeInTimeoutPortrait = new Fadein(1.f, portraitPanel);
	fadeInTimeoutPortrait->start();
	fadeOutTimeoutPortrait = new Fadeout(1.f, portraitPanelOverlay);
	fadeOutTimeoutPortrait->start();

	
	/*fadeInTimeoutPortrait->onCompleteFunction = [this](Timeout * _this){
		while(portraitPanel->background->mesh->textures.size() > 0){
			portraitPanel->background->mesh->popTexture2D();
		}
		while(portraitPanelOverlay->background->mesh->textures.size() > 0){
			portraitPanel->background->mesh->pushTexture2D(portraitPanelOverlay->background->mesh->popTexture2D());
		}
		portraitPanelOverlay->setBackgroundColour(0,0,0,0);
		portraitPanelOverlay->setVisible(false);
		portraitPanel->setVisible(true);
	};*/
}

DialogueDisplay::~DialogueDisplay(){
	delete autoProgressTimer;
	delete fadeInTimeoutFrame;
	delete fadeInTimeoutPortrait;
	delete fadeOutTimeoutFrame;
	delete fadeOutTimeoutPortrait;
}

bool DialogueDisplay::sayNext(){
	// trigger anything left on the current dialogue object
	// (note that if there are multiple lines of text for the object, the triggers will be called multiple times)
	if((*stuffToSay)->getCurrentDialogue()->currentText == (*stuffToSay)->getCurrentDialogue()->text.size()-1){
		for(Trigger * t : (*stuffToSay)->getCurrentDialogue()->triggers){
			t->trigger();
		}
	}

	// move conversation forward
	if(!(*stuffToSay)->sayNextDialogue()){
		// if there's nothing left to say, return false
		return false;
	}

	// set the speaker
	std::string sp = (*stuffToSay)->getCurrentDialogue()->speaker;
	speaker->setText(std::wstring(sp.begin(), sp.end()));
	WAG_ResourceManager::speaker = sp;


	// set the images
	loadFrame((*stuffToSay)->getCurrentDialogue()->portrait);
	loadPortrait((*stuffToSay)->getCurrentDialogue()->speaker);
	
	// set the text
	std::string thingToSay = (*stuffToSay)->getCurrentDialogue()->getCurrentText();
	dialogue->setText(std::wstring(thingToSay.begin(), thingToSay.end()));
	dialogue->tickerIn(0.05f);

	// check for dialogue options
	DialogueAsk * ask = dynamic_cast<DialogueAsk *>((*stuffToSay)->getCurrentDialogue());
	if(ask != nullptr){
		if(ask->currentText == ask->text.size()-1){
			waitingForInput = true;
			removeChild(progressButton);
			for(unsigned long int i = 0; i < ask->options.size(); ++i){
				//dialogue->appendText(std::wstring(s.begin(), s.end()));
				WAG_Button * o = new WAG_Button(world, scene, font, textShader, 0.3f);
				if(i != 0){
					o->setMarginLeft(10);
				}
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
					this->addChild(progressButton);
				};
			}

			std::string greyOption = WAG_ResourceManager::playthrough->characters["WAG_FOX"]->variables["OPTION"];
			if(greyOption != "NONE"){
				WAG_Button * o = new WAG_Button(world, scene, font, textShader, 0.3f);
				o->setMarginLeft(10);
				std::wstringstream ss;
				ss << (options.size()+1) << L". " << std::wstring(greyOption.begin(), greyOption.end());
				o->setText(ss.str());
				options.push_back(o);
				optionslayout->addChild(o);
				o->setMouseEnabled(false);
				o->setBackgroundColour(-0.2f, -0.2f, -0.2f, 0.5f);
			}

			optionslayout->layoutChildren();
		}
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
	fadeInTimeoutFrame->update(_step);
	fadeInTimeoutPortrait->update(_step);
	fadeOutTimeoutFrame->update(_step);
	//std::cout << fadeOutTimeoutPortrait->elapsedSeconds << std::endl;
	fadeOutTimeoutPortrait->update(_step);
	NodeUI::update(_step);
}

void DialogueDisplay::loadFrame(std::string _portrait){
	if(!fadeOutTimeoutFrame->complete){
		fadeOutTimeoutFrame->trigger();
	}if(!fadeInTimeoutFrame->complete){
		fadeInTimeoutFrame->trigger();
	}

	

	while(framePanelOverlay->background->mesh->textures.size() > 0){
		framePanelOverlay->background->mesh->popTexture2D();
	}
	while(framePanel->background->mesh->textures.size() > 0){
		framePanelOverlay->background->mesh->pushTexture2D(framePanel->background->mesh->popTexture2D());
	}

	framePanel->background->mesh->pushTexture2D(WAG_ResourceManager::getTexture(_portrait));
	
	fadeOutTimeoutFrame->restart();
	fadeInTimeoutFrame->restart();
}

void DialogueDisplay::loadPortrait(std::string _speaker){
	if(!fadeOutTimeoutPortrait->complete){
		fadeOutTimeoutPortrait->trigger();
	}if(!fadeInTimeoutPortrait->complete){
		fadeInTimeoutPortrait->trigger();
	}

	while(portraitPanelOverlay->background->mesh->textures.size() > 0){
		portraitPanelOverlay->background->mesh->popTexture2D();
	}
	while(portraitPanel->background->mesh->textures.size() > 0){
		portraitPanelOverlay->background->mesh->pushTexture2D(portraitPanel->background->mesh->popTexture2D());
	}

	portraitPanel->background->mesh->pushTexture2D(WAG_ResourceManager::getTexture(_speaker));
	
	fadeInTimeoutPortrait->restart();
	fadeOutTimeoutPortrait->restart();
}
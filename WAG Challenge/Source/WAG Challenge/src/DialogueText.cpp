#pragma once

#include <DialogueText.h>

DialogueTextLabel::DialogueTextLabel(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader) :
	TextLabel(_world, _scene, _font, _textShader, 1.f),
	NodeBulletBody(_world)
{

}

DialogueTextLabel::~DialogueTextLabel(){
	while(timers.size() > 0){
		delete timers.back();
		timers.pop_back();
	}
}

void DialogueTextLabel::tickerIn(){
	while(timers.size() > 0){
		delete timers.back();
		timers.pop_back();
	}

	for(unsigned long int i = 0; i < usedGlyphs.size(); ++i){
		UIGlyph * g = usedGlyphs.at(i);
		g->setVisible(false);
		Timeout * t = new Timeout(0.05f * i);
		t->onCompleteFunction = [g](Timeout * _this){
			g->setVisible(true);
		};
		t->start();
		timers.push_back(t);
	}
}

void DialogueTextLabel::update(Step * _step){
	TextLabel::update(_step);
	for(signed long int i = timers.size()-1; i >= 0; --i){
		timers.at(i)->update(_step);
		if(timers.at(i)->complete){
			delete timers.at(i);
			timers.erase(timers.begin() + i);
		}
	}
}

DialogueTextArea::DialogueTextArea(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader) :
	TextArea(_world, _scene, _font, _textShader, 1.f),
	NodeBulletBody(_world)
{

}

DialogueTextArea::~DialogueTextArea(){

}

void DialogueTextArea::tickerIn(){
	while(timers.size() > 0){
		delete timers.back();
		timers.pop_back();
	}

	float time = 0;
	for(unsigned long int i = 1; i < usedLines.size(); ++i){
		DialogueTextLabel * tl = dynamic_cast<DialogueTextLabel *>(usedLines.at(i));
		tl->setVisible(false);
		time += usedLines.at(i-1)->usedGlyphs.size() * 0.05f;
		Timeout * t = new Timeout(time);
		t->onCompleteFunction = [tl](Timeout * _this){
			tl->setVisible(true);
			tl->tickerIn();
		};
		t->start();
		timers.push_back(t);
	}
	for(unsigned long int i = 0; i < usedLines.size(); ++i){
		dynamic_cast<DialogueTextLabel *>(usedLines.at(i))->tickerIn();
	}
}

TextLabel * DialogueTextArea::getNewLine(){
	TextLabel * line;
	if(unusedLines.size() > 0) {
		line = unusedLines.back();
		unusedLines.pop_back();
	}else{
		line = new DialogueTextLabel(world, scene, font, textShader);
		line->setRationalWidth(1.f, this);
		line->horizontalAlignment = horizontalAlignment;
	}
	usedLines.push_back(line);
	addChild(line);
	return line;
}


void DialogueTextArea::update(Step * _step){
	TextArea::update(_step);
	for(signed long int i = timers.size()-1; i >= 0; --i){
		timers.at(i)->update(_step);
		if(timers.at(i)->complete){
			delete timers.at(i);
			timers.erase(timers.begin() + i);
		}
	}
}
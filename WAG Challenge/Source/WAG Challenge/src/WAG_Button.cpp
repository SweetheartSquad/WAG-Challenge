#pragma once

#include <WAG_Button.h>
#include <Font.h>
#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>
#include <MeshFactory.h>

WAG_Button::WAG_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width) :
	TextArea(_world, _scene, _font, _textShader, _width),
	NodeBulletBody(_world)
{
	mouseEnabled = true;
	horizontalAlignment = kCENTER;
	verticalAlignment = kTOP;
	setPadding(3);

	setBackgroundColour(195.f/255.f-1, 174.f/255.f-1, 155.f/255.f-1, 1.f);
}

void WAG_Button::update(Step * _step){
	for(TextLabel * l : usedLines){
		l->setPaddingBottom(15);
	}
	TextArea::update(_step);
	if(mouseEnabled){
		if(isHovered){
			if(isDown){
				for(TextLabel * l : usedLines){
					l->setBackgroundColour(27.f/255.f-1, 85.f/255.f-1, 42.f/255.f-1, 0.1f);
				}
			}else{
				for(TextLabel * l : usedLines){
					l->setBackgroundColour(27.f/255.f-1, 85.f/255.f-1, 42.f/255.f-1, 0.5f);
				}
			}
		}else{
			for(TextLabel * l : usedLines){
				l->setBackgroundColour(27.f/255.f-1, 85.f/255.f-1, 42.f/255.f-1, 1.f);
			}
		}
	}else{
		for(TextLabel * l : usedLines){
			l->setBackgroundColour(-0.8f, -0.8f, -0.8f, 0.5f);
		}
	}
}
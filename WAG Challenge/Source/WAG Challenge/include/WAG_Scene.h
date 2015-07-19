#pragma once

#include <Scene.h>
#include <WAG_Game.h>
#include <WAG_ResourceManager.h>
#include <WAG_Button.h>
#include <System.h>

#include <UILayer.h>

#include <BulletDebugDrawer.h>
#include <BulletWorld.h>

#include <Font.h>
#include <TextLabel.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentText.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <Mouse.h>
#include <Keyboard.h>

#include <Sprite.h>

class WAG_Scene : public Scene{
public:
	// main stuff
	UILayer * uiLayer;
	Sprite * mouseIndicator;

	// text stuff
	ComponentShaderBase * shader;
	ComponentShaderText * textShader;
	Font * font;

	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	virtual void update(Step * _step);

	WAG_Scene(Game * _game);
	~WAG_Scene();

protected:
	// call this after you've added everything to the scene so that the mouse is on top
	void addMouse();
};
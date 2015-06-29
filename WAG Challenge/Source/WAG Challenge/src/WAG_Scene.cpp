#pragma once

#include <WAG_Scene.h>

WAG_Scene::WAG_Scene(Game * _game) :
	Scene(_game),
	shader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	uiLayer(new UILayer(this, 0,0,0,0)),
	font(nullptr)
{
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();

	textShader->textComponent->setColor(glm::vec3(1.f, 1.f, 1.f));

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);

	// create the font based on the screen width
	font = new Font("../assets/fonts/Mathlete-Skinny.otf", sd.x/40, false);

	
	// create a mouse indicator and place it in the uiLayer
	mouseIndicator = new Sprite();
	uiLayer->childTransform->addChild(mouseIndicator);
	mouseIndicator->mesh->pushTexture2D(WAG_ResourceManager::cursor);
	mouseIndicator->parents.at(0)->scale(32, 32, 1);
	mouseIndicator->mesh->scaleModeMag = GL_NEAREST;
	mouseIndicator->mesh->scaleModeMin = GL_NEAREST;
	// we have to move the mouseIndicator's vertices to match the coordinates to the intuitive pointer location
	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x += 0.5f;
		mouseIndicator->mesh->vertices[i].y -= 0.5f;
	}
	mouseIndicator->mesh->dirty = true;
	mouseIndicator->setShader(uiLayer->shader, true);

	// add the uiLayer to the scene
	childTransform->addChild(uiLayer, false);
}

WAG_Scene::~WAG_Scene(){
	deleteChildTransform();
	shader->safeDelete();

	delete font;
}


void WAG_Scene::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	clear();
	Scene::render(_matrixStack, _renderOptions);
}
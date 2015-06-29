#pragma once

#include <WAG_Scene.h>

class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDrawer;
class Box2DMeshEntity;
class MeshEntity;

class ShaderComponentHsv;

class Shader;
class RenderSurface;
class StandardFrameBuffer;
class Material;
class Sprite;

class PointLight;

class BulletMeshEntity;
class BulletRagdoll;
class ComponentShaderText;
class DialogueDisplay;

class WAG_SceneMain : public WAG_Scene{
public:
	// does basically everything for the wag game
	DialogueDisplay * dialogueDisplay;

	virtual void update(Step * _step) override;

	WAG_SceneMain(Game * _game);
};
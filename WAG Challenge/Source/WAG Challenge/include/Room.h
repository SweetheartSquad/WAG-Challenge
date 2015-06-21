#pragma once

#include <MeshEntity.h>
#include <Entity.h>
#include <BulletMeshEntity.h>
#include <RoomLayout.h>
#include <vector>

enum Room_t{
	BEDROOM,
	LIVINGROOM,
	KITCHEN,
	BATHROOM,
	OFFICE,
	OTHER
};

class ComponentShaderBase;

class Room: public Entity {
public:

	std::vector<BulletMeshEntity *> boundaries;

	std::vector<int> furnitureSlots;
	std::vector<int> characterSlots;

	Room(BulletWorld * _world, ComponentShaderBase * _shader, RoomLayout_t _type, glm::vec2 _size, Texture * _wallTexture);
	~Room(void);

	void setShader(Shader * _shader, bool _default);

	void translatePhysical(glm::vec3 _v);
};

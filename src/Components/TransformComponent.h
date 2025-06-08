#pragma once
#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec2 position;
	glm::vec2 scale;
	double rotation;

	TransformComponent(glm::vec2 pos = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1,1), double rot = 0.0) : position(pos), scale(scale), rotation(rot)
	{

	}
};
#pragma once
#include <glm/glm.hpp>
struct RigidBodyComponent
{
	glm::vec2 velocity;

	RigidBodyComponent(glm::vec2 velocity_ = glm::vec2(0.0f, 0.0f))
	{
		velocity = velocity_;
	}
};
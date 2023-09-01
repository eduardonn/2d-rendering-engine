#pragma once

#include "Object.hpp"
#include <glm/ext/vector_float2.hpp>

using namespace glm;

class Rigidbody
{
public:
	Rigidbody(vec2* position, float radius);
	~Rigidbody();

	void AddVelocity(const vec2& value);

	vec2* Position;
	vec2 OldPosition;
	vec2 Velocity{ 0.0f };
	vec2 Acceleration{ 0.0f };
	float Radius;
	float Mass;
};

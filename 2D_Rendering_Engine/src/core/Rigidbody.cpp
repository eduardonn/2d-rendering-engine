#include "Rigidbody.hpp"
#include "Time.hpp"
#include "PhysicsSystem.hpp"

Rigidbody::Rigidbody(glm::vec2* position, float radius)
	: Position(position)
	, OldPosition(*position)
	, Radius(radius)
{
	PhysicsSystem::AddObject(this);

	// Using area as mass
	Mass = 3.1415f * radius * radius;
}

Rigidbody::~Rigidbody()
{
	PhysicsSystem::RemoveObject(this);
}

void Rigidbody::AddVelocity(const vec2& value)
{
	Velocity += value;
}
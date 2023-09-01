#include "Object.hpp"
#include "World.hpp"
#include "utils/debug_utils.hpp"

Object::Object()
{
	World::AddObject(this);
}

Object::~Object()
{
	World::RemoveObject(this);
}
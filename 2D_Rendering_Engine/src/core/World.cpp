#include "World.hpp"
#include "utils/debug_utils.hpp"

//std::vector<Object*> World::m_Objects;
std::unordered_set<Object*> World::g_Objects;

void World::AddObject(Object* object)
{
	g_Objects.insert(object);
}
void World::RemoveObject(Object* object)
{
	g_Objects.erase(object);
}

void World::Update()
{
	for (Object* object : g_Objects)
	{
		object->Update();
	}
}
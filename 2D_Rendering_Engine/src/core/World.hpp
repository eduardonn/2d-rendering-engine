#pragma once

#include <vector>
#include "Object.hpp"
#include <unordered_set>

class World
{
public:
	static std::unordered_set<Object*> g_Objects;

	void Update();
	static void AddObject(Object* object);
	static void RemoveObject(Object* object);
};